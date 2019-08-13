/*=========================================================================

  Program:   ImagingEngine
  Module:    image_control_3d.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "control/image_control_3d.h"

using namespace DW::Control;

ThreedImageControl::ThreedImageControl(string control_id)
	: control_id_(control_id)
{
	port_ = 1;
	scale_factor_ = 1.0f;
	roll_angle_ = 0.0f;
	offset_x_ = 0.0f;
	offset_y_ = 0.0f;
	ww_ = 2000;
	wl_ = 400;
	display_width_ = 512;
	display_height_ = 512;
}

ThreedImageControl::~ThreedImageControl()
{

}

string ThreedImageControl::GetControlID()
{
	return control_id_;
}

void ThreedImageControl::SetInput(int port, void *imaging)
{
	// Only one port
	imaging_ = reinterpret_cast<IThreedImaging *>(imaging);
}

void *ThreedImageControl::GetInput(int port)
{
	// Only one port
	return imaging_;
}

void ThreedImageControl::SetTransform(IImageTransform *transform)
{
	transform_ = transform;
}

void ThreedImageControl::Update()
{
	if (imaging_){
		imaging_->Render();
		transform_->SetInput(imaging_->GetShowBuffer());
		transform_->Update();
	}
}

int ThreedImageControl::GetOutputNumber()
{
	return 1;
}

IBitmap *ThreedImageControl::GetOutput()
{
	return transform_->GetOutput();
}

IBitmapDcmInfo *ThreedImageControl::GetOutputInfo()
{
	//TODO
	return NULL;
}

void ThreedImageControl::SetDisplaySize(int width, int height)
{
	display_width_ = width;
	display_height_ = height;
}

void ThreedImageControl::GoTo(int pos)
{

}

void ThreedImageControl::Zoom(float scale)
{
	//TODO 相乘
	scale_factor_ *= scale;
	if (imaging_->CanZoom()){
		imaging_->Zoom(scale);
		//TODO 重新设置数据？
		transform_->SetInput(imaging_->GetShowBuffer());
	}
	else{
		//TODO 如果不需要重新绘制
		transform_->SetZoom(scale_factor_);
	}
}

void ThreedImageControl::Roll(float angle)
{
	//TODO 相加
	roll_angle_ += angle;
	if (imaging_->CanRotate()){
		//TODO 
	}
	else{
		//TODO 如果不需要重新绘制
		transform_->SetRoll(roll_angle_);
	}
}

void ThreedImageControl::Pan(float dx, float dy)
{
	//TODO 相加
	offset_x_ += dx;
	offset_y_ += dy;
	if (imaging_->CanMove()){
		//TODO 
	}
	else{
		//TODO 如果不需要重新绘制
		transform_->SetPan(offset_x_, offset_y_);
	}
}

void ThreedImageControl::WWWL(int ww, int wl)
{
	ww_ = ww;
	wl_ = wl;
	if (imaging_->CanWWWL()){
		//TODO 更改组织显示...
		imaging_->WindowWidthLevel(ww_, wl_);
	}
	else{
		//TODO 如果不需要重新绘制
		transform_->SetWWWL(ww_, wl_);
	}
}

void ThreedImageControl::ZoomToSize(int width, int height)
{

}

void ThreedImageControl::Move(float motionVector[3])
{

}

void ThreedImageControl::Rotate3D(Vector3f axis, float angle)
{
	imaging_->Rotate3D(axis, angle);
	//TODO 重新设置数据？
	transform_->SetInput(imaging_->GetShowBuffer());
	transform_->Update();
}

void ThreedImageControl::SaveAsBitmap(string output_dir)
{

}

void ThreedImageControl::SaveAsDicom(string output_dir)
{

}

void ThreedImageControl::Clear()
{

}