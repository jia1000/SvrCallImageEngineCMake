/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_stretched_cpr.cpp
  author: 	 zhangjian
  Brief:	 平面拉伸CPR渲染方法
			 算法流程：
			 1.按照感兴趣方向(vector of interest)计算cpr图像的最大宽度，
			 边距以包围盒范围为准，并以初始采样点的左右边距来表示
			 2.沿着curve曲线上每个采样点，计算过该点并沿着感兴趣方向的线段上
			 每个点的三维坐标
			 3.使用三线性插值计算该三维坐标对应的CT值
			 4.经过2和3步骤后，形成以curve长度为高度的cpr图像
			 5.应用窗宽窗位后输出图像
			 图像旋转：
			 上层传入感兴趣方向，并以此计算旋转后的图像

=========================================================================*/
#include "render/renderer_stretched_cpr.h"
#include "vtkImageEllipsoidSource.h"
#include "vtkLineSource.h"
#include "filter/spline_driven_image_slicer.h"
#include "tools/convert_vtk_image_data_to_rgba.h"
#include <vtkMath.h>
#include "tools/math.h"
#include "tools/vtk_image_data_creator.h"
#include "tools/timer.h"
#include "tools/logger.h"
#include "tools/configurations.h"
#include "thread/thd.h"
#include "exception/data_exception.h"
#include "exception/render_exception.h"

using namespace DW::Render;
//using namespace DW::Thread;
using namespace DW::Exception;

exception_ptr ex_ptr = nullptr;

StretchedCPRRenderer::StretchedCPRRenderer()
{
	render_mode_ = RenderMode::STRETECHED_CPR;
	show_buffer_ = new ShowBuffer();
}

StretchedCPRRenderer::~StretchedCPRRenderer()
{
	if (show_buffer_) {
		delete show_buffer_;
		show_buffer_ = NULL;
	}
}

void StretchedCPRRenderer::DoRender()
{
	Timer::begin("CPR::DoRender");

	CPRRenderParam* param_imp = dynamic_cast<CPRRenderParam *>(render_param_);
	if (param_imp == NULL){
		CGLogger::Error("StretchedCPRRenderer::DoRender >> no render param exists.");
		throw DataException(RET_STATUS_ENGINE_INPUT_INVALID_PARAM, "no render param exists.");
	}
	VolCurve* curve = param_imp->GetCurve();
	if (curve == NULL) {
		CGLogger::Error("StretchedCPRRenderer::DoRender >> no curve exists.");
		throw DataException(RET_STATUS_ENGINE_INPUT_INVALID_PARAM, "no curve exists.");
	}

	// 图像属性
	int image_width = volume_data_->GetSliceWidth();
	int image_height = volume_data_->GetSliceHeight();
	int image_count = volume_data_->GetSliceCount();
	
	//TODO Curve采样点以及切割线方向，都需要提前经过spacing比例转换
	float direction[3];
	param_imp->GetDirection(direction[0], direction[1], direction[2]);
	//TODO 此处长度单位为mm，以下插值时再映射到图像坐标
	float left_distance = 0.0f, right_distance = 0.0f;
	CalculateImageSize(param_imp->GetCurve(), direction, 1.0, left_distance, right_distance);
	// 缓存第一个采样点左右边距
	origin_padding_left_ = left_distance;
	origin_padding_right_ = right_distance;
	//// 放缩系数
	//float scale = param_imp->GetScale();

	// 计算出物理宽度，然后以spacings[0]为分辨率计算像素宽度
	int width = (int)((left_distance + right_distance) / voxel_spacing_[0] + 0.5);
	//int height = curve->GetNumberOfSamplePoint();
	int height = valid_sample_point_indexs_.size();

	CGLogger::Info("StretchedCPR calculated width/height=" + to_string(width) + "/" + to_string(height));

	if (width < MathTool::kEpsilon || height < MathTool::kEpsilon){
		throw RenderException(RET_STATUS_ENGINE_RENDER_INVALID_SIZE, "invalid calculated size.");
	}

	//////////////////////////////////////////////////////////////////////////
	//// 更新旋转中心点和旋转轴
	////TODO 需要换个位置设置CPR旋转轴
	//curve->GetCurveDirection(rotation_axis_[0], rotation_axis_[1], rotation_axis_[2]);
	//// 设置旋转轴放在上层
	//param_imp->SetAxis(rotation_axis_[0], rotation_axis_[1], rotation_axis_[2]);
	//////////////////////////////////////////////////////////////////////////

	float middle_sample[3] = {0};
	//curve->GetSamplePoint(height / 2, middle_sample[0], middle_sample[1], middle_sample[2]);
	curve->GetSamplePoint(valid_sample_point_indexs_[height / 2], middle_sample[0], middle_sample[1], middle_sample[2]);
	rotation_center_.x = middle_sample[0];
	rotation_center_.y = middle_sample[1];
	rotation_center_.z = middle_sample[2];
	param_imp->SetCenter(middle_sample[0], middle_sample[1], middle_sample[2]);
	// END
	
	int rows=height;
	int cols=width;

	short* ptr = NULL;
	show_buffer_->InitBufferData(width, height, 16);
	// 从vtk获取的ImageData像素值是经过偏移转换的，因此带着符号的：最大967，最小-3024
	// 因此此处用short类型取值
	ptr = reinterpret_cast<short *>(show_buffer_->GetShowBuffer());
	
	//float origin[3], current_sample[3], projection[3];
	float origin[3];
	curve->GetSamplePoint (0, origin[0], origin[1], origin[2]);
#ifndef CURVE_PATIENT_COORDINATE
	// CURVE转换到患者坐标系
	ImageToPatientCoordinate(origin[0], origin[1], origin[2]);
#endif // CURVE_PATIENT_COORDINATE

	float spacing_x = voxel_spacing_[0];
	float spacing_y = voxel_spacing_[1];
	float spacing_z = voxel_spacing_[2];
	// 获取CPU逻辑核心数
	int threads_number = ConfigurationManager::Get()->GetNumberOfThreads();
	int interval = rows / threads_number;
	thd_thread threads[MAXIMIUM_THREADS];
	ThreadParam params[MAXIMIUM_THREADS];
	int i;
	for( i=0; i<threads_number; ++i ){
		
		int start_row, end_row;
		start_row = i * interval;
		if (i == threads_number - 1){
			end_row = rows;
		}
		else{
			end_row = (i + 1) * interval;
		}
		ThreadParam param = params[i];
		params[i].flag = i;
		params[i].start_row = start_row;
		params[i].end_row = end_row;
		params[i].cols = cols;
		int k;
		for(k=0; k<3; ++k){
			params[i].origin[k] = origin[k];
			params[i].direction[k] = direction[k];
			params[i].voxel_spacing[k] = voxel_spacing_[k];
		}
		params[i].image_width = image_width;
		params[i].image_height = image_height;
		params[i].image_count = image_count;
		params[i].origin_padding_left = origin_padding_left_;
		params[i].ptr = ptr;
		params[i].param_imp = param_imp;
		params[i].volume_data = volume_data_;
		params[i].sample_point_indexs = valid_sample_point_indexs_;
		params[i].owner = this;

		thd_thread_detach(threads + i, (thd_thread_method)DoThreadWork, params + i);
	}

	for(i=0; i<threads_number; ++i){
		thd_thread_join(&threads[i]);
	}

	try {
		if (ex_ptr){
			rethrow_exception(ex_ptr);
		}
	}
	catch (std::out_of_range &e) {
		CGLogger::Error("out_of_range error occurred in stretched cpr rendering thread.");
		throw RenderException(RET_STATUS_ENGINE_EXCEPTION_OUT_OF_RANGE, e.what());
	}
	catch (std::bad_alloc &e) {
		CGLogger::Error("bad_alloc error occurred in stretched cpr rendering thread.");
		throw RenderException(RET_STATUS_ENGINE_EXCEPTION_OUT_OF_MEMORY, e.what());
	}
	catch (std::invalid_argument &e) {
		CGLogger::Error("invalid_argument error occurred in stretched cpr rendering thread.");
		throw RenderException(RET_STATUS_ENGINE_INPUT_INVALID_PARAM, e.what());
	}
	catch (std::overflow_error &e) {
		CGLogger::Error("overflow_error error occurred in stretched cpr rendering thread.");
		throw RenderException(RET_STATUS_ENGINE_EXCEPTION_OVERFLOW, e.what());
	}
	catch (std::underflow_error &e) {
		CGLogger::Error("underflow_error error occurred in stretched cpr rendering thread.");
		throw RenderException(RET_STATUS_ENGINE_EXCEPTION_OVERFLOW, e.what());
	}
	catch (...) {
		CGLogger::Error("error occurred in stretched cpr rendering thread.");
		throw RenderException(RET_STATUS_ENGINE_EXCEPTION_RUNTIME_ERROR, "error occurred in stretched cpr rendering thread.");
	}

	//////////////////////////////////////////////////////////////////////////
	// 以下设置输出图像平面信息
	// 计算平面法向量
	float curve_direction[3];
	curve->GetCurveDirection(curve_direction[0], curve_direction[1], curve_direction[2]);
	// Curve线方向向量
	Vector3d tmp_col_vector((double)curve_direction[0], (double)curve_direction[1], (double)curve_direction[2]);
	Vector3d row_vector((double)direction[0], (double)direction[1], (double)direction[2]);
	Vector3d normal_vector;
	MathTool::Cross(row_vector, tmp_col_vector, normal_vector);
	// 根据平面法向量再计算列向量
	Vector3d column_vector(0.0, 0.0, 0.0);
	MathTool::Cross(row_vector, normal_vector, column_vector);
	column_vector[2] = -1;
	ImageOrientation orientation(row_vector, column_vector);
	image_plane_->SetImageOrientation(orientation);
	// 左上角第一个像素在患者坐标系中的坐标
	Point3d left_top_origin;
	left_top_origin.x = origin[0] + direction[0] * (-origin_padding_left_);
	left_top_origin.y = origin[1] + direction[1] * (-origin_padding_left_);
	left_top_origin.z = origin[2] + direction[2] * (-origin_padding_left_);
	image_plane_->SetOrigin(left_top_origin);
	// 行列间距
	PixelSpacing2D spacing_2d(voxel_spacing_[0], voxel_spacing_[0]);
	image_plane_->SetSpacing(spacing_2d);
	// 层厚
	image_plane_->SetThickness(voxel_spacing_[0]);
	// 宽高
	image_plane_->SetRowLength(width * voxel_spacing_[0]);
	image_plane_->SetColumnLength(height * voxel_spacing_[0]);
	//////////////////////////////////////////////////////////////////////////

	//TODO show buffer可以不存储间距了
	show_buffer_->SetDPI(voxel_spacing_[0], voxel_spacing_[0]);

	Timer::end("CPR::DoRender");
	CGLogger::Info(Timer::summery());
}

void StretchedCPRRenderer::DoThreadWork(ThreadParam *param)
{
	float x[3];
	double distance_from_origin = 0.0;
	float current_sample[3], projection[3];
	try
	{
		int row, col;
		for(row=param->start_row; row<param->end_row; ++row){

			//param->param_imp->GetCurve()->GetSamplePoint (row, current_sample[0], current_sample[1], current_sample[2]);
			param->param_imp->GetCurve()->GetSamplePoint (param->sample_point_indexs[row], current_sample[0], current_sample[1], current_sample[2]);

#ifndef CURVE_PATIENT_COORDINATE
			// CURVE转换到患者坐标系
			ImageToPatientCoordinate(current_sample[0], current_sample[1], current_sample[2]);
#endif // CURVE_PATIENT_COORDINATE

			// 求解curpnt在向量LineOri上过prepnt点的投影点坐标(患者坐标系)
			MathTool::Compute3DProjectionOnLine (param->direction, param->origin, current_sample, projection);
			// 计算投影距离,同向为正,反向为负。单位mm
			float tmp_distance = MathTool::ComputeDistanceBetweenPoints (param->origin, projection, param->direction);
			for (col = 0; col < param->cols; col++)
			{
				// 单位mm
				distance_from_origin = (col * param->voxel_spacing[0] - param->origin_padding_left) + tmp_distance;
				float p[3];
				//param->param_imp->GetCurve()->GetSamplePoint (row, p[0], p[1], p[2]);
				param->param_imp->GetCurve()->GetSamplePoint (param->sample_point_indexs[row], p[0], p[1], p[2]);
				x[0] = (p[0] + param->direction[0] * distance_from_origin) / param->voxel_spacing[0];
				x[1] = (p[1] + param->direction[1] * distance_from_origin) / param->voxel_spacing[1];
				x[2] = (p[2] + param->direction[2] * distance_from_origin) / param->voxel_spacing[2];

				short val = -1024;
				if (x[0] < MathTool::kEpsilon || x[0] > param->image_width - 1 || 
					x[1] < MathTool::kEpsilon || x[1] > param->image_height - 1 || 
					x[2] < MathTool::kEpsilon || x[2] > param->image_count - 1){
						*(param->ptr + row * param->cols + col) = val;
						continue;
				}

				param->owner->TrilinearInterpolation(val, x[0], x[1], x[2], param->volume_data);
				*(param->ptr + row * param->cols + col) = val;

			}
		}
	}
	catch(...){
		ex_ptr = current_exception();
	}
}