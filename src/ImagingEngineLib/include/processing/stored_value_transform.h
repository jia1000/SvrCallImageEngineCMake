/*=========================================================================

  Program:   ImagingEngine
  Module:    stored_value_transform.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/show_buffer.h"
#include "processing/image_transform.h"
#include "bitmap/ibitmap.h"

using namespace DW::IMAGE;
using namespace DW::Bitmap;

namespace DW {
	namespace CV {

		///
		// Converts from ShowBuffer object to IBitmap object
		///
		class StoredValueTranform : public IImageTransform
		{
		public:
			StoredValueTranform();
			~StoredValueTranform();

			/// 输入图像
			void SetInput(ShowBuffer *buffer) override;
			/// 返回变换后的图像
			IBitmap *GetOutput() override;
			/// 执行变换
			void Update() override;
			/// 设置放缩系数
			void SetZoom(float scale) override {}
			/// 设置旋转角度
			void SetRoll(float angle) override {}
			/// 设置平移偏移量
			void SetPan(float dx, float dy) override {}
			/// 设置窗宽窗位
			void SetWWWL(int ww, int wl) override {}
			/// 设置输出图像的尺寸
			void SetOutputSize(int width, int height) override {}

		private:
			/// 输入的ShowBuffer对象
			ShowBuffer *show_buffer_;
			/// 输出的BITMAP对象
			IBitmap *image_buffer_;

		};

	}
}