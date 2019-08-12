/*=========================================================================

  Program:   ImagingEngine
  Module:    secondary_capture_image_generator.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "secondary_capture_image_generator.h"
#include "../iod/sc_image_module.h"
#include "../iod/sc_equipment_module.h"
#include "../iod/general_equipment_module.h"
#include "../iod/general_image_module.h"
#include "../iod/general_series_module.h"
#include "../iod/general_study_module.h"
#include "../iod/image_pixel_module.h"
#include "../iod/meta_info_module.h"
#include "../iod/patient_module.h"
#include "../iod/patient_study_module.h"
#include "../iod/sop_common_module.h"
#include "../iod/modality_lut_module.h"
#include "../iod/voi_lut_module.h"

namespace GIL {
	namespace DICOM {

		SecondaryCaptureImageDcmGenerator::SecondaryCaptureImageDcmGenerator(DicomDataset *dataset)
			: IDcmGenerator(dataset)
		{
			Initialize();
		}

		SecondaryCaptureImageDcmGenerator::~SecondaryCaptureImageDcmGenerator()
		{
			auto it = module_list_.begin();
			for(; it != module_list_.end(); ++it){
				delete *it;
			}
			module_list_.clear();
		}

		void SecondaryCaptureImageDcmGenerator::Initialize()
		{
			// Patient
			module_list_.push_back(new PatientModule());
			// Study
			module_list_.push_back(new GeneralStudyModule());
			module_list_.push_back(new PatientStudyModule());
			// Series
			module_list_.push_back(new GeneralSeriesModule());
			// Equipment
			module_list_.push_back(new GeneralEquipmentModule());
			module_list_.push_back(new SCEquipmentModule());
			// Image
			module_list_.push_back(new GeneralImageModule());
			module_list_.push_back(new ImagePixelModule());
			module_list_.push_back(new SCImageModule());
			module_list_.push_back(new ModalityLutModule());
			module_list_.push_back(new VoiLutModule());
			module_list_.push_back(new SOPCommonModule());

			// Reads tags from DicomDataset object to modules
			auto it = module_list_.begin();
			for(; it != module_list_.end(); ++it){
				(*it)->Read(dataset_);
			}

			// Writes tags to DcmDataSet from modules
			it = module_list_.begin();
			for(; it != module_list_.end(); ++it){
				(*it)->Write(dicom_file_->getDataset());
			}
		}

		int SecondaryCaptureImageDcmGenerator::Write(std::string &path)
		{
			OFString str_sopinstanceuid;
			dicom_file_->getDataset()->findAndGetOFString(DCM_SOPInstanceUID, str_sopinstanceuid);
			DcmMetaInfo *meta_info = dicom_file_->getMetaInfo();
			//meta_info_module_->SetFileMetaInformationVersion("01");
			meta_info_module_->SetImplementationClassUID("1.0.0.1.1");
			meta_info_module_->SetImplementationVersionName("DW ImagingEngine 1.0");
			meta_info_module_->SetSourceApplicationEntityTitle("DW DICOM");
			meta_info_module_->SetMediaStorageSOPClassUID("1.2.840.10008.5.1.4.1.1.7");
			meta_info_module_->SetMediaStorageSOPInstanceUID(str_sopinstanceuid.c_str());
			meta_info_module_->Write(meta_info);
			// SOP Class UID
			dicom_file_->getDataset()->putAndInsertString(ToDcmTag(DCM_SOPClassUID), "1.2.840.10008.5.1.4.1.1.7");
			// 写入文件需要指定传输语法
			OFCondition state = dicom_file_->saveFile(path.c_str(), EXS_LittleEndianExplicit);
			if (state.good()){
				return 1;
			}
			else{
				unsigned short code = state.code();
				return -code;
			}
		}

		OFCondition SecondaryCaptureImageDcmGenerator::ReadAndInsertPixelData(DcmDataset* dset,
			IGeneratePixelData *img_source,
			E_TransferSyntax& outputTS)
		{
			OFCondition cond;

			if (img_source == NULL)
				return EC_IllegalParameter;

			//TODO sc图像字段
			Uint16 samplesPerPixel, rows, cols, bitsAlloc, bitsStored, highBit, pixelRepr, planConf;
			Uint16 pixAspectH =1; Uint16 pixAspectV = 1;
			OFString photoMetrInt, rescaleIntercept, rescaleSlope, rescaleType;
			outputTS = EXS_Unknown;
			char* pixData = NULL;
			Uint32 length;

			samplesPerPixel = img_source->GetSamplesPerPixel();
			rows = img_source->GetHeight();
			cols = img_source->GetWidth();
			bitsAlloc = img_source->GetBitsAllocated();
			bitsStored = img_source->GetBitsStored();
			highBit = img_source->GetHighBit();
			pixelRepr = img_source->GetPixelRepresentation();
			planConf = img_source->GetPlanarConfiguration();
			photoMetrInt = img_source->GetPhotometricInterpretation();
			length = img_source->GetPixelLength();
			pixData = img_source->GetPixelData();
			rescaleIntercept = img_source->GetRescaleIntercept();
			rescaleSlope = img_source->GetRescaleSlope();
			rescaleType = img_source->GetRescaleType();

			//OFCondition cond = imgSource->readPixelData(rows, cols,
			//	samplesPerPixel, photoMetrInt, bitsAlloc, bitsStored, highBit, pixelRepr,
			//	planConf, pixAspectH, pixAspectV, pixData, length, outputTS);

			//if (cond.bad())
			//	return cond;

			DcmXfer transport(outputTS);
			if (transport.isEncapsulated())
				InsertEncapsulatedPixelData(dset, pixData, length, outputTS);
			else
			{
				/* Not encapsulated */
				dset->putAndInsertUint8Array(DCM_PixelData, OFreinterpret_cast(Uint8*, pixData), length);
				delete[] pixData;
			}

			cond = dset->putAndInsertUint16(DCM_SamplesPerPixel, samplesPerPixel);
			if (cond.bad())
				return cond;

			cond = dset->putAndInsertOFStringArray(DCM_PhotometricInterpretation, photoMetrInt);
			if (cond.bad())
				return cond;

			// Should only be written if Samples per Pixel > 1
			if (samplesPerPixel > 1)
			{
				cond = dset->putAndInsertUint16(DCM_PlanarConfiguration, planConf);
				if (cond.bad())
					return cond;
			}

			cond = dset->putAndInsertUint16(DCM_Rows, rows);
			if (cond.bad())
				return cond;

			cond = dset->putAndInsertUint16(DCM_Columns, cols);
			if (cond.bad())
				return cond;

			cond = dset->putAndInsertUint16(DCM_BitsAllocated, bitsAlloc);
			if (cond.bad())
				return cond;

			cond = dset->putAndInsertUint16(DCM_BitsStored, bitsStored);
			if (cond.bad())
				return cond;

			cond = dset->putAndInsertUint16(DCM_HighBit, highBit);
			if (cond.bad())
				return cond;

			cond = dset->putAndInsertString(DCM_RescaleIntercept, rescaleIntercept.c_str());
			if (cond.bad())
				return cond;

			cond = dset->putAndInsertString(DCM_RescaleSlope, rescaleSlope.c_str());
			if (cond.bad())
				return cond;

			cond = dset->putAndInsertString(DCM_RescaleType, rescaleType.c_str());
			if (cond.bad())
				return cond;

			//if ( pixAspectH != pixAspectV )
			//{
			//	char buf[200];
			//	int err = sprintf(buf, "%u\\%u", pixAspectV, pixAspectH);
			//	if (err == -1) return EC_IllegalCall;
			//	cond = dset->putAndInsertOFStringArray(DCM_PixelAspectRatio, buf);
			//	if (cond.bad())
			//		return cond;
			//}

			return dset->putAndInsertUint16(DCM_PixelRepresentation, pixelRepr);
		}
	}
}