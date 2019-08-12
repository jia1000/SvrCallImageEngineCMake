/*=========================================================================

  Program:   ImagingEngine
  Module:    meta_info_module.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "module.h"

using namespace std;

namespace GIL {
	namespace DICOM {

		class MetaInfoModule : public IODModule
		{
		public:
			//MetaInfoModule();
			//~MetaInfoModule();

			void Write(DcmItem *dataset)
			{
				dataset->putAndInsertString(ToDcmTag(DCM_FileMetaInformationGroupLength), GetFileMetaInformationGroupLength().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_FileMetaInformationVersion), GetFileMetaInformationVersion().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_MediaStorageSOPClassUID), GetMediaStorageSOPClassUID().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_MediaStorageSOPInstanceUID), GetMediaStorageSOPInstanceUID().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_TransferSyntaxUID), GetTransferSyntaxUID().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_ImplementationClassUID), GetImplementationClassUID().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_ImplementationVersionName), GetImplementationVersionName().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_SourceApplicationEntityTitle), GetSourceApplicationEntityTitle().c_str());
			}
			void Read(DicomDataset *dataset)
			{
				SetFileMetaInformationGroupLength(dataset->getTag(GKDCM_FileMetaInformationGroupLength));
				SetFileMetaInformationVersion(dataset->getTag(GKDCM_FileMetaInformationVersion));
				SetMediaStorageSOPClassUID(dataset->getTag(GKDCM_MediaStorageSOPClassUID));
				SetMediaStorageSOPInstanceUID(dataset->getTag(GKDCM_MediaStorageSOPInstanceUID));
				SetTransferSyntaxUID(dataset->getTag(GKDCM_TransferSyntaxUID));
				SetImplementationClassUID(dataset->getTag(GKDCM_ImplementationClassUID));
				SetImplementationVersionName(dataset->getTag(GKDCM_ImplementationVersionName));
				SetSourceApplicationEntityTitle(dataset->getTag(GKDCM_SourceApplicationEntityTitle));
			}

			/// FileMetaInformationGroupLength
			PropertyBuilderByName(std::string, FileMetaInformationGroupLength);
			/// FileMetaInformationVersion
			PropertyBuilderByName(std::string, FileMetaInformationVersion);
			/// MediaStorageSOPClassUID
			PropertyBuilderByName(std::string, MediaStorageSOPClassUID);
			/// MediaStorageSOPInstanceUID
			PropertyBuilderByName(std::string, MediaStorageSOPInstanceUID);
			/// TransferSyntaxUID
			PropertyBuilderByName(std::string, TransferSyntaxUID);
			/// ImplementationClassUID
			PropertyBuilderByName(std::string, ImplementationClassUID);
			/// ImplementationVersionName
			PropertyBuilderByName(std::string, ImplementationVersionName);
			/// SourceApplicationEntityTitle
			PropertyBuilderByName(std::string, SourceApplicationEntityTitle);

		private:

		};

	}
}