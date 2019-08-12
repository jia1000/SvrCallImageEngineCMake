/*=========================================================================

  Program:   ImagingEngine
  Module:    ct_image_module.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "module.h"

using namespace std;

namespace GIL {
	namespace DICOM {

		class CTImageModule : public IODModule
		{
		public:
			//CTImageModule();
			//~CTImageModule();

			void Write(DcmItem *dataset)
			{
				dataset->putAndInsertString(ToDcmTag(DCM_ImageType), GetImageType().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_ScanOptions), GetScanOptions().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_KVP), GetKVP().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_DataCollectionDiameter), GetDataCollectionDiameter().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_ReconstructionDiameter), GetReconstructionDiameter().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_DistanceSourceToDetector), GetDistanceSourceToDetector().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_DistanceSourceToPatient), GetDistanceSourceToPatient().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_GantryDetectorTilt), GetGantryDetectorTilt().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_TableHeight), GetTableHeight().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_RotationDirection), GetRotationDirection().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_ExposureTime), GetExposureTime().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_XRayTubeCurrent), GetXRayTubeCurrent().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_Exposure), GetExposure().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_ExposureInuAs), GetExposureInuAs().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_FilterType), GetFilterType().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_GeneratorPower), GetGeneratorPower().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_FocalSpots), GetFocalSpots().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_ConvolutionKernel), GetConvolutionKernel().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_RevolutionTime), GetRevolutionTime().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_SingleCollimationWidth), GetSingleCollimationWidth().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_TotalCollimationWidth), GetTotalCollimationWidth().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_TableSpeed), GetTableSpeed().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_TableFeedPerRotation), GetTableFeedPerRotation().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_SpiralPitchFactor), GetSpiralPitchFactor().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_AcquisitionNumber), GetAcquisitionNumber().c_str());

				dataset->putAndInsertString(ToDcmTag(DCM_SamplesPerPixel), GetSamplesPerPixel().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_PhotometricInterpretation), GetPhotometricInterpretation().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_BitsAllocated), GetBitsAllocated().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_BitsStored), GetBitsStored().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_HighBit), GetHighBit().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_RescaleIntercept), GetRescaleIntercept().c_str());
				dataset->putAndInsertString(ToDcmTag(DCM_RescaleSlope), GetRescaleSlope().c_str());
			}
			void Read(DicomDataset *dataset)
			{
				SetImageType(dataset->getTag(GKDCM_ImageType));
				SetScanOptions(dataset->getTag(GKDCM_ScanOptions));
				SetKVP(dataset->getTag(GKDCM_KVP));
				SetDataCollectionDiameter(dataset->getTag(GKDCM_DataCollectionDiameter));
				SetReconstructionDiameter(dataset->getTag(GKDCM_ReconstructionDiameter));
				SetDistanceSourceToDetector(dataset->getTag(GKDCM_DistanceSourceToDetector));
				SetDistanceSourceToPatient(dataset->getTag(GKDCM_DistanceSourceToPatient));
				SetGantryDetectorTilt(dataset->getTag(GKDCM_GantryDetectorTilt));
				SetTableHeight(dataset->getTag(GKDCM_TableHeight));
				SetRotationDirection(dataset->getTag(GKDCM_RotationDirection));
				SetExposureTime(dataset->getTag(GKDCM_ExposureTime));
				SetXRayTubeCurrent(dataset->getTag(GKDCM_XRayTubeCurrent));
				SetExposure(dataset->getTag(GKDCM_Exposure));
				SetExposureInuAs(dataset->getTag(GKDCM_ExposureInuAs));
				SetFilterType(dataset->getTag(GKDCM_FilterType));
				SetGeneratorPower(dataset->getTag(GKDCM_GeneratorPower));
				SetFocalSpots(dataset->getTag(GKDCM_FocalSpots));
				SetConvolutionKernel(dataset->getTag(GKDCM_ConvolutionKernel));
				SetRevolutionTime(dataset->getTag(GKDCM_RevolutionTime));
				SetSingleCollimationWidth(dataset->getTag(GKDCM_SingleCollimationWidth));
				SetTotalCollimationWidth(dataset->getTag(GKDCM_TotalCollimationWidth));
				SetTableSpeed(dataset->getTag(GKDCM_TableSpeed));
				SetTableFeedPerRotation(dataset->getTag(GKDCM_TableFeedPerRotation));
				SetSpiralPitchFactor(dataset->getTag(GKDCM_SpiralPitchFactor));
				SetAcquisitionNumber(dataset->getTag(GKDCM_AcquisitionNumber));

				SetSamplesPerPixel(dataset->getTag(GKDCM_SamplesPerPixel));
				SetPhotometricInterpretation(dataset->getTag(GKDCM_PhotometricInterpretation));
				SetBitsAllocated(dataset->getTag(GKDCM_BitsAllocated));
				SetBitsStored(dataset->getTag(GKDCM_BitsStored));
				SetHighBit(dataset->getTag(GKDCM_HighBit));
				SetRescaleIntercept(dataset->getTag(GKDCM_RescaleIntercept));
				SetRescaleSlope(dataset->getTag(GKDCM_RescaleSlope));
			}

			/// ImageType
			PropertyBuilderByName(std::string, ImageType);
			/// ScanOptions
			PropertyBuilderByName(std::string, ScanOptions);
			/// KVP
			PropertyBuilderByName(std::string, KVP);
			/// DataCollectionDiameter
			PropertyBuilderByName(std::string, DataCollectionDiameter);
			/// ReconstructionDiameter
			PropertyBuilderByName(std::string, ReconstructionDiameter);
			/// DistanceSourceToDetector
			PropertyBuilderByName(std::string, DistanceSourceToDetector);
			/// DistanceSourceToPatient
			PropertyBuilderByName(std::string, DistanceSourceToPatient);
			/// GantryDetectorTilt
			PropertyBuilderByName(std::string, GantryDetectorTilt);
			/// TableHeight
			PropertyBuilderByName(std::string, TableHeight);
			/// RotationDirection
			PropertyBuilderByName(std::string, RotationDirection);
			/// ExposureTime
			PropertyBuilderByName(std::string, ExposureTime);
			/// XRayTubeCurrent
			PropertyBuilderByName(std::string, XRayTubeCurrent);
			/// Exposure
			PropertyBuilderByName(std::string, Exposure);
			/// ExposureInuAs
			PropertyBuilderByName(std::string, ExposureInuAs);
			/// FilterType
			PropertyBuilderByName(std::string, FilterType);
			/// GeneratorPower
			PropertyBuilderByName(std::string, GeneratorPower);
			/// FocalSpots
			PropertyBuilderByName(std::string, FocalSpots);
			/// ConvolutionKernel
			PropertyBuilderByName(std::string, ConvolutionKernel);
			/// RevolutionTime
			PropertyBuilderByName(std::string, RevolutionTime);
			/// SingleCollimationWidth
			PropertyBuilderByName(std::string, SingleCollimationWidth);
			/// TotalCollimationWidth
			PropertyBuilderByName(std::string, TotalCollimationWidth);
			/// TableSpeed
			PropertyBuilderByName(std::string, TableSpeed);
			/// TableFeedPerRotation
			PropertyBuilderByName(std::string, TableFeedPerRotation);
			/// SpiralPitchFactor
			PropertyBuilderByName(std::string, SpiralPitchFactor);
			/// AcquisitionNumber
			PropertyBuilderByName(std::string, AcquisitionNumber);

			/// SeriesDate
			PropertyBuilderByName(std::string, SamplesPerPixel);
			/// SeriesTime
			PropertyBuilderByName(std::string, PhotometricInterpretation);
			/// BitsAllocated
			PropertyBuilderByName(std::string, BitsAllocated);
			/// BitsStored
			PropertyBuilderByName(std::string, BitsStored);
			/// HighBit
			PropertyBuilderByName(std::string, HighBit);
			/// RescaleIntercept
			PropertyBuilderByName(std::string, RescaleIntercept);
			/// RescaleSlope
			PropertyBuilderByName(std::string, RescaleSlope);

		private:

		};

	}
}