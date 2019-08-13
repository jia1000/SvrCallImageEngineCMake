/*=========================================================================

  Program:   ImagingEngine
  Module:    batch_generator_mpr.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "batch/batch_generator_mpr.h"
#include "render/renderer_mpr.h"
#include "render/renderer_mpr_vtk.h"
#include "render/render_param_mpr.h"
#include "exception/data_exception.h"
#include "exception/render_exception.h"

using namespace DW::Batch;
using namespace DW::Render;
using namespace DW::Exception;


MPRSlabBatchGenerator::MPRSlabBatchGenerator()
{

}

MPRSlabBatchGenerator::~MPRSlabBatchGenerator()
{

}

void MPRSlabBatchGenerator::Execute(BatchArgs *args)
{
	MPRSlabBatchArgs *batch_args = static_cast<MPRSlabBatchArgs *>(args);
	if (NULL == batch_args)	{
		if (this->callback_){
			this->callback_->OnBatchAllGenerated(0);
		}
		throw DataException(RET_STATUS_ENGINE_INPUT_INVALID_PARAM, "MPRSlabBatchArgs is null");
	}

	string output_path = batch_args->GetOutputPath();
	float step_angle = batch_args->GetStep();
	float thickness = batch_args->GetThickness();
	float spacing = batch_args->GetSpacingBetweenSlice();
	OrientationType ori = batch_args->GetOrientation();
	BlendMode mode = batch_args->GetBlendMode();
	float clip = batch_args->GetClipPercent();
	IThreedImaging *imaging = batch_args->GetImaging();
	int ww, wl;
	batch_args->GetWWWL(ww, wl);

	string str_msg = "initial position: " + to_string(ori) + "\r\n mode: " + to_string(mode) + "\r\n thickness: " + to_string(thickness) + "\r\n spacing: " + to_string(spacing) + "\r\n clip: " + to_string(clip) + "\r\n ww/wl: " + to_string(ww) + "/" + to_string(wl);
	CGLogger::Info("MPRSlabBatchGenerator::Execute with the following parameters:\r\n" + str_msg);

	IThreedRenderer *renderer = imaging->GetRenderer();
	//TODO 设定初始位置判断
	Camera *camera = renderer->GetCamera();
	if (camera){
		camera->SetCameraViewPlane(OrientationType::CORONAL);
	}
	// 根据层厚，层间距和方向，计算有多少张图像
	int image_number = 0;
	// 重建方向上体数据物理长度，单位mm
	float length_in_direction = 0.0f;
	VolData *data = imaging->GetData();
	double image_spacings[3];
	data->GetPixelData()->GetSpacing(image_spacings);
	int image_width = data->GetSliceWidth();
	int image_height = data->GetSliceHeight();
	int image_count = data->GetSliceCount();
	int output_image_width = 0;
	int output_image_height = 0;
	float x_length = image_width * image_spacings[0];
	float y_length = image_height * image_spacings[1];
	float z_length = image_count * image_spacings[2];
	// 行列向量
	Vector3f row_vector, col_vector;
	// 初始位置图像中心点，单位mm
	Vector3f image_center(0.0f);
	Vector3f step_march(0.0f);
	string str_ori;
	switch (ori)
	{
	case OrientationType::AXIAL:
		length_in_direction =  z_length * clip;
		row_vector[0] = 1.0f;
		row_vector[1] = 0.0f;
		row_vector[2] = 0.0f;
		col_vector[0] = 0.0f;
		col_vector[1] = 1.0f;
		col_vector[2] = 0.0f;
		image_center[0] = x_length / 2.0f;
		image_center[1] = y_length / 2.0f;
		image_center[2] = 0.0;
		step_march[2] = spacing;
		str_ori = "axial";
		output_image_width = image_width;
		output_image_height = image_height;
		break;
	case OrientationType::CORONAL:
		length_in_direction = image_spacings[1] * image_height;
		row_vector[0] = 1.0f;
		row_vector[1] = 0.0f;
		row_vector[2] = 0.0f;
		col_vector[0] = 0.0f;
		col_vector[1] = 0.0f;
		col_vector[2] = 1.0f;
		image_center[0] = x_length / 2.0f;
		image_center[1] = 0.0;
		image_center[2] = z_length * clip / 2.0f; 
		step_march[1] = spacing;
		str_ori = "coronal";
		output_image_width = image_width;
		output_image_height = (int)(z_length * clip / image_spacings[0] + 0.5f);
		break;
	case OrientationType::SAGITTAL:
		length_in_direction = image_spacings[0] * image_width;
		row_vector[0] = 0.0f;
		row_vector[1] = 1.0f;
		row_vector[2] = 0.0f;
		col_vector[0] = 0.0f;
		col_vector[1] = 0.0f;
		col_vector[2] = 1.0f;
		image_center[0] = 0.0;
		image_center[1] = y_length / 2.0f;
		image_center[2] = z_length * clip / 2.0f; 
		step_march[0] = spacing;
		str_ori = "sagittal";
		output_image_width = image_height;
		output_image_height = (int)(z_length * clip / image_spacings[0] + 0.5f);
		break;
	default:
		break;
	}

	MPRRenderParam *param_imp = static_cast<MPRRenderParam *>(imaging->GetRenderer()->GetRenderParam());
	param_imp->SetPlaneVector(row_vector, col_vector);
	param_imp->SetBlendMode(mode);
	param_imp->SetThickness(thickness);
	param_imp->SetClip(clip);
	param_imp->SetWidth(output_image_width);
	param_imp->SetHeight(output_image_height);
	
	float pos;
	for(pos=0; pos<length_in_direction; pos+=spacing){

		image_number++;
		param_imp->SetImageCenter(image_center.x, image_center.y, image_center.z);
		// 移动到下一位置
		image_center += step_march;
		// Render图像
		imaging->Render();

		ShowBuffer *buffer = new ShowBuffer();
		buffer->DeepCopy(imaging->GetShowBuffer());

		ImagePlane *plane = new ImagePlane();
		plane->DeepCopy(imaging->GetCurrentImagePlane());

		// Notify batch image source to cache this buffer
		if (this->GetCallback()){
			string path = output_path + str_ori + "_" + to_string(image_number) + ".bmp";
			BufferResult *result = new BufferResult();
			result->buffer_data = buffer;
			result->image_plane = plane;
			result->instance_number = image_number;
			result->file_name = path;
			result->step = spacing;
			result->window_width = ww;
			result->window_level = wl;
			this->GetCallback()->OnBatchOneGenerated(result, true);
		}

		CGLogger::Info("MPRSlabBatchGenerator::get " + to_string(image_number) + " render result from imaging.");
	}

	if (this->GetCallback()){
		this->GetCallback()->OnBatchAllGenerated(1);
	}
}

void MPRSlabBatchGenerator::Destroy()
{

}