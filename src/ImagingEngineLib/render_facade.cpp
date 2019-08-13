/*=========================================================================

  Program:   ImagingEngine
  Module:    render_facade.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render_facade.h"
#include "render/render_param.h"
#include "render/render_param_mpr.h"
#include "render/render_param_cpr.h"
#include "render/render_param_vr.h"
#include "curve_source.h"
#include "data_source.h"
#include "batch/batch_generator_vr.h"
#include "batch/batch_generator_cpr.h"
#include "batch/batch_generator_mpr.h"
#include "batch/batch_image_source.h"
#include "control/image_control.h"
#include "control/image_control_2d.h"
#include "control/image_control_3d.h"
#include "tools/timer.h"
#include "tools/logger.h"
#include "exception/iexception.h"
#include "exception/data_exception.h"
#include "exception/render_exception.h"

using namespace DW;
using namespace DW::IMAGE;
using namespace DW::Render;
using namespace DW::Batch;
using namespace DW::Control;
using namespace DW::Exception;


RenderFacade *RenderFacade::instance_ = 0;
RenderFacade *RenderFacade::Get() 
{
	if (instance_ == nullptr)
		instance_ = new RenderFacade;
	return instance_;
}

IImageControl *RenderFacade::GetControl(string control_id)
{
	return RenderSource::Get()->GetImageControl(control_id);
}

IBitmap *RenderFacade::GetImageBuffer(string control_id)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return NULL;

	return control->GetOutput();
}

void RenderFacade::Zoom(string control_id, float scale)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	control->Zoom(scale);
	control->Update();
}

void RenderFacade::ZoomToSize(string control_id, int width, int height)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	//TODO update width and height...or redefine the interface...
	control->ZoomToSize(width, height);
	control->Update();
}

void RenderFacade::Pan(string control_id, float dx, float dy)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	control->Pan(dx, dy);
	control->Update();
}

void RenderFacade::MoveTo(string control_id, float move_vector[3])
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	control->Move(move_vector);
	control->Update();
}

void RenderFacade::Roll(string control_id, float angle)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	control->Roll(angle);
	control->Update();
}

void RenderFacade::Rotate(string control_id, float angle, float axis[3])
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	Vector3f axis_vec(axis[0], axis[1], axis[2]);
	control->Rotate3D(axis_vec, angle);
	control->Update();
}

int RenderFacade::SetOrientation(string control_id, OrientationType ori)
{
	IImageControl *control = GetControl(control_id);
	if (NULL == control) return RET_STATUS_ENGINE_INPUT_INVALID_PARAM;;

	IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(0));
	if (NULL == imaging) return RET_STATUS_ENGINE_INPUT_INVALID_PARAM;

	RenderParam *param = imaging->GetRenderer()->GetRenderParam();
	if (param == NULL) return RET_STATUS_ENGINE_INPUT_INVALID_PARAM;

	RenderMode mode = imaging->GetRenderMode();
	switch (mode)
	{
	case DW::IMAGE::RenderMode::SMART_RAYCASTING:
	case DW::IMAGE::RenderMode::RAYCASTING_GPU:
	case DW::IMAGE::RenderMode::RAYCASTING:
	case DW::IMAGE::RenderMode::TEXTURE_3D_GPU:
	case DW::IMAGE::RenderMode::ISO_SURFACE:
	case DW::IMAGE::RenderMode::SSD:
		{
			Camera *camera = imaging->GetRenderer()->GetCamera();
			if (camera){
				camera->SetCameraViewPlane(ori);
			}
			else{
				CGLogger::Info("camera is null for VR Imaging (" + imaging->GetImagingID() + ")");
				return RET_STATUS_ENGINE_INPUT_INVALID_PARAM;
			}
		}
		break;
	case DW::IMAGE::RenderMode::MPR:
	case DW::IMAGE::RenderMode::CROSS_SECTIONAL_MPR:
		{
			MPRRenderParam *param_mpr = dynamic_cast<MPRRenderParam *>(imaging->GetRenderer()->GetRenderParam());
			if (param_mpr){
				Vector3f row_vec, col_vec;
				//Point3f image_center;
				if (ori == OrientationType::AXIAL){
					row_vec[0] = 1.0f;
					row_vec[1] = 0.0f;
					row_vec[2] = 0.0f;
					col_vec[0] = 0.0f;
					col_vec[1] = 1.0f;
					col_vec[2] = 0.0f;
				}
				else if (ori == OrientationType::SAGITTAL){
					row_vec[0] = 0.0f;
					row_vec[1] = 1.0f;
					row_vec[2] = 0.0f;
					col_vec[0] = 0.0f;
					col_vec[1] = 0.0f;
					col_vec[2] = -1.0f;
				}
				else if (ori == OrientationType::CORONAL){
					row_vec[0] = 1.0f;
					row_vec[1] = 0.0f;
					row_vec[2] = 0.0f;
					col_vec[0] = 0.0f;
					col_vec[1] = 0.0f;
					col_vec[2] = -1.0f;
				}
				else{
					row_vec[0] = 1.0f;
					row_vec[1] = 0.0f;
					row_vec[2] = 0.0f;
					col_vec[0] = 0.0f;
					col_vec[1] = 1.0f;
					col_vec[2] = 0.0f;
				}
				param_mpr->SetPlaneVector(row_vec, col_vec);
			}
			else{
				CGLogger::Info("render param is null for MPR Imaging (" + imaging->GetImagingID() + ")");
				return RET_STATUS_ENGINE_INPUT_INVALID_PARAM;
			}
		}
		break;
	case DW::IMAGE::RenderMode::STRAIGHTENED_CPR:
	case DW::IMAGE::RenderMode::STRETECHED_CPR:
		{
			CPRRenderParam *param_cpr = dynamic_cast<CPRRenderParam *>(imaging->GetRenderer()->GetRenderParam());
			if (param_cpr){
				Vector3f vec_of_interest;
				if (ori == OrientationType::AXIAL){
					vec_of_interest[0] = 0.0f;
					vec_of_interest[1] = 0.0f;
					vec_of_interest[2] = -1.0f;
				}
				else if (ori == OrientationType::SAGITTAL){
					vec_of_interest[0] = 0.0f;
					vec_of_interest[1] = 1.0f;
					vec_of_interest[2] = 0.0f;
				}
				else if (ori == OrientationType::CORONAL){
					vec_of_interest[0] = 1.0f;
					vec_of_interest[1] = 0.0f;
					vec_of_interest[2] = 0.0f;
				}
				else{
					vec_of_interest[0] = 0.0f;
					vec_of_interest[1] = 0.0f;
					vec_of_interest[2] = -1.0f;
				}

				param_cpr->SetDirection(vec_of_interest[0], vec_of_interest[1], vec_of_interest[2]);
			}
			else{
				CGLogger::Info("render param is null for CPR Imaging (" + imaging->GetImagingID() + ")");
				return RET_STATUS_ENGINE_INPUT_INVALID_PARAM;
			}
		}
		break;
	default:
		break;
	}
	return RET_STATUS_SUCCESS;
}

int RenderFacade::SetOrientation(string control_id, OrientationType ori, float vec[3])
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return RET_STATUS_FAILURE;

	//TODO finish set orientation
	CGLogger::Info("function has no implementation.");
	return RET_STATUS_FAILURE;

	throw std::logic_error("function has no implementation.");
}

void RenderFacade::SetCPRCurveID(string control_id, string curve_id)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;
	
	IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(1));
	if (NULL == imaging) return;

	CPRRenderParam *param_cpr = dynamic_cast<CPRRenderParam *>(imaging->GetRenderer()->GetRenderParam());
	if (param_cpr == NULL) return;

	VolCurve *curve = CurveSource::Get()->GetCurveByName(current_series_uid_, curve_id);
	if (curve == NULL) return;

	param_cpr->SetCurve(curve);
}

void RenderFacade::SetPositionAlongCurve(string control_id, string curve_id, int point_index)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(1));
	if (NULL == imaging) return;

	MPRRenderParam *param_mpr = dynamic_cast<MPRRenderParam *>(imaging->GetRenderer()->GetRenderParam());
	if (param_mpr == NULL) return;

	VolCurve *curve = CurveSource::Get()->GetCurveById(current_series_uid_, curve_id);
	if (curve == NULL) return;

	Vector3f vx, vy;
	float ix, iy, iz;
	// Validate point_index is between min/max value
	int number_of_components = curve->GetNumberOfSamplePoint();
	if (point_index >= number_of_components){
		point_index = number_of_components - 1;
	}
	else if (point_index < 0){
		point_index = 0;
	}
	curve->GetSamplePoint(point_index, ix, iy, iz);
	curve->GetLocalPlaneVector(point_index, vx, vy);

	// Set imaging parameters
	param_mpr->SetImageCenter(ix, iy, iz);
	param_mpr->SetPlaneVector(vx, vy);

	imaging->Render();
}

void RenderFacade::SetDesiredUpdateRate(string control_id, float rate)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(0));
	if (NULL == imaging) return;

	VRRenderParam *param_vr = dynamic_cast<VRRenderParam *>(imaging->GetRenderer()->GetRenderParam());
	if (param_vr == NULL) return;

	param_vr->SetDesiredUpdateRate(rate);
}

void RenderFacade::RenderControl(string control_id)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	control->Update();
}

void RenderFacade::ResetControl(string control_id)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	//TODO reset control
	CGLogger::Info("function has no implementation.");

	throw std::logic_error("function has no implementation.");
}

int RenderFacade::ChangeSeries(string series_uid)
{
	CGLogger::Info("Change series called. SeriesUID: " + series_uid);
	current_series_uid_ = series_uid;
	VolData *data = ImageDataSource::Get()->GetVolData(series_uid);
	if (NULL == data){
		CGLogger::Error("no data is found when changing series.");
		return RET_STATUS_ENGINE_INPUT_INVALID_PARAM;
	}

	try {
		vector<IThreedImaging *> imaging_list = RenderSource::Get()->GetRenderControls();
		auto it = imaging_list.begin();
		for (; it!=imaging_list.end(); ++it){
			
			(*it)->SetData(data);
			RenderParam *param = (*it)->GetRenderer()->GetRenderParam();
			if (param){
				// Set default window width & level
				int width, level;
				data->GetDefaultWindowWidthLevel(width, level);
				param->SetWindowWidthLevel(width, level);
			}
			(*it)->GetRenderer()->SetData(data);
		}
	}
	catch (DataException &e) {
		CGLogger::Error("change series data error message: \r\n" + e.str());
		return e.GetErrorCode();
	}
	catch (RenderException &e) {
		CGLogger::Error("change series render error message: \r\n" + e.str());
		return e.GetErrorCode();
	}
	catch (...) {
		CGLogger::Error("change series error.");
		return RET_STATUS_ENGINE_EXCEPTION_RUNTIME_ERROR;
	}

	return RET_STATUS_SUCCESS;
}

int RenderFacade::UnloadSeries(string series_uid)
{
	vector<IThreedImaging *> imaging_list = RenderSource::Get()->GetRenderControls();
	auto it = imaging_list.begin();
	try
	{
		for (; it!=imaging_list.end(); ++it){
			(*it)->SetData(NULL);
			(*it)->GetRenderer()->ClearData();
		}

		ImageDataSource::Get()->Destroy(series_uid);
		CurveSource::Get()->DestroyAll(series_uid);
		RenderSource::Get()->ClearDataForAllControl();
	}
	catch(...){
		CGLogger::Info("unload series data error.");
		return RET_STATUS_ENGINE_EXCEPTION_UNLOAD_DATA;
	}

	return RET_STATUS_SUCCESS;
}

int RenderFacade::ClearContorl(string control_id)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return RET_STATUS_ENGINE_INPUT_INVALID_PARAM;
	try {
		control->Clear();
	}
	catch(...){
		CGLogger::Info("clear control data error.");
		return RET_STATUS_ENGINE_EXCEPTION_UNLOAD_DATA;
	}

	return RET_STATUS_SUCCESS;
}

void RenderFacade::SetImageSize(string control_id, int width, int height)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(0));
	if (NULL == imaging) return;

	//TODO 将此处代码移植到control内
	RenderParam *param = imaging->GetRenderer()->GetRenderParam();
	if (param == NULL) return;

	//TODO control设置display width & height
	param->SetWidth(width);
	param->SetHeight(height);
}

void RenderFacade::SetWindowWidthLevel(string control_id, int width, int level)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;
	
	control->WWWL(width, level);
	control->Update();
}

void RenderFacade::SetBlendMode(string control_id, int mode)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(0));
	if (NULL == imaging) return;

	RenderParam *param = imaging->GetRenderer()->GetRenderParam();
	if (param == NULL) return;

	if (mode == 1){
		param->SetBlendMode(BlendMode::MaximumIntensity);
	}
	else if (mode == 2){
		param->SetBlendMode(BlendMode::MinimumIntensity);
	}
	else{
		param->SetBlendMode(BlendMode::AverageIntensity);
	}
}

void RenderFacade::SetSlabThickness(string control_id, float thickness)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;


}

int RenderFacade::CreateVRRotationBatch(string control_id, 
						   string output, 
						   BlendMode mode, 
						   OrientationType init_ori, 
						   RotationDirection direction, 
						   float step_angle, 
						   float clip_percent,
						   int image_number,
						   int ww,
						   int wl)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) {
		CGLogger::Error(" no control [" + control_id + "] was found.");
		return RET_STATUS_ENGINE_INPUT_INVALID_PARAM;
	}
	// Clear last rendered images
	control->Clear();

	//// Create batch image source
	//BatchImageSource *source = new BatchImageSource();
	//source->SetCallback(dynamic_cast<IImageSouceCallback *>(control));
	//control->SetInput(0, source);
	BatchImageSource *source = static_cast<BatchImageSource *>(control->GetInput(0));
	if (source == NULL){
		CGLogger::Error("control [" + control_id + "] has no BatchImageSource input.");
		return RET_STATUS_ENGINE_INPUT_INVALID_PARAM;
	}

	IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(1));
	if (imaging == NULL){
		CGLogger::Error("control [" + control_id + "] has no IThreedImaging input.");
		return RET_STATUS_ENGINE_INPUT_INVALID_PARAM;
	}

	if (clip_percent < MathTool::kEpsilon ||
		clip_percent > 1.0f){
		CGLogger::Error("CreateVRRotationBatch got invalid clip percent value [" + to_string(clip_percent) + "]. the value will be modified to 1.0");
		clip_percent = 1.0f;
	}

	VRRotationBatchGenerator generator;
	try{

		generator.SetCallback(dynamic_cast<IBatchCallback *>(source));
		VRRotationBatchArgs args;
		args.SetOutputPath(output);
		args.SetBlendMode(mode);
		args.SetOrientation(init_ori);
		args.SetImageNumber(image_number);
		args.SetStep(step_angle);
		args.SetDirection(direction);
		args.SetClipPercent(clip_percent);
		args.SetWWWL(ww, wl);
		args.SetImaging(imaging);
		generator.Execute(&args);
	}
	catch(std::bad_cast){
		CGLogger::Error("CreateVRRotationBatch bad cast error.");
		return RET_STATUS_ENGINE_EXCEPTION_RUNTIME_ERROR;
	}
	catch(DataException &e){
		CGLogger::Error("CreateVRRotationBatch error message: " + e.str());
		return e.GetErrorCode();
	}
	catch(RenderException &e){
		CGLogger::Error("CreateVRRotationBatch error message: " + e.str());
		return e.GetErrorCode();
	}
	catch(...){
		CGLogger::Error("CreateVRRotationBatch error.");
		return RET_STATUS_ENGINE_EXCEPTION_RUNTIME_ERROR;
	}

	//try{
	//	// create files on disk
	//	control->SaveAsBitmap(output);
	//}
	//catch(...){
	//	return RET_STATUS_ENGINE_EXCEPTION_RUNTIME_ERROR;
	//}

	return RET_STATUS_SUCCESS;
}

int RenderFacade::CreateCPRRotationBatch(string control_id,
										  string output,
										  string curve_id,
										  OrientationType init_ori,
										  RotationDirection direction, 
										  float step_angle, 
										  int image_number,
										  int ww,
										  int wl)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) {
		CGLogger::Error(" no control [" + control_id + "] was found.");
		return RET_STATUS_ENGINE_INPUT_INVALID_PARAM;
	}
	// Clear last rendered images
	control->Clear();

	//// Create batch image source
	//BatchImageSource *source = new BatchImageSource();
	//source->SetCallback(dynamic_cast<IImageSouceCallback *>(control));
	//control->SetInput(0, source);
	BatchImageSource *source = static_cast<BatchImageSource *>(control->GetInput(0));
	if (source == NULL){
		CGLogger::Error("control [" + control_id + "] has no BatchImageSource input.");
		return RET_STATUS_ENGINE_INPUT_INVALID_PARAM;
	}

	IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(1));
	if (imaging == NULL){
		CGLogger::Error("control [" + control_id + "] has no IThreedImaging input.");
		return RET_STATUS_ENGINE_INPUT_INVALID_PARAM;
	}

	CPRRotationBatchGenerator generator;
	try {
		generator.SetCallback(dynamic_cast<IBatchCallback *>(source));
		CPRRotationBatchArgs args;
		args.SetOutputPath(output);
		args.SetOrientation(init_ori);
		args.SetImageNumber(image_number);
		args.SetStep(step_angle);
		args.SetDirection(direction);
		args.SetWWWL(ww, wl);
		args.SetCurveId(curve_id);
		args.SetImaging(imaging);
		//TODO Set Curve to render param object
		SetCPRCurveID(control_id, curve_id);
		generator.Execute(&args);
	}
	catch(std::bad_cast){
		CGLogger::Error("CreateCPRRotationBatch bad cast error.");
		return RET_STATUS_ENGINE_EXCEPTION_RUNTIME_ERROR;
	}
	catch(DataException &e){
		CGLogger::Error("CreateCPRRotationBatch error message: " + e.str());
		return e.GetErrorCode();
	}
	catch(RenderException &e){
		CGLogger::Error("CreateCPRRotationBatch error message: " + e.str());
		return e.GetErrorCode();
	}
	catch(...){
		CGLogger::Error("CreateCPRRotationBatch error.");
		return RET_STATUS_ENGINE_EXCEPTION_RUNTIME_ERROR;
	}

	//try {
	//	// create files on disk
	//	control->SaveAsBitmap(output);
	//}
	//catch(...){
	//	return RET_STATUS_ENGINE_EXCEPTION_RUNTIME_ERROR;
	//}

	return RET_STATUS_SUCCESS;
}

int RenderFacade::CreateMPRSlabBatch(string control_id,
									  string output,
									  BlendMode mode,
									  OrientationType ori,
									  float clip_percent, 
									  float thickness,
									  float spacing,
									  int ww,
									  int wl)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) {
		CGLogger::Error(" no control [" + control_id + "] was found.");
		return RET_STATUS_ENGINE_INPUT_INVALID_PARAM;
	}
	// Clear last rendered images
	control->Clear();

	//// Create batch image source
	//BatchImageSource *source = new BatchImageSource();
	//source->SetCallback(dynamic_cast<IImageSouceCallback *>(control));
	//control->SetInput(0, source);
	BatchImageSource *source = static_cast<BatchImageSource *>(control->GetInput(0));
	if (source == NULL){
		CGLogger::Error("control [" + control_id + "] has no BatchImageSource input.");
		return RET_STATUS_ENGINE_INPUT_INVALID_PARAM;
	}

	IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(1));
	if (imaging == NULL){
		CGLogger::Error("control [" + control_id + "] has no IThreedImaging input.");
		return RET_STATUS_ENGINE_INPUT_INVALID_PARAM;
	}

	if (clip_percent < MathTool::kEpsilon ||
		clip_percent > 1.0f){
			CGLogger::Error("CreateMPRSlabBatch got invalid clip percent value [" + to_string(clip_percent) + "]. the value will be modified to 1.0");
			clip_percent = 1.0f;
	}

	MPRSlabBatchGenerator generator;
	try {
		generator.SetCallback(dynamic_cast<IBatchCallback *>(source));
		MPRSlabBatchArgs args;
		args.SetOutputPath(output);
		args.SetBlendMode(mode);
		args.SetOrientation(ori);
		args.SetClipPercent(clip_percent);
		//args.SetStep(spacing);
		args.SetThickness(thickness);
		args.SetSpacingBetweenSlice(spacing);
		args.SetWWWL(ww, wl);
		args.SetImaging(imaging);
		generator.Execute(&args);
	}
	catch(std::bad_cast){
		CGLogger::Error("CreateMPRSlabBatch bad cast error.");
		return RET_STATUS_ENGINE_EXCEPTION_RUNTIME_ERROR;
	}
	catch(DataException &e){
		CGLogger::Error("CreateMPRSlabBatch error message: " + e.str());
		return e.GetErrorCode();
	}
	catch(RenderException &e){
		CGLogger::Error("CreateMPRSlabBatch error message: " + e.str());
		return e.GetErrorCode();
	}
	catch(...){
		CGLogger::Error("CreateMPRSlabBatch error.");
		return RET_STATUS_ENGINE_EXCEPTION_RUNTIME_ERROR;
	}

	//try {
	//	// create files on disk
	//	control->SaveAsBitmap(output);
	//}
	//catch(...){
	//	return RET_STATUS_ENGINE_EXCEPTION_RUNTIME_ERROR;
	//}

	return RET_STATUS_SUCCESS;
}