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
		// 生成CT图像DICOM文件的调用顺序 CPR/MPR
		// 1.创建CTImageDcmGenerator对象，构造函数接收一个包含原始信息DicomDataset对象
		// 2.根据需要调用SetTag方法修改目标DICOM文件的Tag值
		// 3.调用Write方法，传入生成的文件路径
		///
		class CTImageDcmGenerator : public IDcmGenerator
		{
		public:
			CTImageDcmGenerator(DicomDataset *dateset);
			~CTImageDcmGenerator();

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