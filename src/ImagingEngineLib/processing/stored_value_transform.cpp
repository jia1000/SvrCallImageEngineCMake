/*=========================================================================

  Program:   ImagingEngine
  Module:    stored_value_transform.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "processing/stored_value_transform.h"
#include "tools/math.h"
#include "bitmap/bitmap.h"

using namespace DW::CV;

StoredValueTranform::StoredValueTranform()
{
	DWBitmap *dwbitmap = new DWBitmap();
	dwbitmap->SetPixelType(2);
	image_buffer_ = dwbitmap;
}

StoredValueTranform::~StoredValueTranform()
{

}

void StoredValueTranform::SetInput(ShowBuffer *buffer)
{
	show_buffer_ = buffer;
}

IBitmap *StoredValueTranform::GetOutput()
{
	return image_buffer_;
}

void StoredValueTranform::Update()
{
	if (NULL == show_buffer_)
		return;

	int image_width = show_buffer_->GetWidth();
	int image_height = show_buffer_->GetHeight();
	int bytes_per_pixel = show_buffer_->GetNumberOfComponent();
	char *buffer = show_buffer_->GetShowBuffer();
	int num_of_comp = show_buffer_->GetNumberOfComponent();

	short *short_buf = reinterpret_cast<short *>(buffer);
	int size = image_width * image_height;
	int i;
	if (num_of_comp == 2){
		//TODO 不加偏移量，存储有符号值
		//for(i=0; i<size; ++i){

		//	//if (short_buf[i] < -1024){
		//	//	short_buf[i] = 0;
		//	//}
		//	//else{
		//	//	short_buf[i] += 1024;
		//	//}

		//	short_buf[i] += 1024;
		//	if (short_buf[i] < 0)
		//		short_buf[i] = 0;
		//}
	}

	image_buffer_->SetBuffer(buffer, 
		show_buffer_->GetWidth(), 
		show_buffer_->GetHeight(),
		bytes_per_pixel);
}