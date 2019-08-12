/*=========================================================================

  Program:   ImagingEngine
  Module:    modality_lut_module.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "module.h"

using namespace std;

namespace GIL {
	namespace DICOM {

		class ModalityLutModule : public IODModule
		{
		public:
			//ModalityLutModule();
			//~ModalityLutModule();

			void Write(DcmItem *dataset)
			{
				dataset->putAndInsertString(ToDcmTag(DCM_RescaleIntercept), GetRescaleIntercept().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_RescaleSlope), GetRescaleSlope().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_RescaleType), GetRescaleType().c_str());
			}
			void Read(DicomDataset *dataset)
			{
				SetRescaleIntercept(dataset->getTag(GKDCM_RescaleIntercept));
				SetRescaleSlope(dataset->getTag(GKDCM_RescaleSlope));
				SetRescaleType(dataset->getTag(GKDCM_RescaleType));
			}

			/// RescaleIntercept
			PropertyBuilderByName(std::string, RescaleIntercept);
			/// RescaleSlope
			PropertyBuilderByName(std::string, RescaleSlope);
			/// RescaleType
			PropertyBuilderByName(std::string, RescaleType);

		private:

		};
		
	}
}