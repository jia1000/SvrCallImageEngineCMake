/*=========================================================================

  Program:   ImagingEngine
  Module:    ct_image_storage_sop_generator.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once
#include "stdlib.h"
#include <iostream>
#include <vector>
#include "igenerator.h"

namespace GIL {
	namespace DICOM {

		///
		// ����CTͼ��DICOM�ļ��ĵ���˳�� CPR/MPR
		// 1.����CTImageDcmGenerator���󣬹��캯������һ������ԭʼ��ϢDicomDataset����
		// 2.������Ҫ����SetTag�����޸�Ŀ��DICOM�ļ���Tagֵ
		// 3.����Write�������������ɵ��ļ�·��
		///
		class CTImageDcmGenerator : public IDcmGenerator
		{
		public:
			CTImageDcmGenerator(DicomDataset *dateset);
			~CTImageDcmGenerator();

			/// �����ļ�
			int Write(std::string &path) override;

		protected:
			OFCondition ReadAndInsertPixelData(
				DcmDataset* dset,
				IGeneratePixelData *img_source,
				E_TransferSyntax& outputTS) override;

		private:
			void Initialize();

		};
		
	}
}