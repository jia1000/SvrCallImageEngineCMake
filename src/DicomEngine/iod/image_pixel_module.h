/*=========================================================================

  Program:   ImagingEngine
  Module:    image_pixel_module.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "module.h"

using namespace std;

namespace GIL {
	namespace DICOM {

		class ImagePixelModule : public IODModule
		{
		public:
			//ImagePixelModule();
			//~ImagePixelModule();

			void Write(DcmItem *dataset)
			{
				dataset->putAndInsertString(ToDcmTag(DCM_SamplesPerPixel), GetSamplesPerPixel().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PhotometricInterpretation), GetPhotometricInterpretation().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_Rows), GetRows().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_Columns), GetColumns().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_BitsAllocated), GetBitsAllocated().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_BitsStored), GetBitsStored().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_HighBit), GetHighBit().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PixelRepresentation), GetPixelRepresentation().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PlanarConfiguration), GetPlanarConfiguration().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PixelAspectRatio), GetPixelAspectRatio().c_str());
			}
			void Read(DicomDataset *dataset)
			{
				SetSamplesPerPixel(dataset->getTag(GKDCM_SamplesPerPixel));
				SetPhotometricInterpretation(dataset->getTag(GKDCM_PhotometricInterpretation));
				SetRows(dataset->getTag(GKDCM_Rows));
				SetColumns(dataset->getTag(GKDCM_Columns));
				SetBitsAllocated(dataset->getTag(GKDCM_BitsAllocated));
				SetBitsStored(dataset->getTag(GKDCM_BitsStored));
				SetHighBit(dataset->getTag(GKDCM_HighBit));
				SetPixelRepresentation(dataset->getTag(GKDCM_PixelRepresentation));
				SetPlanarConfiguration(dataset->getTag(GKDCM_PlanarConfiguration));
				SetPixelAspectRatio(dataset->getTag(GKDCM_PixelAspectRatio));
			}

			/// SamplesPerPixel
			PropertyBuilderByName(std::string, SamplesPerPixel);
			/// PhotometricInterpretation
			PropertyBuilderByName(std::string, PhotometricInterpretation);
			/// Rows
			PropertyBuilderByName(std::string, Rows);
			/// Columns
			PropertyBuilderByName(std::string, Columns);
			/// BitsAllocated
			PropertyBuilderByName(std::string, BitsAllocated);
			/// BitsStored
			PropertyBuilderByName(std::string, BitsStored);
			/// HighBit
			PropertyBuilderByName(std::string, HighBit);
			/// PixelRepresentation
			PropertyBuilderByName(std::string, PixelRepresentation);
			/// PlanarConfiguration
			PropertyBuilderByName(std::string, PlanarConfiguration);
			/// PixelAspectRatio
			PropertyBuilderByName(std::string, PixelAspectRatio);

		private:

		};

	}
}