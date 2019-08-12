/*=========================================================================

  Program:   ImagingEngine
  Module:    patient_study_module.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "module.h"

using namespace std;

namespace GIL {
	namespace DICOM {

		class PatientStudyModule : public IODModule
		{
		public:
			//PatientStudyModule();
			//~PatientStudyModule();

			void Write(DcmItem *dataset)
			{
				dataset->putAndInsertString(ToDcmTag(DCM_PatientAge), GetPatientAge().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PatientSize), GetPatientSize().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PatientWeight), GetPatientWeight().c_str());
			}
			void Read(DicomDataset *dataset)
			{
				SetPatientAge(dataset->getTag(GKDCM_PatientAge));
				SetPatientSize(dataset->getTag(GKDCM_PatientSize));
				SetPatientWeight(dataset->getTag(GKDCM_PatientWeight));
			}

			/// PatientAge
			PropertyBuilderByName(std::string, PatientAge);
			/// PatientSize
			PropertyBuilderByName(std::string, PatientSize);
			/// PatientWeight
			PropertyBuilderByName(std::string, PatientWeight);

		private:

		};
		
	}
}