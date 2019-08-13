/*=========================================================================

  Program:   ImagingEngine
  Module:    bitmap.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "bitmap/bitmap.h"

using namespace DW::Bitmap;

/// 生成BMP图片(无颜色表的位图):在RGB(A)位图数据的基础上加上文件信息头和位图信息头
int GenBmpFile(U8 *pData, U8 bitCountPerPix, int width, int height, const char *filename);
/// 获取BMP文件的位图数据(无颜色表的位图):丢掉BMP文件的文件信息头和位图信息头，获取其RGB(A)位图数据
U8* GetBmpData(U8 *bitCountPerPix, U32 *width, U32 *height, const char* filename);
/// 释放GetBmpData分配的空间
void FreeBmpData(U8 *pdata);
/// 生成BMP图片(无颜色表的位图):在RGB(A)位图数据的基础上加上文件信息头和位图信息头  
int GenBmpFile(U8 *pData, U8 bitCountPerPix, int width, int height, const char *filename)  
{  
	FILE *fp = fopen(filename, "wb");  
	if(!fp)  
	{  
		printf("fopen failed : %s, %d\n", __FILE__, __LINE__);  
		return 0;  
	}  

	U32 bmppitch = ((width*bitCountPerPix + 31) >> 5) << 2;  
	U32 filesize = bmppitch*height;  

	BITMAPFILE bmpfile;  

	bmpfile.bfHeader.bfType = 0x4D42;  
	bmpfile.bfHeader.bfSize = filesize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  
	bmpfile.bfHeader.bfReserved1 = 0;  
	bmpfile.bfHeader.bfReserved2 = 0;  
	bmpfile.bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  

	bmpfile.biInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  
	bmpfile.biInfo.bmiHeader.biWidth = width;  
	bmpfile.biInfo.bmiHeader.biHeight = -height;	//负号表示从上到下顺序
	bmpfile.biInfo.bmiHeader.biPlanes = 1;  
	bmpfile.biInfo.bmiHeader.biBitCount = bitCountPerPix;  
	bmpfile.biInfo.bmiHeader.biCompression = 0;  
	bmpfile.biInfo.bmiHeader.biSizeImage = 0;  
	bmpfile.biInfo.bmiHeader.biXPelsPerMeter = 0;  
	bmpfile.biInfo.bmiHeader.biYPelsPerMeter = 0;  
	bmpfile.biInfo.bmiHeader.biClrUsed = 0;  
	bmpfile.biInfo.bmiHeader.biClrImportant = 0;  

	fwrite(&(bmpfile.bfHeader), sizeof(BITMAPFILEHEADER), 1, fp);  
	fwrite(&(bmpfile.biInfo.bmiHeader), sizeof(BITMAPINFOHEADER), 1, fp);  

	U8 *pEachLinBuf = (U8*)malloc(bmppitch);  
	memset(pEachLinBuf, 0, bmppitch);  
	U8 BytePerPix = bitCountPerPix >> 3;  
	U32 pitch = width * 3;  
	if(pEachLinBuf)  
	{  
		int h,w;  
		for(h = height-1; h >= 0; h--)  
		{  
			for(w = 0; w < width; w++)  
			{  
				if (BytePerPix >= 3){
					//copy by a pixel  
					pEachLinBuf[w*3+0] = pData[h*pitch + w*BytePerPix + 2];  
					pEachLinBuf[w*3+1] = pData[h*pitch + w*BytePerPix + 1];  
					pEachLinBuf[w*3+2] = pData[h*pitch + w*BytePerPix + 0]; 
				}				 
			}  
			fwrite(pEachLinBuf, bmppitch, 1, fp);  

		}  
		free(pEachLinBuf);  
	}  

	fclose(fp);  

	return 1;  
}  

/// 获取BMP文件的位图数据(无颜色表的位图):丢掉BMP文件的文件信息头和位图信息头，获取其RGB(A)位图数据  
U8* GetBmpData(U8 *bitCountPerPix, U32 *width, U32 *height, const char* filename)  
{  
	FILE *pf = fopen(filename, "rb");  
	if(!pf)  
	{  
		printf("fopen failed : %s, %d\n", __FILE__, __LINE__);  
		return NULL;  
	}  

	BITMAPFILE bmpfile;  
	fread(&(bmpfile.bfHeader), sizeof(BITMAPFILEHEADER), 1, pf);  
	fread(&(bmpfile.biInfo.bmiHeader), sizeof(BITMAPINFOHEADER), 1, pf);  

	//print_bmfh(bmpfile.bfHeader);  
	//print_bmih(bmpfile.biInfo.bmiHeader);  

	if(bitCountPerPix)  
	{  
		*bitCountPerPix = bmpfile.biInfo.bmiHeader.biBitCount;  
	}  
	if(width)  
	{  
		*width = bmpfile.biInfo.bmiHeader.biWidth;  
	}  
	if(height)  
	{  
		*height = bmpfile.biInfo.bmiHeader.biHeight;  
	}  

	U32 bmppicth = (((*width)*(*bitCountPerPix) + 31) >> 5) << 2;  
	U8 *pdata = (U8*)malloc((*height)*bmppicth);  

	U8 *pEachLinBuf = (U8*)malloc(bmppicth);  
	memset(pEachLinBuf, 0, bmppicth);  
	U8 BytePerPix = (*bitCountPerPix) >> 3;  
	U32 pitch = (*width) * BytePerPix;  

	if(pdata && pEachLinBuf)  
	{  
		int w, h;  
		for(h = (*height) - 1; h >= 0; h--)  
		{  
			fread(pEachLinBuf, bmppicth, 1, pf);  
			for(w = 0; w < (*width); w++)  
			{  
				pdata[h*pitch + w*BytePerPix + 0] = pEachLinBuf[w*BytePerPix+0];  
				pdata[h*pitch + w*BytePerPix + 1] = pEachLinBuf[w*BytePerPix+1];  
				pdata[h*pitch + w*BytePerPix + 2] = pEachLinBuf[w*BytePerPix+2];  
			}  
		}  
		free(pEachLinBuf);  
	}  
	fclose(pf);  

	return pdata;  
}  

/// 释放GetBmpData分配的空间  
void FreeBmpData(U8 *pdata)  
{  
	if(pdata)  
	{  
		free(pdata);  
		pdata = NULL;  
	}  
}  

DWBitmap::DWBitmap()
				   : width_(0)
				   , height_(0)
				   , number_of_component_(0)
{
	// bitmap_image只支持24bit
	// Linux下需要初始化NULL
	//bitmap_impl_ = NULL;
	pRGB = NULL;
	pixel_data_ = NULL;
}

DWBitmap::DWBitmap(char *data
		 , unsigned int width
		 , unsigned int height
		 , unsigned int comp)
		 : width_(width)
		 , height_(height)
		 , number_of_component_(comp)
{
	// bitmap_image只支持24bit
	//TODO 实现bitmap
	//bitmap_impl_ = new BasicBitmap(width_, height_, BasicBitmap::A8R8G8B8, reinterpret_cast<void *>(data), 0);
	//bitmap_impl_ = new bitmap_image(width_, height_);
	//int size = width * height;
	//int i, j;
	//for(i=0; i<width_; ++i){
	//	for(j=0; i<height_; ++j){
	//		rgb_t color;
	//		color.red = (unsigned char)data[i * width_ * 4 + j * 4];
	//		color.green = (unsigned char)data[i * width_ * 4 + j * 4 +];
	//		color.blue = (unsigned char)data[i * width_ * 4 + j * 4 + 2];
	//		bitmap_impl_->set_pixel(i, j, color);
	//	}
	//}
	// Linux下需要初始化NULL
	pRGB = NULL;
	pixel_data_ = NULL;
}

DWBitmap::~DWBitmap()
{
	//if (bitmap_impl_){
	//	delete bitmap_impl_;
	//	bitmap_impl_ = NULL;
	//}
	if (pixel_data_){
		delete pixel_data_;
		pixel_data_ = NULL;
	}
}

unsigned int DWBitmap::GetWidth()
{
	return width_;
}

unsigned int DWBitmap::GetHeight()
{
	return height_;
}

unsigned int DWBitmap::GetNumberOfComponent()
{
	return number_of_component_;
}

void DWBitmap::Save(const char *file_path)
{
	//生成BMP文件
	GenBmpFile((U8*)pixel_data_, 24, width_, height_, file_path); 

	//if (bitmap_impl_){
	//	// BasicColor用于调色板
	//	bitmap_impl_->SaveBmp(file_path);
	//}
}

void DWBitmap::SetBuffer(char *data, unsigned int width, unsigned int height, unsigned int comp)
{
	width_ = width;
	height_ = height;
	number_of_component_ = comp;
	int size = width * height;

	if (pixel_data_){
		delete pixel_data_;
	}
	//TODO 判断通道数
	if (comp == 2){
		pixel_data_ = new char[size * number_of_component_];
		memcpy(pixel_data_, data, size * number_of_component_);
	}
	else if (comp >= 3){
		//LI_RGB *tmp_RGB = new LI_RGB[size];
		pixel_data_ = new char[size * 3];
		int i, j;
		for(i=0; i<size; ++i){

			pixel_data_[i * 3] = (unsigned char)data[i * number_of_component_];
			pixel_data_[i * 3 + 1] = (unsigned char)data[i * number_of_component_ + 1];
			pixel_data_[i * 3 + 2] = (unsigned char)data[i * number_of_component_ + 2];

			//tmp_RGB[i].r = (unsigned char)data[i * number_of_component_];
			//tmp_RGB[i].g = (unsigned char)data[i * number_of_component_ + 1];
			//tmp_RGB[i].b = (unsigned char)data[i * number_of_component_ + 2];
		}
		//pixel_data_ = new char[size * 3];
		//memcpy(pixel_data_, reinterpret_cast<char *>(tmp_RGB), size * 3);
		//delete[] tmp_RGB;

		number_of_component_ = 3;
	}
}

const char *DWBitmap::GetBuffer()
{
	return pixel_data_;
	//return (char *)pRGB;
}

unsigned char DWBitmap::GetPlanarConfiguration()
{
	if (number_of_component_ == 3){
		return 0;
	}
	return 0;
}

const char *DWBitmap::GetPhotometricInterpretation()
{
	if (number_of_component_ == 2)
		return "MONOCHROME2";

	return "RGB";
}

const char *DWBitmap::GetRescaleIntercept()
{
	return "0";
}

const char *DWBitmap::GetRescaleSlope()
{
	return "1";
}

const char *DWBitmap::GetRescaleType()
{
	return "HU";
}

unsigned char DWBitmap::GetPixelType()
{
	return pixel_type_;
}

unsigned short DWBitmap::GetBitsAllocated()
{
	if (number_of_component_ == 2)
		return 16;

	return 8;
}

unsigned short DWBitmap::GetBitsStored()
{
	if (number_of_component_ == 2)
		return 16;

	return 8;
}

unsigned short DWBitmap::GetHighBit()
{
	if (number_of_component_ == 2)
		return 15;

	return 7;
}

unsigned short DWBitmap::GetPixelRepresentation()
{
	//TODO 存储无符号值，viewer在解释pixeldata时没有加上偏移量，还未查到原因
	return 1;		//Signed
	//return 0;		//Unsigned
}

bool DWBitmap::IsTopDown()
{
	if (number_of_component_ == 2)
		return true;

	return false;
}

void DWBitmap::SetPixelType(unsigned char pixel)
{
	pixel_type_ = pixel;
}

void DWBitmap::DeepCopy(DWBitmap *src)
{
	width_ = src->width_;
	height_ = src->height_;
	pixel_type_ = src->pixel_type_;

	if (pixel_data_){
		delete pixel_data_;
	}
	memcpy(pixel_data_, src->pixel_data_, width_ * height_ * number_of_component_);
}

IBitmap *DWBitmap::Clone()
{
	DWBitmap *new_bmp = new DWBitmap();
	new_bmp->width_ = width_;
	new_bmp->height_ = height_;
	new_bmp->number_of_component_ = number_of_component_;
	new_bmp->pixel_type_ = pixel_type_;

	int size = width_ * height_ * number_of_component_;
	new_bmp->pixel_data_ = new char[size];
	memcpy(new_bmp->pixel_data_, pixel_data_, size);

	return new_bmp;
}