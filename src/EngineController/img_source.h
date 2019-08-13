/*=========================================================================

  Program:   ImagingEngine
  Module:    img_source.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "io/img_source.h"
#include "bitmap/ibitmap.h"

using namespace GIL::DICOM;
using namespace DW::Bitmap;

class PixelDataSource : public IGeneratePixelData
{
public:
	PixelDataSource(IBitmap *bmp)
		: bitmap_(bmp)
	{

	}
	~PixelDataSource()
	{

	}

	unsigned int GetWidth() override
	{
		return bitmap_->GetWidth();
	}

	unsigned int GetHeight() override
	{
		return bitmap_->GetHeight();
	}

	unsigned int GetSamplesPerPixel() override
	{
		if (bitmap_->GetNumberOfComponent() >= 3)
			return 3;
		else
			return 1;
	}

	char *GetPixelData() override
	{
		return (char *)bitmap_->GetBuffer();
	}

	const char *GetPhotometricInterpretation() override
	{
		return bitmap_->GetPhotometricInterpretation();
	}

	unsigned short GetBitsAllocated() override
	{
		return bitmap_->GetBitsAllocated();
	}

	unsigned short GetBitsStored() override
	{
		return bitmap_->GetBitsStored();
	}

	unsigned short GetHighBit() override
	{
		return bitmap_->GetHighBit();
	}

	unsigned short GetPixelRepresentation() override
	{
		return bitmap_->GetPixelRepresentation();
	}

	unsigned short GetPlanarConfiguration() override
	{
		return bitmap_->GetPlanarConfiguration();
	}

	unsigned long GetPixelLength() override
	{
		return bitmap_->GetWidth() * bitmap_->GetHeight() * bitmap_->GetNumberOfComponent();
	}

	const char *GetRescaleIntercept() override
	{
		return bitmap_->GetRescaleIntercept();
	}
	const char *GetRescaleSlope() override
	{
		return bitmap_->GetRescaleSlope();
	}
	const char *GetRescaleType() override
	{
		return bitmap_->GetRescaleType();
	}

private:
	IBitmap *bitmap_;

};