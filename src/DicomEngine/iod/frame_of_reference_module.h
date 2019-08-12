/*=========================================================================

  Program:   ImagingEngine
  Module:    frame_of_reference_module.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "module.h"

using namespace std;

namespace GIL {
	namespace DICOM {

		class FrameOfReferenceModule : public IODModule
		{
		public:
			//FrameOfReferenceModule();
			//~FrameOfReferenceModule();

			void Write(DcmItem *dataset)
			{
				dataset->putAndInsertString(ToDcmTag(DCM_FrameOfReferenceUID), GetFrameOfReferenceUID().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PositionReferenceIndicator), GetPositionReferenceIndicator().c_str());
			}
			void Read(DicomDataset *dataset)
			{
				SetFrameOfReferenceUID(dataset->getTag(GKDCM_FrameOfReferenceUID));
				SetPositionReferenceIndicator(dataset->getTag(GKDCM_PositionReferenceIndicator));
			}

			/// FrameOfReferenceUID
			PropertyBuilderByName(std::string, FrameOfReferenceUID);
			/// PositionReferenceIndicator
			PropertyBuilderByName(std::string, PositionReferenceIndicator);

		private:

		};

	}
}