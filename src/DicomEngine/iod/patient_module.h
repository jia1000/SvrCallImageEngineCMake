/*=========================================================================

  Program:   ImagingEngine
  Module:    patient_iod_module.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "module.h"

using namespace std;

namespace GIL {
	namespace DICOM {

		class PatientModule : public IODModule
		{
		public:
			//PatientModule();
			//~PatientModule();

			void Write(DcmItem *dataset)
			{
				dataset->putAndInsertString(ToDcmTag(DCM_PatientName), GetPatientName().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PatientID), GetPatientId().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PatientSex), GetPatientSex().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PatientBirthDate), GetPatientBirthDate().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PatientBirthTime), GetPatientBirthTime().c_str());
			}
			void Read(DicomDataset *dataset)
			{
				SetPatientName(dataset->getTag(GKDCM_PatientName));
				SetPatientId(dataset->getTag(GKDCM_PatientID));
				SetPatientSex(dataset->getTag(GKDCM_PatientSex));
				SetPatientBirthDate(dataset->getTag(GKDCM_PatientBirthDate));
				SetPatientBirthTime(dataset->getTag(GKDCM_PatientBirthTime));
			}

			/// Patients name 
			PropertyBuilderByName(std::string, PatientName);
			/// Patient Id
			PropertyBuilderByName(std::string, PatientId);
			/// Patients Sex
			PropertyBuilderByName(std::string, PatientSex);
			/// PatientsBirthDate
			PropertyBuilderByName(std::string, PatientBirthDate);
			/// PatientsBirthTime
			PropertyBuilderByName(std::string, PatientBirthTime);
			

		private:

		};

	}
}