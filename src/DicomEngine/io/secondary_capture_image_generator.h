/*=========================================================================

  Program:   ImagingEngine
  Module:    secondary_capture_image_generator.h
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
		// ���ɶ��λ�ȡͼ��DICOM�ļ��ĵ���˳�� VR
		// 1.����SecondaryCaptureImageDcmGenerator���󣬹��캯������һ������ԭʼ��ϢDicomDataset����
		// 2.������Ҫ����SetTag�����޸�Ŀ��DICOM�ļ���Tagֵ
		// 3.����Write�������������ɵ��ļ�·��
		///
		class SecondaryCaptureImageDcmGenerator : public IDcmGenerator
		{
		public:
			SecondaryCaptureImageDcmGenerator(DicomDataset *dateset);
			~SecondaryCaptureImageDcmGenerator();

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