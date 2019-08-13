/*=========================================================================

  Program:   ImagingEngine
  Module:    bitmap_info.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

namespace DW {
	namespace Bitmap {
		
		///
		// Store information that is used when saving a dicom file
		///
		class IBitmapDcmInfo
		{
		public:
			/// Gets distance for each step, like rotation angle
			virtual float GetStepValue() = 0;
			/// Gets 2d pixel spacings for the image
			virtual void GetPixelSpacings(double spacings[2]) = 0;
			/// Gets image instance number
			virtual int GetInstanceNumber() = 0;
			/// Gets image thickness
			virtual float GetThickness() = 0;
			/// Gets image origin position in mm unit, it's right the image position patient tag in dicom file
			virtual void GetOrigin(double origin[3]) = 0;
			/// Gets image orientation in x/y/z coordinate 
			virtual void GetOrientation(double row_vector[3], double column_vector[3]) = 0;
			/// Gets window width & level
			virtual void GetWindowLevel(int &ww, int &wl) = 0;
			/// Gets spacing between slices in mm unit
			virtual float GetSpacingBetweenSlices() = 0;

		};

	}
}