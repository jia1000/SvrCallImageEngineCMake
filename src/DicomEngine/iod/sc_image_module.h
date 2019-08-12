/*=========================================================================

  Program:   ImagingEngine
  Module:    sc_image_module.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "module.h"

using namespace std;

namespace GIL {
	namespace DICOM {

		class SCImageModule : public IODModule
		{
		public:
			//SCImageModule();
			//~SCImageModule();

			void Write(DcmItem *dataset)
			{
				dataset->putAndInsertString(ToDcmTag(DCM_DateOfSecondaryCapture), GetDateOfSecondaryCapture().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_TimeOfSecondaryCapture), GetTimeOfSecondaryCapture().c_str());
			}
			void Read(DicomDataset *dataset)
			{
				SetDateOfSecondaryCapture(dataset->getTag(GKDCM_DateOfSecondaryCapture));
				SetTimeOfSecondaryCapture(dataset->getTag(GKDCM_TimeOfSecondaryCapture));
			}

			/// DateOfSecondaryCapture
			PropertyBuilderByName(std::string, DateOfSecondaryCapture);
			/// TimeOfSecondaryCapture
			PropertyBuilderByName(std::string, TimeOfSecondaryCapture);

		private:

		};

	}
}