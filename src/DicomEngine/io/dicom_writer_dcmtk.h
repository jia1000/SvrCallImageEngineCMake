/*=========================================================================

  Program:   ImagingEngine
  Module:    dicom_writer_dcmtk.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once
#include "idicom_writer.h"

namespace GIL {
	namespace DICOM {

		class DICOMWriterDcmtk : public IDICOMWriter
		{
		public:
			DICOMWriterDcmtk();
			~DICOMWriterDcmtk();

			/// Sets output path
			void SetPath(std::string path) override;
			/// Gets output path
			std::string GetPath() override;
			/// Adds a dicom item
			void AddItem(DcmElement *attribute) override;
			/// Adds a empty dicom item
			void AddEmptyItem(const DcmTag &tag) override;
			/// Adds a dicom sequence
			bool AddSequenceItem(const DcmTag &tag, DcmItem *attribute) override;
			/// Writes to file
			bool write() override;

		private:
			std::string output_path_;
			DcmFileFormat *dicom_file_;

		};

	}
}
