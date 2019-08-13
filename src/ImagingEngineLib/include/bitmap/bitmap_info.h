/*=========================================================================

  Program:   ImagingEngine
  Module:    bitmap_info.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once
#include "bitmap/ibitmap_info.h"
#include "data/show_buffer.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Bitmap {

		///
		// Store information that is used when saving a dicom file
		///
		class BitmapDcmInfo : public IBitmapDcmInfo
		{
		public:
			BitmapDcmInfo(BufferResult *result);
			~BitmapDcmInfo();

			/// Gets distance for each step, like rotation angle
			float GetStepValue() override;
			/// Gets 2d pixel spacings for the image
			void GetPixelSpacings(double spacings[2]) override;
			/// Gets image instance number
			int GetInstanceNumber() override;
			/// Gets image thickness
			float GetThickness() override;
			/// Gets image origin position in mm unit, it's right the image position patient tag in dicom file
			void GetOrigin(double origin[3]) override;
			/// Gets image orientation in x/y/z coordinate 
			void GetOrientation(double row_vector[3], double column_vector[3]) override;
			/// Gets window width & level
			void GetWindowLevel(int &ww, int &wl) override;
			/// Gets spacing between slices in mm unit
			float GetSpacingBetweenSlices() override;

		private:
			/// Pointer to a output result of 2d image control
			BufferResult *buffer_result_;

		};

	}
}