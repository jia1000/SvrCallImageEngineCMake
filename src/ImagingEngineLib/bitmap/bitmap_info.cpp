/*=========================================================================

  Program:   ImagingEngine
  Module:    bitmap_info.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "bitmap/bitmap_info.h"

using namespace DW::Bitmap;

BitmapDcmInfo::BitmapDcmInfo(BufferResult *result)
{
	buffer_result_ = result;
}

BitmapDcmInfo::~BitmapDcmInfo()
{

}

float BitmapDcmInfo::GetStepValue()
{
	return buffer_result_->step;
}

void BitmapDcmInfo::GetPixelSpacings(double spacings[2])
{
	spacings[0] = buffer_result_->image_plane->GetSpacing().X();
	spacings[1] = buffer_result_->image_plane->GetSpacing().Y();
}

int BitmapDcmInfo::GetInstanceNumber()
{
	return buffer_result_->instance_number;
}

float BitmapDcmInfo::GetThickness()
{
	return buffer_result_->image_plane->GetThickness();
}

void BitmapDcmInfo::GetOrigin(double origin[3])
{
	Point3d pnt = buffer_result_->image_plane->GetOrigin();
	origin[0] = pnt.x;
	origin[1] = pnt.y;
	origin[2] = pnt.z;
}

void BitmapDcmInfo::GetOrientation(double row_vector[3], double column_vector[3])
{
	ImageOrientation ori = buffer_result_->image_plane->GetImageOrientation();
	Vector3d row_v = ori.GetRowVector();
	row_vector[0] = row_v[0];
	row_vector[1] = row_v[1];
	row_vector[2] = row_v[2];
	Vector3d col_v = ori.GetColumnVector();
	column_vector[0] = col_v[0];
	column_vector[1] = col_v[1];
	column_vector[2] = col_v[2];
}

void BitmapDcmInfo::GetWindowLevel(int &ww, int &wl)
{
	ww = buffer_result_->window_width;
	wl = buffer_result_->window_level;
}

float BitmapDcmInfo::GetSpacingBetweenSlices()
{
	return buffer_result_->step;
}