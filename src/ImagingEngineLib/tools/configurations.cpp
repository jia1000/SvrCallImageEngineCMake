/*=========================================================================

  Program:   ImagingEngine
  Module:    configurations.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "tools/configurations.h"


ConfigurationManager *ConfigurationManager::instance_ = 0;

ConfigurationManager::ConfigurationManager()
{
	vr_render_method_ = "SMART_RAYCASTING";
	mpr_render_method_ = "MPRRendererVtk";
	threads_number_ = GetCpuCoreNumber();
	vr_blend_mode_ = "MIP";
	log_level_ = 9;
	control_transform_ = "StoredValueTranform";
}

void ConfigurationManager::SetConfigReader(IConfigReader *reader)
{
	config_reader_ = reader;
	if (config_reader_){
		if (config_reader_->GetVRRenderMethod().length() > 0)
			vr_render_method_ = config_reader_->GetVRRenderMethod();
		if (config_reader_->GetMPRRenderMethod().length() > 0)
			mpr_render_method_ = config_reader_->GetMPRRenderMethod();
		if (config_reader_->GetNumberOfThreads() > 0)
			threads_number_ = config_reader_->GetNumberOfThreads();
		if (config_reader_->GetLogLevel() > 0)
			log_level_ = config_reader_->GetLogLevel();
		if (config_reader_->GetTransformMethod().length() > 0){
			control_transform_ = config_reader_->GetTransformMethod();
		}
	}
}