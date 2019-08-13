/*=========================================================================

  Program:   ImagingEngine
  Module:    nii_loader.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data/data_definition.h"
#include "data/vol_data.h"
#include "io/dcm_reader.h"

using namespace std;

namespace DW {
	namespace IO {

		class NiiImageLoader : public IDicomReader
		{
		public:
			NiiImageLoader();
			virtual ~NiiImageLoader();

			/// IDicomReader implementation
			bool LoadDirectory(const char* dir) override;
			bool LoadFiles(vector<const char*> files) override;
			bool LoadDicomData(const char* dir) override;
			bool LoadVolumeMask(const char* file) override;
			VolData* GetData() override;
			void Close() override;

		protected: 
			DataConvertUtil* data_convret_;
			VolData* volume_data_;
			vtkSmartPointer<vtkImageData> vtk_image_data_;

		};

		class NiiImageLoader2 : NiiImageLoader
		{
		public:
			NiiImageLoader2();
			~NiiImageLoader2();

			bool LoadFiles(vector<const char*> files) override;
			
		};

	}
}
