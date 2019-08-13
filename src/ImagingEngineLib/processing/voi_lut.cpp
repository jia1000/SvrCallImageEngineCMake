/*=========================================================================

  Program:   ImagingEngine
  Module:    voi_lut.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "processing/voi_lut.h"
#include <math.h>
#include "tools/math.h"

using namespace DW::CV;

#define RGBA_TO_LONG(r,g,b,a) ((unsigned int)(((unsigned char)(r)|((unsigned int)((unsigned char)(g))<<8))|(((unsigned int)(unsigned char)(b))<<16)|(((unsigned int)(unsigned char)(a))<<24)))

inline int ConvertCTVToIndex(int ctv);

VoiLut::VoiLut()
	: kMaxCTV(MAXIMIUM_CT_VALUE)
{
	lut_table_ = new unsigned int [kMaxCTV];
	//for(int i=0; i<kMaxCTV; ++i){
	//	lut_table_[i] = new unsigned char [4];
	//}
}

VoiLut::VoiLut(const WindowLevel &window_level)
	: kMaxCTV(MAXIMIUM_CT_VALUE)
{
	lut_table_ = new unsigned int [kMaxCTV];
	//for(int i=0; i<kMaxCTV; ++i){
	//	lut_table_[i] = new unsigned char [4];
	//}
	window_level_ = window_level;
	CreateLUT();
}

VoiLut::~VoiLut()
{
	if (lut_table_){
		delete[] lut_table_;
		lut_table_ = NULL;
	}
}

void VoiLut::SetWindowLevel(const WindowLevel &window_level)
{
	if (window_level_ == window_level)
		return;

	window_level_ = window_level;
	CreateLUT();
}

//
//void VoiLut::CreateLUT()
//{
//	float scalevalue = 1.0f;
//	int width = window_level_.GetWindowWidth();
//	int level = window_level_.GetWindowLevel() + 1024;
//	int low_ctv = level - width / 2;
//	int high_ctv = low_ctv + width;
//
//	int i;
//	int gM = min(low_ctv, kMaxCTV);
//	for (i = 0; i < gM; i++){
//		lut_table_[i][0] = (unsigned char)0;
//		lut_table_[i][1] = (unsigned char)0;
//		lut_table_[i][2] = (unsigned char)0;
//		lut_table_[i][3] = (unsigned char)0;
//	}
//
//	int gN = Max(high_ctv, 0);
//	for (i = gN; i < kMaxCTV; i++){
//		lut_table_[i][0] = (unsigned char)255;
//		lut_table_[i][1] = (unsigned char)255;
//		lut_table_[i][2] = (unsigned char)255;
//		lut_table_[i][3] = (unsigned char)255;
//	}
//
//
//	int tmp_val;
//	int gL = max(0, low_ctv);
//	int gH = min(high_ctv, kMaxCTV);
//	for (i = gL; i < gH; i++)
//	{
//		tmp_val = (int)((i - low_ctv) * 255 / width + 0.5);
//		if(tmp_val > 255)
//			tmp_val = 255;
//
//		//lut_table_[i] = RGBA_TO_BGRA(tmp_val, tmp_val, tmp_val, 255);
//
//		lut_table_[i][0] = (unsigned char)tmp_val;
//		lut_table_[i][1] = (unsigned char)tmp_val;
//		lut_table_[i][2] = (unsigned char)tmp_val;
//		lut_table_[i][3] = (unsigned char)255;
//	}
//}


void VoiLut::CreateLUT()
{
	float scalevalue = 1.0f;
	int width = window_level_.GetWindowWidth();
	int low_ctv = ConvertCTVToIndex(window_level_.GetWindowLevel() - width / 2);
	int high_ctv = ConvertCTVToIndex(low_ctv + width);

	int i;
	int gM = min(low_ctv, kMaxCTV);
	for (i = 0; i < gM; i++)
		lut_table_[i] = RGBA_TO_LONG(0, 0, 0, 255);

	int gN = Max(high_ctv, 0);
	for (i = gN; i < kMaxCTV; i++)
		lut_table_[i] = RGBA_TO_LONG(255, 255, 255, 255);


	int tmp_val;
	int gL = max(0, low_ctv);
	int gH = min(high_ctv, kMaxCTV);
	for (i = gL; i < gH; i++)
	{
		tmp_val = (int)((i - low_ctv) * 255 / width + 0.5);
		if(tmp_val > 255)
			tmp_val = 255;

		lut_table_[i] = RGBA_TO_LONG(tmp_val, tmp_val, tmp_val, 255);
	}
}

unsigned int *VoiLut::GetLutTable() const
{
	return lut_table_;
}

inline int ConvertCTVToIndex(int ctv)
{
	return ctv + 1024;
}