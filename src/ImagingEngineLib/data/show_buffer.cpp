/*=========================================================================

  Program:   ImagingEngine
  Module:    show_buffer.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "data/show_buffer.h"
#include "exception/data_exception.h"

using namespace DW::IMAGE;
using namespace DW::Exception;

ShowBuffer::ShowBuffer()
{
	buffer_data_ = NULL;
}

ShowBuffer::~ShowBuffer()
{
	if (buffer_data_){
		delete [] buffer_data_;
		buffer_data_ = NULL;
	}
}

void ShowBuffer::InitBufferData(int width, int height, BYTE bits_per_pixel)
{
	if (buffer_width_ == width && buffer_height_ == height && bits_per_pixel_ == bits_per_pixel)
		return;

	buffer_width_ = width;
	buffer_height_ = height;
	bits_per_pixel_ = bits_per_pixel;

	if (buffer_data_){
		delete[] buffer_data_;
	}
	try
	{
		buffer_data_ = new UNITDATASHOW[width * height * bits_per_pixel / 8];
	}
	catch(std::bad_alloc &e){
		CGLogger::Error("bad_alloc error occurred in InitBufferData.");
		throw DataException(RET_STATUS_ENGINE_EXCEPTION_OUT_OF_MEMORY, e.what());
	}
}

void ShowBuffer::SetBufferData(UNITDATASHOW* bytes, int width, int height, BYTE bits_per_pixel)
{
	int old_width = buffer_width_;
	int old_height = buffer_height_;
	int old_components = bits_per_pixel_ / 8;

	buffer_width_ = width;
	buffer_height_ = height;
	bits_per_pixel_ = bits_per_pixel;

	int number_of_components = bits_per_pixel_ / 8;
	if (buffer_data_){
		delete[] buffer_data_;
	}

	try
	{
		buffer_data_ = new UNITDATASHOW[width * height * number_of_components];
	}
	catch(std::bad_alloc &e){
		CGLogger::Error("bad_alloc error occurred in SetBufferData.");
		throw DataException(RET_STATUS_ENGINE_EXCEPTION_OUT_OF_MEMORY, e.what());
	}

	memcpy(reinterpret_cast<UNITDATASHOW *>(buffer_data_), bytes, width * height * number_of_components);
}

UNITDATASHOW* ShowBuffer::GetShowBuffer()
{
	return buffer_data_;
}

void ShowBuffer::ShallowCopy(ShowBuffer *from)
{
	this->buffer_width_ = from->buffer_width_;
	this->buffer_height_ = from->buffer_height_;
	this->bits_per_pixel_ = from->bits_per_pixel_;
	this->buffer_data_ = from->buffer_data_;
}

void ShowBuffer::DeepCopy(ShowBuffer *from)
{
	this->buffer_width_ = from->buffer_width_;
	this->buffer_height_ = from->buffer_height_;
	this->bits_per_pixel_ = from->bits_per_pixel_;
	this->x_dpi_ = from->x_dpi_;
	this->y_dpi_ = from->y_dpi_;
	int num_of_component = this->bits_per_pixel_ / 8;
	if (buffer_data_){
		delete[] buffer_data_;
	}

	try
	{
		this->buffer_data_ = new UNITDATASHOW[this->buffer_width_ * this->buffer_height_ * num_of_component];
		memcpy(reinterpret_cast<UNITDATASHOW *>(this->buffer_data_), 
			from->buffer_data_, 
			this->buffer_width_ * this->buffer_height_ * num_of_component); 
	}
	catch(std::bad_alloc &e){
		CGLogger::Error("bad_alloc error occurred in SetBufferData.");
		throw DataException(RET_STATUS_ENGINE_EXCEPTION_OUT_OF_MEMORY, e.what());
	}
}

void ShowBuffer::DumpBufferToFile(const char* file_path)
{
	//// 输出正确的bmp图像
	//BitmapWriter::SaveAsBMP(file_path, buffer_data_, buffer_width_, buffer_height_); 
}