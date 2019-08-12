/*=========================================================================

  Program:   ImagingEngine
  Module:    igenerator.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "igenerator.h"

namespace GIL {
	namespace DICOM {

		IDcmGenerator::IDcmGenerator(DicomDataset *dataset)
		{
			dataset_ = dataset;
			dicom_file_ = new DcmFileFormat();
			meta_info_module_ = new MetaInfoModule();
		}

		IDcmGenerator::~IDcmGenerator()
		{
			if (dicom_file_){
				delete dicom_file_;
				dicom_file_ = NULL;
			}
		}

		void IDcmGenerator::SetTag(const std::string &tag, const std::string &value)
		{
			dicom_file_->getDataset()->putAndInsertString(ToDcmTag(tag), value.c_str());
		}

		void IDcmGenerator::SetTag(const std::string &tag, const double &value)
		{
			dicom_file_->getDataset()->putAndInsertFloat64(ToDcmTag(tag), value);
		}

		void IDcmGenerator::SetTag(const std::string &tag, const int &value)
		{
			dicom_file_->getDataset()->putAndInsertUint32(ToDcmTag(tag), value);
		}

		void IDcmGenerator::SetTag(const DcmTagKey &tag_key, const std::string &value)
		{
			dicom_file_->getDataset()->putAndInsertString(ToDcmTag(tag_key), value.c_str());
		}

		void IDcmGenerator::SetTag(const DcmTagKey &tag_key, const double &value)
		{
			dicom_file_->getDataset()->putAndInsertFloat64(ToDcmTag(tag_key), value);
		}

		void IDcmGenerator::SetTag(const DcmTagKey &tag_key, const int &value)
		{
			dicom_file_->getDataset()->putAndInsertUint16(ToDcmTag(tag_key), value);
		}

		
		OFCondition IDcmGenerator::InsertEncapsulatedPixelData(DcmDataset* dset,
			char *pixData,
			Uint32 length,
			const E_TransferSyntax& outputTS) const
		{
			OFCondition cond;

			// create initial pixel sequence
			DcmPixelSequence* pixelSequence = new DcmPixelSequence(DCM_PixelSequenceTag);
			if (pixelSequence == NULL)
				return EC_MemoryExhausted;

			// insert empty offset table into sequence
			DcmPixelItem *offsetTable = new DcmPixelItem(DCM_PixelItemTag);
			if (offsetTable == NULL)
			{
				delete pixelSequence; pixelSequence = NULL;
				return EC_MemoryExhausted;
			}
			cond = pixelSequence->insert(offsetTable);
			if (cond.bad())
			{
				delete offsetTable; offsetTable = NULL;
				delete pixelSequence; pixelSequence = NULL;
				return cond;
			}

			// store compressed frame into pixel sequence
			DcmOffsetList dummyList;
			cond = pixelSequence->storeCompressedFrame(dummyList, OFreinterpret_cast(Uint8*,pixData), length, 0);
			// storeCompressedFrame(..) does a deep copy, so the pixData memory can be freed now
			delete[] pixData;
			if (cond.bad())
			{
				delete pixelSequence; pixelSequence = NULL;
				return cond;
			}

			// insert pixel data attribute incorporating pixel sequence into dataset
			DcmPixelData *pixelData = new DcmPixelData(DCM_PixelData);
			if (pixelData == NULL)
			{
				delete pixelSequence; pixelSequence = NULL;
				return EC_MemoryExhausted;
			}
			/* tell pixel data element that this is the original presentation of the pixel data
			* pixel data and how it compressed
			*/
			pixelData->putOriginalRepresentation(outputTS, NULL, pixelSequence);
			cond = dset->insert(pixelData);
			if (cond.bad())
			{
				delete pixelData; pixelData = NULL; // also deletes contained pixel sequence
				return cond;
			}

			return EC_Normal;
		}


		OFCondition IDcmGenerator::ReadAndInsertPixelData(DcmDataset* dset,
			IGeneratePixelData *img_source,
			E_TransferSyntax& outputTS)
		{
			return EC_Normal;
		}

		void IDcmGenerator::SetPixelData(IGeneratePixelData *pixel_data)
		{
			E_TransferSyntax syntax = E_TransferSyntax::EXS_LittleEndianExplicit;
			OFCondition result = ReadAndInsertPixelData(dicom_file_->getDataset(), pixel_data, syntax);
			if (result.good()){

			}
		}
	}
}