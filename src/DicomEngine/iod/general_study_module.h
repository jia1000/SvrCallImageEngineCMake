/*=========================================================================

  Program:   ImagingEngine
  Module:    general_study_module.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "module.h"

using namespace std;

namespace GIL {
	namespace DICOM {

		class GeneralStudyModule : public IODModule
		{
		public:
			//GeneralStudyModule();
			//~GeneralStudyModule();

			void Write(DcmItem *dataset)
			{
				dataset->putAndInsertString(ToDcmTag(DCM_StudyDate), GetStudyDate().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_StudyTime), GetStudyTime().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_AccessionNumber), GetAccessionNumber().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_ReferringPhysicianName), GetReferringPhysicianName().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_StudyDescription), GetStudyDescription().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_StudyInstanceUID), GetStudyInstanceUID().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_StudyID), GetStudyID().c_str());
			}
			void Read(DicomDataset *dataset)
			{
				SetStudyDate(dataset->getTag(GKDCM_StudyDate));
				SetStudyTime(dataset->getTag(GKDCM_StudyTime));
				SetAccessionNumber(dataset->getTag(GKDCM_AccessionNumber));
				SetReferringPhysicianName(dataset->getTag(GKDCM_ReferringPhysicianName));
				SetStudyDescription(dataset->getTag(GKDCM_StudyDescription));
				SetStudyInstanceUID(dataset->getTag(GKDCM_StudyInstanceUID));
				SetStudyID(dataset->getTag(GKDCM_StudyID));
			}

			/// StudyDate
			PropertyBuilderByName(std::string, StudyDate);
			/// StudyTime
			PropertyBuilderByName(std::string, StudyTime);
			/// AccessionNumber
			PropertyBuilderByName(std::string, AccessionNumber);
			/// ReferringPhysiciansName
			PropertyBuilderByName(std::string, ReferringPhysicianName);
			/// StudyDescription
			PropertyBuilderByName(std::string, StudyDescription);
			/// StudyInstaneUID
			PropertyBuilderByName(std::string, StudyInstanceUID);
			/// StudyId
			PropertyBuilderByName(std::string, StudyID);


		private:

		};

	}
}