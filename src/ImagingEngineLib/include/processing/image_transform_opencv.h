/*=========================================================================

  Program:   ImagingEngine
  Module:    image_transform_opencv.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/show_buffer.h"
#include "data/window_level.h"
#include "processing/voi_lut.h"
#include "processing/image_transform.h"
#include "bitmap/ibitmap.h"

using namespace DW::IMAGE;
using namespace DW::Bitmap;

namespace DW {
	namespace CV {

		class ImageTranformOpenCV : public IImageTransform
		{
		public:
			ImageTranformOpenCV();
			~ImageTranformOpenCV();

			/// ����ͼ��
			void SetInput(ShowBuffer *buffer) override;
			/// ���ر任���ͼ��
			IBitmap *GetOutput() override;
			/// ִ�б任
			void Update() override;
			/// ���÷���ϵ��
			void SetZoom(float scale) override;
			/// ������ת�Ƕ�
			void SetRoll(float angle) override;
			/// ����ƽ��ƫ����
			void SetPan(float dx, float dy) override;
			/// ���ô���λ
			void SetWWWL(int ww, int wl) override;
			/// �������ͼ��ĳߴ�
			void SetOutputSize(int width, int height) override;

		private:
			/// �����ShowBuffer����
			ShowBuffer *show_buffer_;
			/// �����BITMAP����
			IBitmap *image_buffer_;
			/// ��������
			float scale_factor_;
			/// ƽ������ת�Ƕ�
			float roll_angle_;
			/// ˮƽƫ����
			float offset_x_;
			/// ��ֱƫ����
			float offset_y_;
			/// ����λ
			WindowLevel window_level_;
			/// VOI lut table
			VoiLut *lut_;
			/// ���ͼ��Ŀ��
			int output_width_;
			int output_height_;

		};

	}
}
