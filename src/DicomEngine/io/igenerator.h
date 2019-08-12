/*=========================================================================

  Program:   ImagingEngine
  Module:    igenerator.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "stdlib.h"
#include <iostream>
#include <vector>
#include "idicom_writer.h"
#include "../iod/module.h"
#include "../iod/meta_info_module.h"
#include "../api/dicom/dicomdataset.h"
#include "img_source.h"
#include "dcmtk/ofstd/ofcond.h"

namespace GIL {
	namespace DICOM {

		class IDcmGenerator
		{
		public:
			IDcmGenerator(DicomDataset *dataset);
			virtual ~IDcmGenerator();
			/// 更新DicomTag的值
			virtual void SetTag(const std::string &tag, const std::string &value);
			virtual void SetTag(const std::string &tag, const double &value);
			virtual void SetTag(const std::string &tag, const int &value);
			virtual void SetTag(const DcmTagKey &tag_key, const std::string &value);
			virtual void SetTag(const DcmTagKey &tag_key, const double &value);
			virtual void SetTag(const DcmTagKey &tag_key, const int &value);
			/// 生成文件
			virtual int Write(std::string &path) = 0;
			/// 设置PixelData
			virtual void SetPixelData(IGeneratePixelData *pixel_data);

		protected:
			virtual DcmTag ToDcmTag(const string &tag_string)
			{
				DcmTag tag(0x0008, 0x0001);
				return tag;
			}
			virtual DcmTag ToDcmTag(const DcmTagKey &tag_key)
			{
				DcmTag tag(tag_key);
				return tag;
			}
			OFCondition InsertEncapsulatedPixelData(
				DcmDataset* dset,
				char *pixData,
				Uint32 length,
				const E_TransferSyntax& outputTS) const;

			virtual OFCondition ReadAndInsertPixelData(
				DcmDataset* dset,
				IGeneratePixelData *img_source,
				E_TransferSyntax& outputTS);

		protected:
			MetaInfoModule *meta_info_module_;
			vector<IODModule *> module_list_;
			DicomDataset *dataset_;
			DcmFileFormat *dicom_file_;

		};

	}
}