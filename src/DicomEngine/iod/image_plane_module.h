/*=========================================================================

  Program:   ImagingEngine
  Module:    image_plane_module.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "module.h"

using namespace std;

namespace GIL {
	namespace DICOM {

		class ImagePlaneModule : public IODModule
		{
		public:
			//ImagePlaneModule();
			//~ImagePlaneModule();

			void Write(DcmItem *dataset)
			{
				dataset->putAndInsertString(ToDcmTag(DCM_SliceThickness), GetSliceThickness().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_ImagePositionPatient), GetImagePositionPatient().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_ImageOrientationPatient), GetImageOrientationPatient().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_SliceLocation), GetSliceLocation().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PixelSpacing), GetPixelSpacing().c_str());
			}
			void Read(DicomDataset *dataset)
			{
				SetSliceThickness(dataset->getTag(GKDCM_SliceThickness));
				SetImagePositionPatient(dataset->getTag(GKDCM_ImagePositionPatient));
				SetImageOrientationPatient(dataset->getTag(GKDCM_ImageOrientationPatient));
				SetSliceLocation(dataset->getTag(GKDCM_SliceLocation));
				SetPixelSpacing(dataset->getTag(GKDCM_PixelSpacing));
			}

			/// SliceThickness
			PropertyBuilderByName(std::string, SliceThickness);
			/// ImagePositionPatient
			PropertyBuilderByName(std::string, ImagePositionPatient);
			/// ImageOrientationPatient
			PropertyBuilderByName(std::string, ImageOrientationPatient);
			/// SliceLocation
			PropertyBuilderByName(std::string, SliceLocation);
			/// PixelSpacing
			PropertyBuilderByName(std::string, PixelSpacing);

		private:

		};

	}
}