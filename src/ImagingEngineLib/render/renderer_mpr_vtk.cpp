/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_mpr_vtk.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render/renderer_mpr_vtk.h"
#include "render/render_param_mpr.h"
#include "tools/vtk_image_data_creator.h"
#include "tools/convert_vtk_image_data_to_rgba.h"
#include "tools/math.h"
#include "tools/timer.h"
#include "tools/logger.h"
#include "exception/data_exception.h"
#include "exception/render_exception.h"

using namespace std;
using namespace DW::IMAGE;
using namespace DW::Render;
using namespace DW::Exception;

MPRRendererVtk::MPRRendererVtk()
	: IThreedRenderer()
{
	is_first_render_ = true;
	render_mode_ = RenderMode::MPR;
	show_buffer_ = new ShowBuffer();
	image_plane_ = new ImagePlane();

	is_off_screen_rendering_ = true;
	
	vtk_image_reslice_ = vtkSmartPointer<vtkImageSlabReslice>::New();
}

MPRRendererVtk::~MPRRendererVtk()
{
	if (show_buffer_){
		delete show_buffer_;
		show_buffer_ = NULL;
	}
	if (image_plane_){
		delete image_plane_;
		image_plane_ = NULL;
	}
}

void MPRRendererVtk::Render()
{
	if (volume_data_ == NULL) {
		CGLogger::Error("MPRRendererVtk::Render >> volume data is null.");
		return;
	}

	DoRender();
}

ShowBuffer *MPRRendererVtk::GetShowBuffer()
{
	return show_buffer_;
}

void MPRRendererVtk::SetData(VolData* data)
{
	volume_data_ = data;
	if (NULL == volume_data_){
		vtk_image_reslice_->RemoveAllInputs();

		CGLogger::Info("MPRRendererVtk::SetData null");
		return;
	}

	if (volume_data_->GetPixelData()){
		volume_data_->GetPixelData()->GetSpacing(voxel_spacing_);
	}
	else{
		CGLogger::Error("MPRRendererVtk::SetData >> volume data has no pixel data.");
		throw DataException(RET_STATUS_ENGINE_DATA_PIXELDATA_NULL, "volume data has no pixel data.");
	}
	/// Workaround for vtkSmartVolumeMapper bug (https://gitlab.kitware.com/vtk/vtk/issues/17328)
	volume_data_->GetPixelData()->GetVtkImageData()->Modified();
#if VTK_MAJOR_VERSION > 5
	vtk_image_reslice_->SetInputData(volume_data_->GetPixelData()->GetVtkImageData());
#else
	vtk_image_reslice_->SetInput(volume_data_->GetPixelData()->GetVtkImageData());
#endif
	is_first_render_ = true;
}

void MPRRendererVtk::ClearData()
{
	volume_data_ = NULL;
	vtk_image_reslice_->RemoveAllInputs();

	CGLogger::Info("MPRRendererVtk::ClearData");
}

void MPRRendererVtk::DoRender()
{
	Timer::begin("MPR::DoRender");

	MPRRenderParam* param_imp = dynamic_cast<MPRRenderParam *>(render_param_);
	if (param_imp == NULL){
		CGLogger::Error("MPRRendererVtk::DoRender >> no render param exists.");
		throw DataException(RET_STATUS_ENGINE_INPUT_INVALID_PARAM, "no render param exists.");
	}

	int data_height = volume_data_->GetSliceHeight();
	// 获取图像中心点
	Point3f center_point;
	param_imp->GetImageCenter(center_point);
	// 获取成像平面的x/y方向向量
	Vector3f normal_vector, row_vector, column_vector;
	param_imp->GetPlaneVector(row_vector, column_vector);
	MathTool::Cross(row_vector, column_vector, normal_vector);
	// 因为vtk图像是从左下角开始，转换y轴坐标
	double axialElements[16] = {
		row_vector[0], column_vector[0], normal_vector[0], center_point.x,
		row_vector[1], column_vector[1], normal_vector[1], center_point.y,
		row_vector[2], column_vector[2], normal_vector[2], center_point.z,
		0, 0, 0, 1 
	};

	vtkSmartPointer<vtkMatrix4x4> resliceAxes =
		vtkSmartPointer<vtkMatrix4x4>::New();
	resliceAxes->DeepCopy(axialElements);
	vtk_image_reslice_->SetResliceAxes(resliceAxes);
	vtk_image_reslice_->SetSlabThickness(param_imp->GetThickness());
	vtk_image_reslice_->SetOutputSpacing(voxel_spacing_[0], voxel_spacing_[0], voxel_spacing_[0]);

	//////////////////////////////////////////////////////////////////////////
	// VolData数据场均是从头到脚（需要按照ImagePositionPatient排序），设置截取比例
	int output_extent[6];
	volume_data_->GetPixelData()->GetVtkImageData()->GetExtent(output_extent);
	double output_origin[3];
	int output_image_width = param_imp->GetWidth();
	int output_image_height = param_imp->GetHeight();
	output_extent[1] = output_extent[0] + output_image_width - 1;
	output_extent[3] = output_extent[2] + output_image_height - 1;
	vtk_image_reslice_->SetOutputExtent(output_extent);
	
	// Refer to the following address for detailed explanation
	// https://vtk.org/pipermail/vtkusers/2010-June/061035.html
	output_origin[0] = -(double)output_image_width / 2.0 * voxel_spacing_[0];
	output_origin[1] = -(double)output_image_height / 2.0 * voxel_spacing_[0];
	output_origin[2] = 0.0;

	vtk_image_reslice_->SetOutputOrigin(output_origin[0], output_origin[1], output_origin[2]);
	//////////////////////////////////////////////////////////////////////////


	if (param_imp->GetBlendMode() == BlendMode::MaximumIntensity){
		vtk_image_reslice_->SetBlendMode(VTK_IMAGE_SLAB_MAX);
	}
	else if (param_imp->GetBlendMode() == BlendMode::MinimumIntensity){
		vtk_image_reslice_->SetBlendMode(VTK_IMAGE_SLAB_MIN);
	}
	else {
		vtk_image_reslice_->SetBlendMode(VTK_IMAGE_SLAB_MEAN);
	}
	vtk_image_reslice_->InterpolateOn();
	if (param_imp->GetInterpolationMode() == InterpolationMode::Cubic){
		vtk_image_reslice_->SetInterpolationModeToCubic(); 
	}
	else if (param_imp->GetInterpolationMode() == InterpolationMode::Linear){
		vtk_image_reslice_->SetInterpolationModeToLinear();
	}
	else {
		vtk_image_reslice_->SetInterpolationModeToNearestNeighbor();
	}
	vtk_image_reslice_->SetBackgroundLevel(-1024);
	vtk_image_reslice_->SetOutputDimensionality(2);
	try
	{
		vtk_image_reslice_->Update();
	}
	catch(std::bad_alloc &e){
		CGLogger::Error("bad_alloc error occurred in vtk_image_reslice_ updating.");
		throw RenderException(RET_STATUS_ENGINE_EXCEPTION_OUT_OF_MEMORY, e.what());
	}

	vtkSmartPointer<vtkImageData> tmp_vtk_data = vtk_image_reslice_->GetOutput();
	
	//////////////////////////////////////////////////////////////////////////
	// 将vtkImageData转换为ShowBuffer
	try
	{
		short *pdata = reinterpret_cast<short *>(tmp_vtk_data->GetScalarPointer());
		int number_of_components = tmp_vtk_data->GetNumberOfScalarComponents();
		int width = tmp_vtk_data->GetDimensions()[0];
		int height = tmp_vtk_data->GetDimensions()[1];
		int x, y;
		int plane_size = width * height;
		short *raw_data = new short [plane_size];
		for (y=0; y<height; ++y){
			for (x=0; x<width; ++x){
				/*raw_data[y*width + x] = pdata[(height-1 - y) * width + x];*/
				//// 以下输出DICOM方向正确
				raw_data[y*width + x] = pdata[y* width + x];
			}
		}
		show_buffer_->SetBufferData(reinterpret_cast<char *>(raw_data), width, height, 16);

		delete [] raw_data;
	}
	catch(std::bad_alloc &e){
		CGLogger::Error("bad_alloc error occurred in mpr vtk pixel transform.");
		throw RenderException(RET_STATUS_ENGINE_EXCEPTION_OUT_OF_MEMORY, e.what());
	}
	catch(std::out_of_range &e){
		CGLogger::Error("out_of_range error occurred in mpr vtk pixel transform.");
		throw RenderException(RET_STATUS_ENGINE_EXCEPTION_OUT_OF_RANGE, e.what());
	}
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// 以下设置输出图像平面信息
	// 图像方向
	Vector3d row_vec_d(row_vector[0], row_vector[1], row_vector[2]);
	Vector3d col_vec_d(column_vector[0], column_vector[1], -column_vector[2]);
	ImageOrientation orientation(row_vec_d, col_vec_d);
	image_plane_->SetImageOrientation(orientation);
	// 行列间距
	PixelSpacing2D spacing_2d(voxel_spacing_[0], voxel_spacing_[0]);
	image_plane_->SetSpacing(spacing_2d);
	// 层厚
	image_plane_->SetThickness(param_imp->GetThickness());	
	// 左上角第一个像素在患者坐标系中的坐标
	// 需要注意的是vtkImageData存储的坐标值可能基于世界坐标系，此时需要转换到患者坐标系
	double left_top_origin[3];
	tmp_vtk_data->GetOrigin(left_top_origin);
	image_plane_->SetOrigin(left_top_origin[0], left_top_origin[1], left_top_origin[2]);
	// 宽高
	int extent[6];
	tmp_vtk_data->GetExtent(extent);
	image_plane_->SetRowLength((extent[1] - extent[0]) * voxel_spacing_[0]);
	image_plane_->SetColumnLength((extent[3] - extent[2]) * voxel_spacing_[0]);
	//////////////////////////////////////////////////////////////////////////
	
	Timer::end("MPR::DoRender");
	CGLogger::Debug(Timer::summery());
}

void MPRRendererVtk::BufferTransform()
{
	//if (output_vtk_image_data_ && show_buffer_){

	//	//原来的方法，用来对比生成的图像
	//	int width = output_vtk_image_data_->GetDimensions()[0];
	//	int height = output_vtk_image_data_->GetDimensions()[1];
	//	// 从8位转换到32位
	//	int slice = -1;

	//	int ww, wl;
	//	(dynamic_cast<MPRRenderParam *>(render_param_))->GetWindowWidthLevel(ww, wl);
	//	// convert to 32 bits bitmap
	//	vtkImageData *pTmpImageData = NULL;	
	//	ConvertVtkImagedataToRGBA* convert = new ConvertVtkImagedataToRGBA();
	//	if (false == convert->ConvertImageScalarsToRGBA(output_vtk_image_data_, &pTmpImageData, -1, ww, wl))
	//	{
	//		return;
	//	}

	//	//// 写入磁盘文件
	//	//vtkSmartPointer<vtkBMPWriter> writer = vtkSmartPointer<vtkBMPWriter>::New();
	//	//writer->SetFileName(dump_file_name_.c_str());
	//	//writer->SetInputData(pTmpImageData);
	//	//writer->Write();

	//	UNITDATASHOW* pdata = reinterpret_cast<UNITDATASHOW*>(pTmpImageData->GetScalarPointer());
	//	int number_of_components = pTmpImageData->GetNumberOfScalarComponents();

	//	show_buffer_->SetBufferData(pdata, width, height, number_of_components * 8);
	//}
}

void MPRRendererVtk::SetOffScreenRendering(bool flag)
{
	is_off_screen_rendering_ = flag;
}

bool MPRRendererVtk::PickByImagePos(const Vector2i& in_point, Vector3d& out_point)
{
	return false;
}

bool MPRRendererVtk::PickByImagePos(const int& x, const int& y, Vector3d& out_point)
{
	return false;
}

bool MPRRendererVtk::PickBy3DPoint(const Vector3d& in_point, int& x, int& y)
{
	return false;
}

float MPRRendererVtk::CalculateLength(Vector3d&, Vector3d&)
{
	return 0.0;
}

void MPRRendererVtk::GetOrientationTags(Vector3d&, Vector3d&, Vector3d&, Vector3d&)
{

}

void MPRRendererVtk::ComputeWorldToDisplay(Point3f& world_pos, Point3f& display_pos)
{

}

void MPRRendererVtk::ComputeWorldToDisplay(double x, double y, double z, double display_point[3])
{

}

void MPRRendererVtk::SetCameraViewPlane(const OrthogonalPlane &viewPlane)
{
	MPRRenderParam* param_imp = dynamic_cast<MPRRenderParam *>(render_param_);
	if (param_imp == NULL) return;
	
	current_view_plane_ = viewPlane;

	//float rotate_angle = param_imp->GetPlaneVector()
	////convert angle from degree to radian
	//double angleInRads = vtkMath::RadiansFromDegrees(angle);
	//double cosAngle = cos(angleInRads);
	//double sinAngle = sin(angleInRads);
	//// Set the slice orientation
	//vtkSmartPointer<vtkMatrix4x4> resliceAxes = vtkSmartPointer<vtkMatrix4x4>::New();

	//switch (current_view_plane_){
	//case OrthogonalPlane::XYPlane:
	//	double axialElements[16] = {
	//		1, 0, 0, 0,
	//		0, cosAngle,-sinAngle, 0,
	//		0, sinAngle,cosAngle, 0,
	//		0, 0, 0, 1 };
	//	resliceAxes->DeepCopy(axialElements);
	//}


	//vtkCamera *camera = param_imp->GetCamera()->GetVtkCamera();
	//if (camera == NULL) return;
	//// Adjust camera settings According to view
	//camera->SetFocalPoint(0.0, 0.0, 0.0);
	//switch (current_view_plane_)
	//{
	//case OrthogonalPlane::XYPlane:
	//	camera->SetViewUp(0.0, -1.0, 0.0);
	//	camera->SetPosition(0.0, 0.0, -1.0);
	//	break;
	//case OrthogonalPlane::YZPlane:
	//	camera->SetViewUp(0.0, 0.0, 1.0);
	//	camera->SetPosition(1.0, 0.0, 0.0);
	//	break;
	//case OrthogonalPlane::XZPlane:
	//	camera->SetViewUp(0.0, 0.0, 1.0);
	//	camera->SetPosition(0.0, -1.0, 0.0);
	//	break;
	//}
}
