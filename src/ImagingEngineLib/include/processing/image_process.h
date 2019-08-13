/*=========================================================================

  Program:   ImagingEngine
  Module:    image_process.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

// OpenCV在linux没有编译通过 [5/24/2019 Modified by zhangjian]
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
			///// opencv Mat和Buffer的互转
			//void Mat2Buffer(const cv::Mat &img, UNITDATASHOW *output);
			//void Buffer2Mat(const UNITDATASHOW *input, int width, int height, int bits, cv::Mat &img);
			///// 
			//BOOL HBitmapToMat(IBitmap *_hBmp, cv::Mat& _mat);
			//BOOL MatToHBitmap(cv::Mat& _mat, IBitmap *_hBmp);

			///// hbitmap convert to IplImage   
			//IplImage *HBitmapToIplImage(HBITMAP hBmp);

		protected:
			/// 图像原始数据
			IBitmap *in_image_data_;
			int image_width_;
			int image_height_;
			int bits_per_pixel_;
		};

		//////////////////////////////////////////////////////////////////////////
		/// 放缩图像
		class ImageZoomProcess : public ImageProcessBase
		{
		public:
			ImageZoomProcess(IBitmap *in_image_data, float scale);
			~ImageZoomProcess();

			/// 图像缩放后数据
			bool Excute(IBitmap *out_image_data) override; 

		private:
			/// 缩放倍率
			float scale_;
		};

		//////////////////////////////////////////////////////////////////////////
		/// 选择图像
		class ImageRotateProcess : public ImageProcessBase
		{
		public:
			ImageRotateProcess(IBitmap *in_image_data, float angle);
			~ImageRotateProcess();

			bool Excute(IBitmap *out_image_data) override;

		private:
			/// 旋转角度
			float angle_;
		};

		//////////////////////////////////////////////////////////////////////////
		/// 平移图像
		class ImageMoveProcess : public ImageProcessBase
		{
		public:
			ImageMoveProcess(IBitmap *in_image_data, float dx, float dy);
			~ImageMoveProcess();

			bool Excute(IBitmap *out_image_data) override;

		private:
			/// 平移偏移量x/y
			float offset_x_;
			float offset_y_;
		};
		
	}
}