/*=========================================================================

  Program:   ImagingEngine
  Module:    voi_lut_module.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "module.h"

using namespace std;

namespace GIL {
	namespace DICOM {

		class VoiLutModule : public IODModule
		{
		public:
			//VoiLutModule();
			//~VoiLutModule();

			void Write(DcmItem *dataset)
			{
				dataset->putAndInsertString(ToDcmTag(DCM_WindowCenter), GetWindowCenter().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_WindowWidth), GetWindowWidth().c_str());
			}
			void Read(DicomDataset *dataset)
			{
				SetWindowCenter(dataset->getTag(GKDCM_WindowCenter));
				SetWindowWidth(dataset->getTag(GKDCM_WindowWidth));
			}

			/// WindowCenter
			PropertyBuilderByName(std::string, WindowCenter);
			/// WindowWidth
			PropertyBuilderByName(std::string, WindowWidth);

		private:

		};

	}
}