/*=========================================================================

  Program:   ImagingEngine
  Module:    image_control.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "control/image_control_2d.h"
#include "orientation/orientation_interpreter.h"
#include "bitmap/bitmap_info.h"

using namespace DW::Control;
using namespace DW::Orientation;


TwodImageControl::TwodImageControl(string control_id)
	: control_id_(control_id)
{
	scale_factor_ = 1.0f;
	roll_angle_ = 0.0f;
	offset_x_ = 0.0f;
	offset_y_ = 0.0f;
	ww_ = 2000;
	wl_ = 400;
	port_ = 2;
	display_width_ = 512;
	display_height_ = 512;
	display_index_ = -1;
	image_source_ = NULL;
	imaging_ = NULL;
}

TwodImageControl::~TwodImageControl()
{
	if (image_source_){
		delete image_source_;
		image_source_ = NULL;
	}
	if (imaging_){
		delete imaging_;
		imaging_ = NULL;
	}
}

string TwodImageControl::GetControlID()
{
	return control_id_;
}

void TwodImageControl::SetInput(int port, void *source)
{
	if (port < 0)
		port = 0;
	if (port > port_)
		port = port_;

	if (port == 0){
		if (image_source_){
			delete image_source_;
		}
		image_source_ = reinterpret_cast<IImageSource *>(source);
	}
	else{
		if (imaging_){
			delete imaging_;
		}
		imaging_ = reinterpret_cast<IThreedImaging *>(source);
	}
}

void *TwodImageControl::GetInput(int port)
{
	if (port < 0)
		port = 0;
	if (port > port_)
		port = port_;

	if (port == 0){
		return image_source_;
	}
	else{
		return imaging_;
	}
}

void TwodImageControl::SetTransform(IImageTransform *transform)
{
	transform_ = transform;
}

void TwodImageControl::Update()
{
	transform_->Update();
}

int TwodImageControl::GetOutputNumber()
{
	if (image_source_){
		return image_source_->GetImageNumber();
	}
	else{
		return 0;
	}
}

IBitmap *TwodImageControl::GetOutput()
{
	return transform_->GetOutput();;
}

IBitmap *TwodImageControl::GetOutput(int index)
{
	if (index < 0 || image_source_ == NULL)
		return NULL;
	int count = image_source_->GetImageNumber();
	if (index >= count)
		return NULL;

	BufferResult *result = image_source_->GetImage(index);
	transform_->SetInput(result->buffer_data);
	transform_->Update();

	return transform_->GetOutput()->Clone();
}

IBitmapDcmInfo *TwodImageControl::GetOutputInfo()
{
	return GetOutputInfo(display_index_);
}

IBitmapDcmInfo *TwodImageControl::GetOutputInfo(int index)
{
	if (index < 0 || image_source_ == NULL)
		return NULL;
	int count = image_source_->GetImageNumber();
	if (index >= count)
		return NULL;

	BufferResult *result = image_source_->GetImage(index);
	return new BitmapDcmInfo(result);
}

void TwodImageControl::SetDisplaySize(int width, int height)
{
	display_width_ = width;
	display_height_ = height;
}

void TwodImageControl::GoTo(int pos)
{
	display_index_ = pos;
	int number = image_source_->GetImageNumber();
	if (number <= display_index_)
		display_index_ = number - 1;
	else if (display_index_ < 0)
		display_index_ = 0;

	if (image_source_){
		transform_->SetInput(image_source_->GetImage(display_index_)->buffer_data);
	}	
}

void TwodImageControl::Zoom(float scale)
{
	//TODO 相乘
	scale_factor_ *= scale;
	transform_->SetZoom(scale_factor_);
}

void TwodImageControl::ZoomToSize(int width, int height)
{
	////TODO ZoomToSize
	//scale_factor_ *= scale;
	//transform_->SetZoom(scale_factor_);
}

void TwodImageControl::Roll(float angle)
{
	//TODO 相加
	roll_angle_ += angle;
	transform_->SetRoll(roll_angle_);
}

void TwodImageControl::Pan(float dx, float dy)
{
	//TODO 相加
	offset_x_ += dx;
	offset_y_ += dy;
	transform_->SetPan(offset_x_, offset_y_);
}

void TwodImageControl::WWWL(int ww, int wl)
{
	ww_ = ww;
	wl_ = wl;
	transform_->SetWWWL(ww_, wl_);
}

void TwodImageControl::OnImageAdded()
{
	if (display_index_ < 0){
		display_index_ = 0;
		if (image_source_){
			transform_->SetInput(image_source_->GetImage(display_index_)->buffer_data);
		}
	}
}

void TwodImageControl::SaveAsBitmap(string output_dir)
{
	if (image_source_ == NULL){
		return;
	}
	// 将当前数据源下所有图像都存储到磁盘路径
	int count = image_source_->GetImageNumber();
	IBitmap *bmp = NULL;
	BufferResult *result = NULL;
	int i;
	for(i=0; i<count; ++i){
		result = image_source_->GetImage(i);
		transform_->SetInput(result->buffer_data);
		transform_->Update();
		bmp = transform_->GetOutput();
		//TODO 临时性做法，因为目前写bitmap不支持16位
		if (bmp && bmp->GetNumberOfComponent() >= 3){
			bmp->Save(result->file_name.c_str());
			//TODO 同一份内存
			//delete bmp;
		}

		char left[4] = { '\0' };
		char right[4] = { '\0' };
		char up[4] = { '\0' };
		char down[4] = { '\0' };
		OrientationInterpreter::Interpret(result->image_plane->GetImageOrientation(), left, right, up, down);
	}
}

void TwodImageControl::SaveAsDicom(string output_dir)
{

}

void TwodImageControl::Clear()
{
	if (image_source_){
		image_source_->Destroy();
	}
}