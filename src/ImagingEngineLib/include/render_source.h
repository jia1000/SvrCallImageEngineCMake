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

		/// 创建IThreedImaging对象
		IThreedImaging *CreateRenderControl(string control_id, RenderControlType type);
		/// 
		vector<IThreedImaging*> GetRenderControls() {return imaging_list_;}
		/// 根据编号(control_id)获取IImageControl对象
		IImageControl *GetImageControl(string control_id);
		/// 创建TwodImageControl对象，用于缓存生成的Batch图像
		IImageControl *CreateTwodImageControl(string control_id, RenderControlType type);
		/// 创建ThreedImageControl用于实时绘制图像
		IImageControl *CreateThreedImageControl(string control_id, RenderControlType type);
		/// 清空ImageControl的图像数据
		void ClearDataForAllControl();

	private:
		RenderSource() {}
		/// Create a renderer
		IThreedRenderer *CreateRenderer(RenderControlType type);
		///// 根据编号(control_id)获取IThreedImaging对象
		//IThreedImaging *GetRenderControl(string control_id);
		/// Create a IImageControl
		IImageControl *CreateImageControl(string control_id, ImageControlType type);

	private:
		static RenderSource *instance_;
		///TODO 不再需要
		vector<IThreedImaging *> imaging_list_;
		vector<IImageControl *> image_control_list_;
	};
	
}