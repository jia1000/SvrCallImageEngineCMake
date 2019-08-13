/*=========================================================================

  Program:   ImagingEngine
  Module:    render_source.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render_source.h"
#include "tools/string_util.h"
#include "render/render_param.h"
#include "render/render_param_cpr.h"
#include "render/render_param_mpr.h"
#include "render/render_param_vr.h"
#include "render/renderer_3d.h"
#include "render/renderer_cpr.h"
#include "render/renderer_stretched_cpr.h"
#include "render/renderer_straightened_cpr.h"
#include "render/renderer_mpr.h"
#include "render/renderer_mpr_vtk.h"
#include "render/renderer_vr.h"
#include "render/imaging_3d.h"
#include "render/imaging_cpr.h"
#include "render/imaging_mpr.h"
#include "render/imaging_vr.h"
#include "control/image_control_2d.h"
#include "control/image_control_3d.h"
#include "processing/image_transform.h"
#include "processing/image_transform_opencv.h"
#include "processing/stored_value_transform.h"
#include "tools/configurations.h"
#include "tools/logger.h"
#include "batch/batch_image_source.h"

using namespace DW;
using namespace DW::IMAGE;
using namespace DW::Render;
using namespace DW::CV;


RenderSource *RenderSource::instance_ = 0;
RenderSource *RenderSource::Get() 
{
	if (instance_ == nullptr)
		instance_ = new RenderSource;
	return instance_;
}

IThreedImaging *RenderSource::CreateRenderControl(string control_id, RenderControlType type)
{
	if (control_id.empty()){
		control_id = GenerateGUID();
	}

	IThreedRenderer *renderer = CreateRenderer(type);
	IThreedImaging *imaging = NULL;
	switch (type)
	{
	case DW::IMAGE::RenderControlType::VR:
		{
			imaging = new VRImaging(control_id);
			imaging->SetRenderer(renderer);
		}
		break;
	case DW::IMAGE::RenderControlType::MPR:
	case DW::IMAGE::RenderControlType::CROSS_SECTIONAL_MPR:
		{
			imaging = new MPRImaging(control_id);
			imaging->SetRenderer(renderer);
		}
		break;
	case DW::IMAGE::RenderControlType::STRETECHED_CPR:
	case DW::IMAGE::RenderControlType::STRAIGHTENED_CPR:
		{
			imaging = new CPRImaging(control_id);
			imaging->SetRenderer(renderer);
		}
		break;
	default:
		break;
	}

	if (imaging){
		imaging_list_.push_back(imaging);
	}
	return imaging;
}

IImageControl* RenderSource::GetImageControl(string control_id)
{
	auto it = image_control_list_.begin();
	for (; it<image_control_list_.end(); ++it){
		if ((*it)->GetControlID() == control_id){
			return (*it);
		}
	}
	return NULL;
}

IImageControl *RenderSource::CreateImageControl(string control_id, ImageControlType type)
{
	bool found = false;
	IImageControl *control = NULL;
	auto it = image_control_list_.begin();
	while (it != image_control_list_.end()){
		if ((*it)->GetControlID() == control_id){
			found = true;
			control = *it;
			break;
		}
		++it;
	}
	if (false == found){
		// Create a new one
		if (ImageControlType::CONTROL_TWOD == type){
			control = new TwodImageControl(control_id);
		}
		else{
			control = new ThreedImageControl(control_id);
		}

		image_control_list_.push_back(control);
	}

	return control;
}

IImageControl *RenderSource::CreateTwodImageControl(string control_id, RenderControlType type)
{
	IImageControl *control = CreateImageControl(control_id, ImageControlType::CONTROL_TWOD);
	if (control){
		IThreedImaging *imaging = CreateRenderControl(control_id, type);
		if (ConfigurationManager::Get()->GetImageSourceMethod() == "BatchImageSource"){
			// Create batch image source
			BatchImageSource *source = new BatchImageSource();
			source->SetCallback(dynamic_cast<IImageSouceCallback *>(control));
			control->SetInput(0, source);
		}
		control->SetInput(1, imaging);
		if (ConfigurationManager::Get()->GetTransformMethod() == "ImageTranformOpenCV"){
			control->SetTransform(new ImageTranformOpenCV());

			CGLogger::Info("Create image transform with ImageTranformOpenCV");
		}
		else{
			control->SetTransform(new StoredValueTranform());

			CGLogger::Info("Create image transform with StoredValueTranform");
		}
	}
	return control;
}

IImageControl *RenderSource::CreateThreedImageControl(string control_id, RenderControlType type)
{
	IImageControl *control = CreateImageControl(control_id, ImageControlType::CONTROL_THREED);
	if (control){
		IThreedImaging *imaging = CreateRenderControl(control_id, type);
		control->SetInput(0, imaging);
		control->SetTransform(new ImageTranformOpenCV());
	}
	return control;
}

IThreedRenderer *RenderSource::CreateRenderer(RenderControlType type)
{
	IThreedRenderer *renderer = NULL;
	// 通过type以及配置来选择采用哪种Renderer
	switch (type)
	{
	case DW::IMAGE::RenderControlType::VR:
		{
			VRRenderParam *param_vr = new VRRenderParam();
			VolumeRenderer *vol_renderer = new VolumeRenderer();
			renderer = vol_renderer;
			string render_mode = ConfigurationManager::Get()->GetVRRenderMethod();
			if (render_mode == "RAYCASTING_GPU"){
				vol_renderer->SetRenderingMode(RenderMode::RAYCASTING_GPU);

				CGLogger::Info("Create vr renderer with RAYCASTING_GPU");
			}
			else if (render_mode == "RAYCASTING"){
				vol_renderer->SetRenderingMode(RenderMode::RAYCASTING);

				CGLogger::Info("Create vr renderer with RAYCASTING");
			}
			else if (render_mode == "TEXTURE"){
				vol_renderer->SetRenderingMode(RenderMode::SMART_RAYCASTING);

				CGLogger::Info("Create vr renderer with RAYCASTING");
			}
			else{
				vol_renderer->SetRenderingMode(RenderMode::SMART_RAYCASTING);

				CGLogger::Info("Create vr renderer with SMART_RAYCASTING");
			}
			if (ConfigurationManager::Get()->GetVRBlendMode() == "MIP"){
				vol_renderer->SetBlendMode(BlendMode::MaximumIntensity);

				CGLogger::Info("Create vr renderer with MIP blend mode");
			}	
			vol_renderer->SetIndependentComponents(true);
			vol_renderer->SetRenderParam(param_vr);
		}		
		break;
	case DW::IMAGE::RenderControlType::MPR:
	case DW::IMAGE::RenderControlType::CROSS_SECTIONAL_MPR:
		{
			MPRRenderParam *param_mpr = new MPRRenderParam();
			param_mpr->SetWidth(512);
			param_mpr->SetHeight(512);
			param_mpr->SetImageCenter(0,0,0);
			param_mpr->SetRotationCenter(0,0,0);		
			param_mpr->SetBlendMode(BlendMode::MaximumIntensity);
			param_mpr->SetInterpolationMode(InterpolationMode::Cubic);
			if (ConfigurationManager::Get()->GetMPRRenderMethod() == "MPRRendererVtk"){
				renderer = new MPRRendererVtk();

				CGLogger::Info("Create mpr renderer with MPRRendererVtk");
			}
			else{
				renderer = new MPRRenderer();

				CGLogger::Info("Create mpr renderer with MPRRenderer");
			}			
			renderer->SetRenderParam(param_mpr);
		}		
		break;
	case DW::IMAGE::RenderControlType::STRETECHED_CPR:
		{
			CPRRenderParam *param = new CPRRenderParam();
			param->SetWidth(512);
			param->SetHeight(512);		
			param->SetDirection(1.0f, 0.0f, 0.0f);		
			renderer = new StretchedCPRRenderer();
			renderer->SetRenderParam(param);
		}		
		break;
	case DW::IMAGE::RenderControlType::STRAIGHTENED_CPR:
		{
			CPRRenderParam *param_cpr = new CPRRenderParam();
			param_cpr->SetWidth(512);
			param_cpr->SetHeight(512);		
			param_cpr->SetDirection(1.0f, 0.0f, 0.0f);		
			renderer = new StraightededCPRRenderer();
			renderer->SetRenderParam(param_cpr);
		}
		break;
	default:
		break;
	}

	return renderer;
}

void RenderSource::ClearDataForAllControl()
{
	auto it = image_control_list_.begin();
	for (; it<image_control_list_.end(); ++it){
		(*it)->Clear();
	}
}