/*=========================================================================

  Program:   ImagingEngine
  Module:    general_equipment_module.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "module.h"

using namespace std;

namespace GIL {
	namespace DICOM {

		class GeneralEquipmentModule : public IODModule
		{
		public:
			//GeneralEquipmentModule();
			//~GeneralEquipmentModule();

			void Write(DcmItem *dataset)
			{
				dataset->putAndInsertString(ToDcmTag(DCM_Manufacturer), GetManufacturer().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_InstitutionName), GetInstitutionName().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_InstitutionAddress), GetInstitutionAddress().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_StationName), GetStationName().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_InstitutionalDepartmentName), GetInstitutionalDepartmentName().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_ManufacturerModelName), GetManufacturerModelName().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_DeviceSerialNumber), GetDeviceSerialNumber().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_SoftwareVersions), GetSoftwareVersions().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_SpatialResolution), GetSpatialResolution().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PixelPaddingValue), GetPixelPaddingValue().c_str());
			}
			void Read(DicomDataset *dataset)
			{
				SetManufacturer(dataset->getTag(GKDCM_Manufacturer));
				SetInstitutionName(dataset->getTag(GKDCM_InstitutionName));
				SetInstitutionAddress(dataset->getTag(GKDCM_InstitutionAddress));
				SetStationName(dataset->getTag(GKDCM_StationName));
				SetInstitutionalDepartmentName(dataset->getTag(GKDCM_InstitutionalDepartmentName));
				SetManufacturerModelName(dataset->getTag(GKDCM_ManufacturerModelName));
				SetSoftwareVersions(dataset->getTag(GKDCM_SoftwareVersions));
				SetDeviceSerialNumber(dataset->getTag(GKDCM_DeviceSerialNumber));
				SetSpatialResolution(dataset->getTag(GKDCM_SpatialResolution));
				SetPixelPaddingValue(dataset->getTag(GKDCM_PixelPaddingValue));
			}

			/// SeriesDate
			PropertyBuilderByName(std::string, Manufacturer);
			/// SeriesTime
			PropertyBuilderByName(std::string, InstitutionName);
			/// InstitutionAddress
			PropertyBuilderByName(std::string, InstitutionAddress);
			/// StationName
			PropertyBuilderByName(std::string, StationName);
			/// InstitutionalDepartmentName
			PropertyBuilderByName(std::string, InstitutionalDepartmentName);
			/// ManufacturerModelName
			PropertyBuilderByName(std::string, ManufacturerModelName);
			/// SoftwareVersions
			PropertyBuilderByName(std::string, SoftwareVersions);
			/// DeviceSerialNumber
			PropertyBuilderByName(std::string, DeviceSerialNumber);
			/// SpatialResolution
			PropertyBuilderByName(std::string, SpatialResolution);
			/// PixelPaddingValue
			PropertyBuilderByName(std::string, PixelPaddingValue);

		private:

		};

	}
}