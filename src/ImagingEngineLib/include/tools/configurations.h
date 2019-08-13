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
	/// VR��Ⱦ����������ֵSMART_RAYCASTING, RAYCASTING_GPU ...
	virtual string GetVRRenderMethod() = 0;
	/// MPR��Ⱦ����������ֵMPRRenderer, MPRRendererVtk
	virtual string GetMPRRenderMethod() = 0;
	/// ����������̵߳���Ŀ��������������Զ�ȡCPU�߼�������
	virtual int GetNumberOfThreads() = 0;
	///// VRͼ��Ļ���ģʽ 
	//virtual string GetVRBlendMode() = 0;
	/// ��־����
	virtual int GetLogLevel() = 0;
	/// ͼ��ؼ������ر任����������ֵImageTranformOpenCV, StoredValueTranform
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