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

			/// ����ͼ��
			void SetInput(ShowBuffer *buffer) override;
			/// ���ر任���ͼ��
			IBitmap *GetOutput() override;
			/// ִ�б任
			void Update() override;
			/// ���÷���ϵ��
			void SetZoom(float scale) override {}
			/// ������ת�Ƕ�
			void SetRoll(float angle) override {}
			/// ����ƽ��ƫ����
			void SetPan(float dx, float dy) override {}
			/// ���ô���λ
			void SetWWWL(int ww, int wl) override {}
			/// �������ͼ��ĳߴ�
			void SetOutputSize(int width, int height) override {}

		private:
			/// �����ShowBuffer����
			ShowBuffer *show_buffer_;
			/// �����BITMAP����
			IBitmap *image_buffer_;

		};

	}
}