/****************************************************************************************************

 * \file dicom_controller.h
 * \date 2019/06/10 14:20
 *
 * \author jiayf
 * Contact: @deepwise.com
 *
 * \brief  

 Copyright(c) 2018-2030 DeepWise Corporation

****************************************************************************************************/

#pragma once
// #include "series_data_info.h"

#include "io/dcm_reader.h"
#include "data/vol_data.h"
#include "series_data_info.h"

#include <vector>


using namespace DW::IMAGE;

namespace DW {
	namespace IO {
		class DcmtkDcmLoader : public IDicomReader
		{
		public:
			DcmtkDcmLoader();
			virtual ~DcmtkDcmLoader();

			bool LoadDirectory(const char* dir) override;
			bool LoadFiles(std::vector<const char*> files) override;
			bool LoadDicomData(const char* dir) override;
			bool LoadVolumeMask(const char* file) override ;
			VolData* GetData() override;
			void Close() override;

		private:
			void ConvertStoredValue2CTValue(unsigned char *data, unsigned long length, DicomParas &paras);

		private:
			VolData* volume_data_;
		};
	}
}