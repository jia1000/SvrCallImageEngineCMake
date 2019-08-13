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
		// ͼ�����/��ת/ƽ��/����λ�任
		///
		class IImageTransform
		{
		public:
			/// ����ͼ��
			virtual void SetInput(ShowBuffer *buffer) = 0;
			/// ���ر任���ͼ��
			virtual IBitmap *GetOutput() = 0;
			/// ִ�б任
			virtual void Update() = 0;
			/// ���÷���ϵ��
			virtual void SetZoom(float scale) = 0;
			/// ������ת�Ƕ�
			virtual void SetRoll(float angle) = 0;
			/// ����ƽ��ƫ����
			virtual void SetPan(float dx, float dy) = 0;
			/// ���ô���λ
			virtual void SetWWWL(int ww, int wl) = 0;
			/// �������ͼ��ĳߴ�
			virtual void SetOutputSize(int width, int height) = 0;

		};

	}
}
