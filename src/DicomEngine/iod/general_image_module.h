/*=========================================================================

  Program:   ImagingEngine
  Module:    general_image_module.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "module.h"

using namespace std;

namespace GIL {
	namespace DICOM {

		class GeneralImageModule : public IODModule
		{
		public:
			//GeneralImageModule();
			//~GeneralImageModule();

			void Write(DcmItem *dataset)
			{
				dataset->putAndInsertString(ToDcmTag(DCM_ImageType), GetImageType().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_AcquisitionDate), GetAcquisitionDate().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_ContentDate), GetContentDate().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_AcquisitionDateTime), GetAcquisitionDateTime().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_AcquisitionTime), GetAcquisitionTime().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_ContentTime), GetContentTime().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_DerivationDescription), GetDerivationDescription().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_InstanceNumber), GetInstanceNumber().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PatientOrientation), GetPatientOrientation().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_ImagesInAcquisition), GetImagesInAcquisition().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_ImageComments), GetImageComments().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_LossyImageCompression), GetLossyImageCompression().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_LossyImageCompressionRatio), GetLossyImageCompressionRatio().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PresentationLUTShape), GetPresentationLUTShape().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_BurnedInAnnotation), GetBurnedInAnnotation().c_str());
			}
			void Read(DicomDataset *dataset)
			{
				SetImageType(dataset->getTag(GKDCM_ImageType));
				SetAcquisitionDate(dataset->getTag(GKDCM_AcquisitionDate));
				SetContentDate(dataset->getTag(GKDCM_ContentDate));
				SetAcquisitionDateTime(dataset->getTag(GKDCM_AcquisitionDateTime));
				SetAcquisitionTime(dataset->getTag(GKDCM_AcquisitionTime));
				SetContentTime(dataset->getTag(GKDCM_ContentTime));
				SetDerivationDescription(dataset->getTag(GKDCM_DerivationDescription));
				SetPatientOrientation(dataset->getTag(GKDCM_PatientOrientation));
				SetInstanceNumber(dataset->getTag(GKDCM_InstanceNumber));
				SetImagesInAcquisition(dataset->getTag(GKDCM_ImagesInAcquisition));
				SetImageComments(dataset->getTag(GKDCM_ImageComments));
				SetLossyImageCompression(dataset->getTag(GKDCM_LossyImageCompression));
				SetLossyImageCompressionRatio(dataset->getTag(GKDCM_LossyImageCompressionRatio));
				SetPresentationLUTShape(dataset->getTag(GKDCM_PresentationLUTShape));
				SetBurnedInAnnotation(dataset->getTag(GKDCM_BurnedInAnnotation));
			}

			/// ImageType
			PropertyBuilderByName(std::string, ImageType);
			/// AcquisitionDate
			PropertyBuilderByName(std::string, AcquisitionDate);
			/// ContentDate
			PropertyBuilderByName(std::string, ContentDate);
			/// AcquisitionDateTime
			PropertyBuilderByName(std::string, AcquisitionDateTime);
			/// AcquisitionTime
			PropertyBuilderByName(std::string, AcquisitionTime);
			/// ContentTime
			PropertyBuilderByName(std::string, ContentTime);
			/// DerivationDescription
			PropertyBuilderByName(std::string, DerivationDescription);
			/// PatientOrientation
			PropertyBuilderByName(std::string, PatientOrientation);
			/// InstanceNumber
			PropertyBuilderByName(std::string, InstanceNumber);
			/// ImagesInAcquisition
			PropertyBuilderByName(std::string, ImagesInAcquisition);
			/// ImageComments
			PropertyBuilderByName(std::string, ImageComments);
			/// LossyImageCompression
			PropertyBuilderByName(std::string, LossyImageCompression);
			/// LossyImageCompressionRatio
			PropertyBuilderByName(std::string, LossyImageCompressionRatio);
			/// PresentationLUTShape
			PropertyBuilderByName(std::string, PresentationLUTShape);
			/// BurnedInAnnotation
			PropertyBuilderByName(std::string, BurnedInAnnotation);

		private:

		};

	}
}