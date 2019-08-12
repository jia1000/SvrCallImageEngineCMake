/*=========================================================================

  Program:   ImagingEngine
  Module:    dicom_writer_dcmtk.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "dicom_writer_dcmtk.h"

namespace GIL {
	namespace DICOM {

		DICOMWriterDcmtk::DICOMWriterDcmtk()
		{
			dicom_file_ = new DcmFileFormat();
		}

		DICOMWriterDcmtk::~DICOMWriterDcmtk()
		{
			if (dicom_file_){
				delete dicom_file_;
				dicom_file_ = NULL;
			}
		}

		void DICOMWriterDcmtk::SetPath(std::string path)
		{
			output_path_ = path;
		}

		std::string DICOMWriterDcmtk::GetPath()
		{
			return output_path_;
		}

		void DICOMWriterDcmtk::AddEmptyItem(const DcmTag &tag)
		{
			dicom_file_->getDataset()->insertEmptyElement(tag, true);
		}

		void DICOMWriterDcmtk::AddItem(DcmElement *attribute)
		{
			dicom_file_->getDataset()->insert(attribute, true);
		}

		bool DICOMWriterDcmtk::AddSequenceItem(const DcmTag &tag, DcmItem *attribute)
		{
			OFCondition result = dicom_file_->getDataset()->insertSequenceItem(tag, attribute);
			return result.good();
		}

		bool DICOMWriterDcmtk::write()
		{
			return false;
		}

	}
}