/*=========================================================================

  Program:   ImagingEngine
  Module:    ibitmap.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once


namespace DW {
	namespace Bitmap {

		///
		// Represents a bitmap object with a structure to store information to save as a dicom file
		///
		class IBitmap
		{
		public:
			/// Gets pixel type, default is 0
			/// 0 = Unsigned, both Modality Lut and VOI Lut are applied
			/// 1 = Signed, Modality Lut is applied, but VOI Lut is not applied
			/// 2 = Unsigned, both Modality Lut and VOI Lut are not applied
			/// 3 = Signed, both Modality Lut and VOI Lut are not applied. A special case in contrast to value 2
			virtual unsigned char GetPixelType() = 0;
			/// Gets image width
			virtual unsigned int GetWidth() = 0;
			/// Gets image height
			virtual unsigned int GetHeight() = 0;
			/// Gets bytes per pixel
			virtual unsigned int GetNumberOfComponent() = 0;
			/// Save to file
			virtual void Save(const char *file_path) = 0;
			/// Sets buffer data
			virtual void SetBuffer(char *data, unsigned int width, unsigned int height, unsigned int comp) = 0;
			/// Gets buffer data
			virtual const char *GetBuffer() = 0;
			
			/// Gets planar Configuration
			/// Indicates whether the pixel data are encoded color-by-plane or color-by-pixel. 
			/// Required if Samples per Pixel (0028,0002) has a value greater than 1
			/// Enumerated Values:
			/// 0 The sample values for the first pixel are followed by the sample values for the second pixel, etc. For RGB images, this means the order of the pixel values encoded shall be R1, G1, B1, R2, G2, B2, бн, etc.
			/// 1 Each color plane shall be encoded contiguously. For RGB images, this means the order of the pixel values encoded is R1, R2, R3, бн, G1, G2, G3, бн, B1, B2, B3, etc.
			virtual unsigned char GetPlanarConfiguration() = 0;
			/// Gets value that specifies the intended interpretation of the pixel data
			/// Enumerated Values:
			/// EMONOCHROME1
			/// MONOCHROME2
			/// RGB
			virtual const char *GetPhotometricInterpretation() = 0;
			/// Gets pixel rescale intercept
			/// Output units = m*SV+b
			/// The value b in relationship between stored values (SV) and the output units.
			virtual const char *GetRescaleIntercept() = 0;
			/// Gets pixel rescale slope
			/// The value m in the equation specified in GetRescaleIntercept.
			virtual const char *GetRescaleSlope() = 0;
			/// Gets pixel rescale type
			/// Required if the Rescale Type is not HU (Hounsfield Units). May be present otherwise.
			virtual const char *GetRescaleType() = 0;
			/// Gets bits Allocated for one sample
			virtual unsigned short GetBitsAllocated() = 0;
			/// Gets bits Stored, number of bits stored within Bits Allocated
			virtual unsigned short GetBitsStored() = 0;
			/// Gets high Bit, highest bit position set within Bits Allocated
			virtual unsigned short GetHighBit() = 0;
			/// Gets pixel Representation (0=unsigned, 1=signed)
			virtual unsigned short GetPixelRepresentation() = 0;
			
			/// Clones a new IBitmap object by deep copy
			virtual IBitmap *Clone() = 0;
			/// Gets a flag that indicates the pixel data is top down sequence
			virtual bool IsTopDown() = 0;

		};

	}
}