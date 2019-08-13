/*=========================================================================

  Program:   ImagingEngine
  Module:    bitmap.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "bitmap/ibitmap.h"

//#include "bitmap/BasicBitmap.h"
//#include "bitmap/bitmap_image.hpp"

namespace DW {
	namespace Bitmap {

#pragma pack(push, 1)

		typedef unsigned char  U8;
		typedef unsigned short U16;
		typedef unsigned int   U32;

		typedef struct tagBITMAPFILEHEADER
		{

			U16 bfType;
			U32 bfSize;
			U16 bfReserved1;
			U16 bfReserved2;
			U32 bfOffBits;
		} BITMAPFILEHEADER;

		typedef struct tagBITMAPINFOHEADER
		{
			U32 biSize;
			U32 biWidth;
			int biHeight;	//������ʾ���򣺵���/����
			U16 biPlanes;
			U16 biBitCount;
			U32 biCompression;
			U32 biSizeImage;
			U32 biXPelsPerMeter;
			U32 biYPelsPerMeter;
			U32 biClrUsed;
			U32 biClrImportant;
		} BITMAPINFOHEADER;

		typedef struct tagRGBQUAD
		{
			U8 rgbBlue;
			U8 rgbGreen;
			U8 rgbRed;
			U8 rgbReserved;
		} RGBQUAD;

		typedef struct tagBITMAPINFO
		{
			BITMAPINFOHEADER bmiHeader;
			RGBQUAD bmiColors[1];
		} BITMAPINFO;


		typedef struct tagBITMAP
		{
			BITMAPFILEHEADER bfHeader;
			BITMAPINFO biInfo;
		}BITMAPFILE;

		/// �ֽ�˳�򱣳�RGB
		typedef struct _LI_RGB  
		{   
			U8 r; 
			U8 g; 
			U8 b;   
		}LI_RGB; 

#pragma pack(pop)
		
		class DWBitmap : public IBitmap
		{
		public:
			DWBitmap();
			DWBitmap(char *data
				, unsigned int width
				, unsigned int height
				, unsigned int comp);
			~DWBitmap();
			unsigned int GetWidth() override;
			unsigned int GetHeight() override;
			unsigned int GetNumberOfComponent() override;
			void Save(const char *file_path) override;
			void SetBuffer(char *data, unsigned int width, unsigned int height, unsigned int comp) override;
			const char *GetBuffer() override;

			unsigned char GetPixelType() override;
			unsigned char GetPlanarConfiguration() override;
			const char *GetPhotometricInterpretation() override;
			const char *GetRescaleIntercept() override;
			const char *GetRescaleSlope() override;
			const char *GetRescaleType() override;
			unsigned short GetBitsAllocated() override;
			unsigned short GetBitsStored() override;
			unsigned short GetHighBit() override;
			unsigned short GetPixelRepresentation() override;
			bool IsTopDown() override;

			void SetPixelType(unsigned char);
			void DeepCopy(DWBitmap *src);
			IBitmap *Clone() override;

		private:
			/// ���
			unsigned int width_;
			/// �߶�
			unsigned int height_;
			/// ͨ����
			unsigned int number_of_component_;
			/// ��������
			char *pixel_data_;
			/// RGB���ظ�ʽ
			LI_RGB *pRGB;
			/// ͼ�����ظ�ʽ
			unsigned char pixel_type_;
			///// ʵ��Bitmap����
			//BasicBitmap *bitmap_impl_;
			///// ʵ��Bitmap����2
			//bitmap_image *bitmap_impl_;

		};


	}
}
