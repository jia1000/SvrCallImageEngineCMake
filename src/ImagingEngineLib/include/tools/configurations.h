/*=========================================================================

  Program:   ImagingEngine
  Module:    configurations.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "tools/function.h"

class IConfigReader
{
public:
	/// VR渲染方法，返回值SMART_RAYCASTING, RAYCASTING_GPU ...
	virtual string GetVRRenderMethod() = 0;
	/// MPR渲染方法，返回值MPRRenderer, MPRRendererVtk
	virtual string GetMPRRenderMethod() = 0;
	/// 并发任务多线程的数目，如果不配置则自动取CPU逻辑核心数
	virtual int GetNumberOfThreads() = 0;
	///// VR图像的绘制模式 
	//virtual string GetVRBlendMode() = 0;
	/// 日志级别
	virtual int GetLogLevel() = 0;
	/// 图像控件的像素变换方法，返回值ImageTranformOpenCV, StoredValueTranform
	virtual string GetTransformMethod() = 0;

};

class ConfigurationManager
{
public:
	~ConfigurationManager() { instance_ = nullptr; }

	static ConfigurationManager* Get() 
	{
		if (instance_ == nullptr)
			instance_ = new ConfigurationManager;
		return instance_;
	}

	void SetConfigReader(IConfigReader *reader);

	string GetVRRenderMethod()
	{
		return vr_render_method_;
	}

	string GetMPRRenderMethod()
	{
		return mpr_render_method_;
	}

	int GetNumberOfThreads()
	{
		return threads_number_;
	}

	string GetVRBlendMode()
	{
		return vr_blend_mode_;
	}

	int GetLogLevel()
	{
		return log_level_;
	}

	string GetTransformMethod()
	{
		return control_transform_;
	}

	string GetImageSourceMethod()
	{
		return "BatchImageSource";
	}

private:
	ConfigurationManager();

private:
	static ConfigurationManager* instance_;
	IConfigReader *config_reader_;
	string vr_render_method_;
	string mpr_render_method_;
	int threads_number_;
	string vr_blend_mode_;
	int log_level_;
	string control_transform_;

};