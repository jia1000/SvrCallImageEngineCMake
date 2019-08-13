/*=========================================================================

  Program:   ImagingEngine
  Module:    image_transform_opencv.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "processing/image_transform_opencv.h"
#include "tools/math.h"
#include "processing/image_process.h"
#include "bitmap/bitmap.h"

using namespace DW::CV;


ImageTranformOpenCV::ImageTranformOpenCV()
{
	WindowLevel level(2000, 400, "default");
	lut_ = new VoiLut(level);
	scale_factor_ = 1.0f;
	roll_angle_ = 0.0f;
	offset_x_ = 0.0f;
	offset_y_ = 0.0f;
	output_width_ = 512;
	output_height_ = 512;
	image_buffer_ = new DWBitmap();
	dynamic_cast<DWBitmap *>(image_buffer_)->SetPixelType(0);
}

ImageTranformOpenCV::~ImageTranformOpenCV()
{
	if (lut_){
		delete lut_;
		lut_ = NULL;
	}
}

void ImageTranformOpenCV::SetInput(ShowBuffer *buffer)
{
	show_buffer_ = buffer;
}

IBitmap *ImageTranformOpenCV::GetOutput()
{
	return image_buffer_;
}

void ImageTranformOpenCV::SetOutputSize(int width, int height)
{
	output_width_ = width;
	output_height_ = height;
}

void ImageTranformOpenCV::Update()
{
	if (NULL == show_buffer_)
		return;

	int image_width = show_buffer_->GetWidth();
	int image_height = show_buffer_->GetHeight();
	int bits_per_pixel = show_buffer_->GetBitsPerPixel();
	short *buffer = reinterpret_cast<short *>(show_buffer_->GetShowBuffer());
	int num_of_comp = show_buffer_->GetNumberOfComponent();

	if (bits_per_pixel == 16){

		// Apply window width & level
		unsigned int *output = new unsigned int [image_width * image_height];
		unsigned int *lut_table = lut_->GetLutTable();
		if (lut_table){
			int count = image_width * image_height;
			for (int i = 0; i < count; ++i){

				//TODO 1024的偏移量在哪设置？
				short ctv = buffer[i] + 1024;

				//TODO 支持的最大CT值在哪设置？
				if (ctv < 0)
					ctv = 0;
				if (ctv > MAXIMIUM_CT_VALUE)
					ctv = MAXIMIUM_CT_VALUE - 1;

				output[i] = lut_table[ctv];

				//output[i * 4] = lut_table[ctv][0];
				//output[i * 4 + 1] = lut_table[ctv][1];
				//output[i * 4 + 2] = lut_table[ctv][2];
				//output[i * 4 + 3] = lut_table[ctv][3];
			}
		}

		image_buffer_->SetBuffer(reinterpret_cast<char *>(output), 
			show_buffer_->GetWidth(), 
			show_buffer_->GetHeight(),
			4);

		delete [] output;
		output = NULL;
	}
	else if (bits_per_pixel == 32){
		image_buffer_->SetBuffer(reinterpret_cast<char *>(buffer), 
			show_buffer_->GetWidth(), 
			show_buffer_->GetHeight(),
			4);
	}
	
	// TODO  [5/24/2019 Modified by zhangjian]
	//// Apply scale factor
	//if (fabs(scale_factor_ - 1.0f) > MathTool::kEpsilon){

	//	HBITMAP hbitmap;
	//	// Use opencv to zoom image
	//	ImageZoomProcess processor(image_buffer_, 
	//		output_width_, 
	//		output_height_, 
	//		bits_per_pixel,
	//		scale_factor_);

	//	processor.Excute(hbitmap);

	//	DeleteObject (image_buffer_);
	//	// Set it to be the transformed HBITMAP
	//	image_buffer_ = hbitmap;

	//	//BITMAP bitmap;
	//	//GetObject (image_buffer_, sizeof (BITMAP), &bitmap);
	//	//RECT area;
	//	//area.left = (bitmap.bmWidth - output_width_) / 2;
	//	//area.top = (bitmap.bmHeight - output_height_) / 2;
	//	//area.right = area.left + output_width_;
	//	//area.bottom = area.top + output_height_;
	//	//BitmapWriter::Crop(image_buffer_, area);
	//}

	//// Apply roll
	//if (fabs(roll_angle_) > MathTool::kEpsilon){

	//	HBITMAP hbitmap;
	//	// User opencv to rotate image in plane
	//	ImageRotateProcess processor(image_buffer_, 
	//		output_width_, 
	//		output_height_, 
	//		bits_per_pixel,
	//		roll_angle_);

	//	processor.Excute(hbitmap);

	//	DeleteObject (image_buffer_);
	//	// Set it to be the transformed HBITMAP
	//	image_buffer_ = hbitmap;
	//}

	//// Apply pan
	//if (fabs(offset_x_) > MathTool::kEpsilon ||
	//	fabs(offset_y_) > MathTool::kEpsilon){

	//		HBITMAP hbitmap;
	//		// User opencv to move image in plane
	//		ImageMoveProcess processor(image_buffer_, 
	//			output_width_, 
	//			output_height_, 
	//			bits_per_pixel,
	//			offset_x_,
	//			offset_y_);

	//		processor.Excute(hbitmap);

	//		DeleteObject (image_buffer_);
	//		// Set it to be the transformed HBITMAP
	//		image_buffer_ = hbitmap;
	//}

}

void ImageTranformOpenCV::SetZoom(float scale)
{
	scale_factor_ = scale;
}

void ImageTranformOpenCV::SetRoll(float angle)
{
	roll_angle_ = angle;
}

void ImageTranformOpenCV::SetPan(float dx, float dy)
{
	offset_x_ = dx;
	offset_y_ = dy;
}

void ImageTranformOpenCV::SetWWWL(int ww, int wl)
{
	window_level_.SetWindowWidth(ww);
	window_level_.SetWindowLevel(wl);
	lut_->SetWindowLevel(window_level_);
}