/*=========================================================================

  Program:   ImagingEngine
  Module:    bmp_source.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include <iostream>

namespace GIL {
	namespace DICOM {

		///
		// Base Class for plugins extracting pixel data from standard image data
		///
		class IGeneratePixelData
		{
		public:
			/// Gets image width
			virtual unsigned int GetWidth() = 0;
			/// Gets image height
			virtual unsigned int GetHeight() = 0;
			/// Gets bytes per pixel
			virtual unsigned int GetSamplesPerPixel() = 0;
			/// Gets buffer data,
			/// Output units = m*SV+b
			/// The value SV (stored values) in the above formula.
			virtual char *GetPixelData() = 0;
			/// Gets the DICOM color model used for the compressed data
			virtual const char *GetPhotometricInterpretation() = 0;
			/// Gets bits Allocated for one sample
			virtual unsigned short GetBitsAllocated() = 0;
			/// Gets bits Stored, number of bits stored within Bits Allocated
			virtual unsigned short GetBitsStored() = 0;
			/// Gets high Bit, highest bit position set within Bits Allocated
			virtual unsigned short GetHighBit() = 0;
			/// Gets pixel Representation (0=unsigned, 1=signed)
			virtual unsigned short GetPixelRepresentation() = 0;
			/// Gets planar Configuration
			/// Indicates whether the pixel data are encoded color-by-plane or color-by-pixel. 
			/// Required if Samples per Pixel (0028,0002) has a value greater than 1
			/// Enumerated Values:
			/// 0 The sample values for the first pixel are followed by the sample values for the second pixel, etc. For RGB images, this means the order of the pixel values encoded shall be R1, G1, B1, R2, G2, B2, бн, etc.
			/// 1 Each color plane shall be encoded contiguously. For RGB images, this means the order of the pixel values encoded is R1, R2, R3, бн, G1, G2, G3, бн, B1, B2, B3, etc.
			virtual unsigned short GetPlanarConfiguration() = 0;
			/// Gets length of pixel data
			virtual unsigned long GetPixelLength() = 0;
			/// Gets pixel rescale intercept
			/// The value b in the equation specified in GetPixelData.
			virtual const char *GetRescaleIntercept() = 0;
			/// Gets pixel rescale slope
			/// The value m in the equation specified in GetPixelData.
			virtual const char *GetRescaleSlope() = 0;
			/// Gets pixel rescale type
			/// Required if the Rescale Type is not HU (Hounsfield Units). May be present otherwise.
			virtual const char *GetRescaleType() = 0;

			///// Gets the transfer syntax imposed by the imported pixel pixel data
			//virtual unsigned short GetTransferSyntax() = 0;

		};
	}
}