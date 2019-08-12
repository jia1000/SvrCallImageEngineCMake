/*=========================================================================

  Program:   ImagingEngine
  Module:    sc_equipment_module.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "module.h"

using namespace std;

namespace GIL {
	namespace DICOM {

		class SCEquipmentModule : public IODModule
		{
		public:
			//SCEquipmentModule();
			//~SCEquipmentModule();

			void Write(DcmItem *dataset)
			{
				dataset->putAndInsertString(ToDcmTag(DCM_Modality), GetModality().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_ConversionType), GetConversionType().c_str());
			}
			void Read(DicomDataset *dataset)
			{
				SetModality(dataset->getTag(GKDCM_Modality));
				SetConversionType(dataset->getTag(GKDCM_ConversionType));
			}

			/// Modality
			PropertyBuilderByName(std::string, Modality);
			/// ConversionType
			PropertyBuilderByName(std::string, ConversionType);

		private:

		};

	}
}