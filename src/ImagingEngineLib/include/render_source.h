/*=========================================================================

  Program:   ImagingEngine
  Module:    imaging_source.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/vol_data.h"
#include "actor/actor_3d.h"
#include "data/render_mode.h"
#include "render/renderer_3d.h"
#include "control/image_control.h"

using namespace DW::Control;

namespace DW {

	class RenderSource
	{
	public:
		~RenderSource() { instance_ = nullptr; }
	
		static RenderSource *Get();

		/// ����IThreedImaging����
		IThreedImaging *CreateRenderControl(string control_id, RenderControlType type);
		/// 
		vector<IThreedImaging*> GetRenderControls() {return imaging_list_;}
		/// ���ݱ��(control_id)��ȡIImageControl����
		IImageControl *GetImageControl(string control_id);
		/// ����TwodImageControl�������ڻ������ɵ�Batchͼ��
		IImageControl *CreateTwodImageControl(string control_id, RenderControlType type);
		/// ����ThreedImageControl����ʵʱ����ͼ��
		IImageControl *CreateThreedImageControl(string control_id, RenderControlType type);
		/// ���ImageControl��ͼ������
		void ClearDataForAllControl();

	private:
		RenderSource() {}
		/// Create a renderer
		IThreedRenderer *CreateRenderer(RenderControlType type);
		///// ���ݱ��(control_id)��ȡIThreedImaging����
		//IThreedImaging *GetRenderControl(string control_id);
		/// Create a IImageControl
		IImageControl *CreateImageControl(string control_id, ImageControlType type);

	private:
		static RenderSource *instance_;
		///TODO ������Ҫ
		vector<IThreedImaging *> imaging_list_;
		vector<IImageControl *> image_control_list_;
	};
	
}