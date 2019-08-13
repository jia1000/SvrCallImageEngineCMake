/*=========================================================================

  Program:   ImagingEngine
  Module:    batch_generator_cpr.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "batch/batch_generator_cpr.h"
#include "exception/data_exception.h"
#include "exception/render_exception.h"
#include "data/curve_line.h"
#include "render/render_param_cpr.h"

using namespace DW::Batch;
using namespace DW::Exception;


CPRRotationBatchGenerator::CPRRotationBatchGenerator()
{

}

CPRRotationBatchGenerator::~CPRRotationBatchGenerator()
{

}

void CPRRotationBatchGenerator::Execute(BatchArgs *args)
{
	CPRRotationBatchArgs *batch_args = static_cast<CPRRotationBatchArgs *>(args);
	if (NULL == batch_args) {
		if (this->callback_){
			this->callback_->OnBatchAllGenerated(0);
		}
		throw DataException(RET_STATUS_ENGINE_INPUT_INVALID_PARAM, "CPRRotationBatchArgs is null");
	}
	string output_path = batch_args->GetOutputPath();
	float step_angle = batch_args->GetStep();
	int image_number = batch_args->GetImageNumber();
	string curve_id = batch_args->GetCurveId();
	RotationDirection direction = batch_args->GetDirection();
	IThreedImaging *imaging = batch_args->GetImaging();
	OrientationType ori = batch_args->GetOrientation();
	int ww, wl;
	batch_args->GetWWWL(ww, wl);

	string str_msg = "vessel name: " + curve_id + "\r\n rotation: " + to_string(direction) + "\r\n image: " + to_string(image_number) + "\r\n ww/wl: " + to_string(ww) + "/" + to_string(wl);
	CGLogger::Info("CPRRotationBatchGenerator::Execute with the following parameters:\r\n" + str_msg);

	IThreedRenderer *renderer = imaging->GetRenderer();
	CPRRenderParam *cpr_param = dynamic_cast<CPRRenderParam *>(renderer->GetRenderParam());
	if (cpr_param){
		// 每次都要从冠状位开始旋转，在此复位切割线方向
		cpr_param->SetDirection(1.0f, 0.0f, 0.0f);
	}
	//TODO 列填充可以水平旋转，行填充可以垂直旋转
	float angle = 0.0f;
	Vector3f axis_vec(0.0f, 0.0f, 1.0f);
	switch (direction)
	{
	case DW::IMAGE::LEFT_TO_RIGHT:
		break;
	case DW::IMAGE::RIGHT_TO_LEFT:
		axis_vec[2] = -1.0f;
		break;
	case DW::IMAGE::HEAD_TO_FEET:
		axis_vec[0] = 1.0f;
		axis_vec[1] = 0.0f;
		axis_vec[2] = 0.0f;
		break;
	case DW::IMAGE::FEET_TO_HEAD:
		axis_vec[0] = 1.0f;
		axis_vec[1] = 0.0f;
		axis_vec[2] = 0.0f;
		break;
	default:
		break;
	}

	float count_angle = 0.0f;
	int i;
	for(i=0; i<image_number; ++i){

		if (i == 0){
			imaging->Render();
		}
		else{
			imaging->Rotate3D(axis_vec, step_angle);
		}

		ShowBuffer *buffer = new ShowBuffer();
		buffer->DeepCopy(imaging->GetShowBuffer());

		ImagePlane *plane = new ImagePlane();
		plane->DeepCopy(imaging->GetCurrentImagePlane());
			
		// Notify batch image source to cache this buffer
		if (this->GetCallback()){
			string path = output_path + curve_id + "_" + to_string(count_angle) + ".bmp";
			BufferResult *result = new BufferResult();
			result->buffer_data = buffer;
			result->image_plane = plane;
			result->instance_number = i + 1;
			result->file_name = path;
			result->step = step_angle;
			result->window_width = ww;
			result->window_level = wl;
			this->GetCallback()->OnBatchOneGenerated(result, true);
		}
		// 累加角度
		count_angle += step_angle;

		CGLogger::Info("CPRRotationBatchGenerator::get " + to_string(i + 1) + " render result from imaging.");
	}

	if (this->GetCallback()){
		this->GetCallback()->OnBatchAllGenerated(1);
	}
}

void CPRRotationBatchGenerator::Destroy()
{

}