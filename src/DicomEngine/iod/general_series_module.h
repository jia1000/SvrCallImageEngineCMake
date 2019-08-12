/*=========================================================================

  Program:   ImagingEngine
  Module:    general_series_module.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "module.h"

using namespace std;

namespace GIL {
	namespace DICOM {

		class GeneralSeriesModule : public IODModule
		{
		public:
			//GeneralSeriesModule();
			//~GeneralSeriesModule();

			void Write(DcmItem *dataset)
			{
				dataset->putAndInsertString(ToDcmTag(DCM_SeriesDate), GetSeriesDate().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_SeriesTime), GetSeriesTime().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_Modality), GetModality().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_SeriesDescription), GetSeriesDescription().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PerformingPhysicianName), GetPerformingPhysicianName().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_OperatorsName), GetOperatorsName().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_ProtocolName), GetProtocolName().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_BodyPartExamined), GetBodyPartExamined().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PatientPosition), GetPatientPosition().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_SeriesInstanceUID), GetSeriesInstanceUID().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_SeriesNumber), GetSeriesNumber().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_Laterality), GetLaterality().c_str());
			}
			void Read(DicomDataset *dataset)
			{
				SetSeriesDate(dataset->getTag(GKDCM_SeriesDate));
				SetSeriesTime(dataset->getTag(GKDCM_SeriesTime));
				SetModality(dataset->getTag(GKDCM_Modality));
				SetSeriesDescription(dataset->getTag(GKDCM_SeriesDescription));
				SetPerformingPhysicianName(dataset->getTag(GKDCM_PerformingPhysicianName));
				SetOperatorsName(dataset->getTag(GKDCM_OperatorsName));
				SetBodyPartExamined(dataset->getTag(GKDCM_BodyPartExamined));
				SetProtocolName(dataset->getTag(GKDCM_ProtocolName));
				SetPatientPosition(dataset->getTag(GKDCM_PatientPosition));
				SetSeriesInstanceUID(dataset->getTag(GKDCM_SeriesInstanceUID));
				SetSeriesNumber(dataset->getTag(GKDCM_SeriesNumber));
				SetLaterality(dataset->getTag(GKDCM_Laterality));
			}

			/// SeriesDate
			PropertyBuilderByName(std::string, SeriesDate);
			/// SeriesTime
			PropertyBuilderByName(std::string, SeriesTime);
			/// Modality
			PropertyBuilderByName(std::string, Modality);
			/// SeriesDescription
			PropertyBuilderByName(std::string, SeriesDescription);
			/// PerformingPhisiciansName
			PropertyBuilderByName(std::string, PerformingPhysicianName);
			/// OperatorsName
			PropertyBuilderByName(std::string, OperatorsName);
			/// BodyPartExamined
			PropertyBuilderByName(std::string, BodyPartExamined);
			/// ProtocolName
			PropertyBuilderByName(std::string, ProtocolName);
			/// PatientPosition
			PropertyBuilderByName(std::string, PatientPosition);
			/// SeriesInstanceUID
			PropertyBuilderByName(std::string, SeriesInstanceUID);
			/// SeriesNumber
			PropertyBuilderByName(std::string, SeriesNumber);
			/// Literality
			PropertyBuilderByName(std::string, Laterality);

		private:

		};
		
	}
}