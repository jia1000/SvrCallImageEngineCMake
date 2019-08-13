/*=========================================================================

  Program:   ImagingEngine
  Module:    image_process.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

// OpenCV��linuxû�б���ͨ�� [5/24/2019 Modified by zhangjian]
//#include "opencv2/core/core.hpp"    
//#include "opencv2/highgui/highgui.hpp"  
//#include "opencv2/imgproc/imgproc.hpp" 
//#include "opencv2/opencv.hpp"
#include "data/data_definition.h"
#include "bitmap/ibitmap.h"

using namespace DW::Bitmap;

namespace DW {
	namespace CV {
		
		class ImageProcessBase
		{
		public:
			ImageProcessBase(IBitmap *in_image_data);
			~ImageProcessBase();

			virtual bool Excute(IBitmap *out_image_data) = 0;

		protected:	
			///// opencv Mat��Buffer�Ļ�ת
			//void Mat2Buffer(const cv::Mat &img, UNITDATASHOW *output);
			//void Buffer2Mat(const UNITDATASHOW *input, int width, int height, int bits, cv::Mat &img);
			///// 
			//BOOL HBitmapToMat(IBitmap *_hBmp, cv::Mat& _mat);
			//BOOL MatToHBitmap(cv::Mat& _mat, IBitmap *_hBmp);

			///// hbitmap convert to IplImage   
			//IplImage *HBitmapToIplImage(HBITMAP hBmp);

		protected:
			/// ͼ��ԭʼ����
			IBitmap *in_image_data_;
			int image_width_;
			int image_height_;
			int bits_per_pixel_;
		};

		//////////////////////////////////////////////////////////////////////////
		/// ����ͼ��
		class ImageZoomProcess : public ImageProcessBase
		{
		public:
			ImageZoomProcess(IBitmap *in_image_data, float scale);
			~ImageZoomProcess();

			/// ͼ�����ź�����
			bool Excute(IBitmap *out_image_data) override; 

		private:
			/// ���ű���
			float scale_;
		};

		//////////////////////////////////////////////////////////////////////////
		/// ѡ��ͼ��
		class ImageRotateProcess : public ImageProcessBase
		{
		public:
			ImageRotateProcess(IBitmap *in_image_data, float angle);
			~ImageRotateProcess();

			bool Excute(IBitmap *out_image_data) override;

		private:
			/// ��ת�Ƕ�
			float angle_;
		};

		//////////////////////////////////////////////////////////////////////////
		/// ƽ��ͼ��
		class ImageMoveProcess : public ImageProcessBase
		{
		public:
			ImageMoveProcess(IBitmap *in_image_data, float dx, float dy);
			~ImageMoveProcess();

			bool Excute(IBitmap *out_image_data) override;

		private:
			/// ƽ��ƫ����x/y
			float offset_x_;
			float offset_y_;
		};
		
	}
}