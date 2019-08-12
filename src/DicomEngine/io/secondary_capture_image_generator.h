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
		// 生成二次获取图像DICOM文件的调用顺序 VR
		// 1.创建SecondaryCaptureImageDcmGenerator对象，构造函数接收一个包含原始信息DicomDataset对象
		// 2.根据需要调用SetTag方法修改目标DICOM文件的Tag值
		// 3.调用Write方法，传入生成的文件路径
		///
		class SecondaryCaptureImageDcmGenerator : public IDcmGenerator
		{
		public:
			SecondaryCaptureImageDcmGenerator(DicomDataset *dateset);
			~SecondaryCaptureImageDcmGenerator();

			/// 生成文件
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