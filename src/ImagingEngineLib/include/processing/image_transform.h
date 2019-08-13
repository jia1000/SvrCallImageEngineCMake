/*=========================================================================

  Program:   ImagingEngine
  Module:    image_processor.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/show_buffer.h"
#include "bitmap/ibitmap.h"

using namespace DW::IMAGE;
using namespace DW::Bitmap;

namespace DW {
	namespace CV {

		///
		// 图像放缩/旋转/平移/窗宽窗位变换
		///
		class IImageTransform
		{
		public:
			/// 输入图像
			virtual void SetInput(ShowBuffer *buffer) = 0;
			/// 返回变换后的图像
			virtual IBitmap *GetOutput() = 0;
			/// 执行变换
			virtual void Update() = 0;
			/// 设置放缩系数
			virtual void SetZoom(float scale) = 0;
			/// 设置旋转角度
			virtual void SetRoll(float angle) = 0;
			/// 设置平移偏移量
			virtual void SetPan(float dx, float dy) = 0;
			/// 设置窗宽窗位
			virtual void SetWWWL(int ww, int wl) = 0;
			/// 设置输出图像的尺寸
			virtual void SetOutputSize(int width, int height) = 0;

		};

	}
}
