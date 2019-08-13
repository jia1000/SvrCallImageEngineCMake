/*=========================================================================

  Program:   ImagingEngine
  Module:    image_control.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "render/renderer_3d.h"
#include "render/imaging_3d.h"
#include "processing/image_transform.h"
#include "data/show_buffer.h"
#include "data/image_source.h"
#include "bitmap/ibitmap.h"
#include "bitmap/ibitmap_info.h"

using namespace DW::IMAGE;
using namespace DW::Render;
using namespace DW::CV;
using namespace DW::Bitmap;

namespace DW {
	namespace Control {

		enum ImageControlType
		{
			CONTROL_TWOD,
			CONTROL_THREED,
		};

		///
		// 图像控件基类，存储当前图像/序列Batch图像
		///
		class IImageControl
		{
		public:
			/// 获取控件类型
			virtual ImageControlType GetType() = 0;
			/// 获取控件唯一编号
			virtual string GetControlID() = 0;
			/// 设置图像数据源
			virtual void SetInput(int, void *) = 0;
			/// 获取图像数据源
			virtual void *GetInput(int) = 0;
			/// 设置图像变换执行类
			virtual void SetTransform(IImageTransform *) = 0;
			/// 应用图像变换
			virtual void Update() = 0;
			/// 可获取生成图像的总数
			virtual int GetOutputNumber() = 0;
			/// 获取图像句柄
			virtual IBitmap *GetOutput() = 0;
			virtual IBitmap *GetOutput(int) = 0;
			/// 获取图像信息
			virtual IBitmapDcmInfo *GetOutputInfo() = 0;
			virtual IBitmapDcmInfo *GetOutputInfo(int) = 0;
			/// 设置显示图像的尺寸
			virtual void SetDisplaySize(int width, int height) = 0;
			/// 翻页
			virtual void GoTo(int pos) = 0;
			/// 放缩
			virtual void Zoom(float scale) = 0;
			/// 旋转
			virtual void Roll(float angle) = 0;
			/// 平移
			virtual void Pan(float dx, float dy) = 0;
			/// 窗宽窗位
			virtual void WWWL(int ww, int wl) = 0;
			/// 放缩到指定大小
			virtual void ZoomToSize(int width, int height) = 0;
			/// Move the camera according to the motion vector we spend
			/// @param motionVector[] motion Vector that determines where and how much camera moves
			virtual void Move(float motion_vector[3]) = 0;
			/// 绕着指定中心轴旋转一定角度
			virtual void Rotate3D(Vector3f axis, float angle) = 0;
			/// 将当前数据源图像存储为bitmap
			virtual void SaveAsBitmap(string output_dir) = 0;
			/// 将当前数据源图像存储为dicom图像
			virtual void SaveAsDicom(string output_dir) = 0;
			/// 清空数据
			virtual void Clear() = 0;
			
		};

	}
}