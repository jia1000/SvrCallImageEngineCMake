/*=========================================================================

  Program:   ImagingEngine
  Module:    sop_common_module.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "module.h"

using namespace std;

namespace GIL {
	namespace DICOM {

		class SOPCommonModule : public IODModule
		{
		public:
			//SOPCommonModule();
			//~SOPCommonModule();

			void Write(DcmItem *dataset)
			{
				dataset->putAndInsertString(ToDcmTag(DCM_SpecificCharacterSet), GetSpecificCharacterSet().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_InstanceCreationDate), GetInstanceCreationDate().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_InstanceCreationTime), GetInstanceCreationTime().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_InstanceCreatorUID), GetInstanceCreatorUID().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_SOPClassUID), GetSOPClassUID().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_SOPInstanceUID), GetSOPInstanceUID().c_str());
			}
			void Read(DicomDataset *dataset)
			{
				SetSpecificCharacterSet(dataset->getTag(GKDCM_SpecificCharacterSet));
				SetInstanceCreationDate(dataset->getTag(GKDCM_InstanceCreationDate));
				SetInstanceCreationTime(dataset->getTag(GKDCM_InstanceCreationTime));
				SetInstanceCreatorUID(dataset->getTag(GKDCM_InstanceCreatorUID));
				SetSOPClassUID(dataset->getTag(GKDCM_SOPClassUID));
				SetSOPInstanceUID(dataset->getTag(GKDCM_SOPInstanceUID));
			}

			/// SpecificCharacterSet
			PropertyBuilderByName(std::string, SpecificCharacterSet);
			/// InstanceCreationDate
			PropertyBuilderByName(std::string, InstanceCreationDate);
			/// InstanceCreationTime
			PropertyBuilderByName(std::string, InstanceCreationTime);
			/// InstanceCreatorUID
			PropertyBuilderByName(std::string, InstanceCreatorUID);
			/// SOPClassUID
			PropertyBuilderByName(std::string, SOPClassUID);
			/// SOPInstanceUID
			PropertyBuilderByName(std::string, SOPInstanceUID);

		private:

		};

	}
}