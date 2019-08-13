/*=========================================================================

  Program:   ImagingEngine
  Module:    nii_loader.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "io/nii_loader.h"
#include "data/vol_data.h"
#include <vtkStringArray.h>
#include <vtkStdString.h>
#include <vtkNIFTIImageReader.h>
#include "data/pixel_data.h"
#include "tools/vtk_image_data_creator.h"

using namespace DW::IMAGE;
using namespace DW::IO;

NiiImageLoader::NiiImageLoader()
{
	volume_data_ = new VolData();
}

NiiImageLoader::~NiiImageLoader()
{
	Close();
}

bool NiiImageLoader::LoadDirectory(const char* dir)
{
	try
	{
		// initialize dicom image reader
		vtkSmartPointer<vtkDICOMImageReader> vtk_image_reader_ = vtkSmartPointer<vtkDICOMImageReader>::New();
		// 1. Read all the DICOM files in the specified directory
		vtk_image_reader_->SetDirectoryName(dir);
		vtk_image_reader_->Update();
		// 2. Set it to vtkImageData object
		//将图像数据缓存到vtk_image_data_指针里
		vtk_image_data_ = vtk_image_reader_->GetOutput();
		double *spacing = vtk_image_reader_->GetPixelSpacing();
		int width = vtk_image_reader_->GetWidth();
		int height = vtk_image_reader_->GetHeight();
		int bits = vtk_image_reader_->GetBitsAllocated();

		int dims[3];
		vtk_image_data_->GetDimensions(dims);

		int nx, ny, nz;
		float dx, dy, dz;
		int nvox;

		nx = dims[0], ny = dims[1], nz = dims[2];
		nvox = nx * ny * nz;

		double spacings[3];
		vtk_image_data_->GetSpacing(spacings);


		// sort by image position from big to small
		int size = width * height * nz;
		short *raw_data = new short[size];
		const char * tt = vtk_image_data_->GetScalarTypeAsString();
		short *vtk_data = reinterpret_cast<short *>(vtk_image_data_->GetScalarPointer());
		int x, y, z, i;
		short min_val = -1024;
		int plane_size = width * height;
		for (z=0; z<nz; ++z){
			for (y=0; y<height; ++y){
				for (x=0; x<width; ++x){
					raw_data[z*plane_size + y*width + x] = vtk_data[z*plane_size + (height-1 - y) * width + x];
				}
				//memcpy(raw_data + z*plane_size + y*width, vtk_data + z*plane_size + (height-1 - y) * width, width);
			}
		}

		// 3. Convert to VolData object
		volume_data_ = new VolData();
		volume_data_->SetBitsPerPixel(16);
		volume_data_->SetBitsStored(16);
		//volume_data_->SetPixelData(new VtkPixelData(vtk_image_data_));
		volume_data_->SetPixelData(new RawPixelData(reinterpret_cast<char *>(raw_data)));

		volume_data_->GetPixelData()->SetBitsPerPixel(16);
		volume_data_->GetPixelData()->SetDimensions(nx, ny, nz);
		volume_data_->GetPixelData()->SetOrigin(0, 0, 0);
		volume_data_->GetPixelData()->SetSpacing(0.3671875, 0.3671875, 0.45);
		volume_data_->SetSliceWidth(nx);
		volume_data_->SetSliceHeight(ny);
		volume_data_->SetSliceCount(nz);
		volume_data_->SetSliceFOVWidth(460.00f);
		volume_data_->SetSliceFOVHeight(460.00f);
		volume_data_->SetStudyInstanceUID("1.2.840.113619.2.278.3.679457.166.1526860577.400");
		volume_data_->SetSeriesInstanceUID("1.2.840.113619.2.278.3.679457.166.1526860577.404.3");
		volume_data_->SetBoundingBox(0, 0, 0, nx - 1, ny - 1, nz - 1);
		volume_data_->SetDefaultWindowWidthLevel(2000, 400);
		return true;
	}
	catch (...)
	{
		return false;
	}
}


bool NiiImageLoader::LoadFiles(vector<const char*> files)
{
	if (files.size() == 0) return false;
	
	// 0. Create a vtkStringArray object with file paths
	const char *file_path = files[0];

	vtkSmartPointer<vtkNIFTIImageReader> niiReader = vtkSmartPointer<vtkNIFTIImageReader>::New();
	niiReader->SetFileName(file_path);
	niiReader->Update();

	// 1.0 Read image data from nii file
	vtkSmartPointer<vtkImageData> image = niiReader->GetOutput();
	//vtk_image_data_ = image;
	int dims[3];
	image->GetDimensions(dims);

	int nx, ny, nz;
	float dx, dy, dz;
	int nvox;

	nx = dims[0], ny = dims[1], nz = dims[2];
	nvox = nx * ny * nz;

	double spacings[3];
	image->GetSpacing(spacings);
	dx = spacings[0], dy = spacings[1], dz = spacings[2];
	
	//////////////////////////////////////////////////////////////////////////
	/// render volume error
	int width = nx;
	int height = ny;
	// sort by image position from big to small
	int size = width * height * nz;
	short *raw_data = new short[size];
	const char * tt = image->GetScalarTypeAsString();
	short *vtk_data = reinterpret_cast<short *>(image->GetScalarPointer());
	int x, y, z, i;
	short min_val = -1024;

	int plane_size = width * height;
	for (z=0; z<nz; ++z){
		//memcpy(raw_data + z*plane_size, vtk_data + (nz - 1 - z)*plane_size, plane_size);
		for (y=0; y<height; ++y){
			for (x=0; x<width; ++x){
				raw_data[z*plane_size + y*width + x] = vtk_data[(nz-1 - z)*plane_size + (height-1 - y) * width + x];
			}
		}
	}

	// 3. Convert to VolData object
	volume_data_->SetBitsPerPixel(16);
	volume_data_->SetBitsStored(16);
	//volume_data_->SetPixelData(new VtkPixelData(image));
	//volume_data_->SetPixelData(new VtkPixelData(vtk_image_data_));
	volume_data_->SetPixelData(new RawPixelData(reinterpret_cast<char *>(raw_data)));

	volume_data_->GetPixelData()->SetBitsPerPixel(16);
	volume_data_->GetPixelData()->SetDimensions(nx, ny, nz);
	volume_data_->GetPixelData()->SetOrigin(0, 0, 0);
	volume_data_->GetPixelData()->SetSpacing(spacings[0], spacings[1], spacings[2]);
	volume_data_->SetSliceCount(nz);
	volume_data_->SetSliceFOVWidth(460.00f);
	volume_data_->SetSliceFOVHeight(460.00f);
	volume_data_->SetStudyInstanceUID("1.2.840.113619.2.278.3.679457.166.1526860577.400");
	volume_data_->SetSeriesInstanceUID("1.2.840.113619.2.278.3.679457.166.1526860577.404.3");
	volume_data_->SetBoundingBox(0,0,0,nx-1,ny-1,nz-1);
	volume_data_->SetDefaultWindowWidthLevel(820, 250);
	return true;
}

bool NiiImageLoader::LoadDicomData(const char* path)
{
	// 0. Create a vtkStringArray object with file paths
	const char *file_path = path;

	vtkSmartPointer<vtkNIFTIImageReader> niiReader = vtkSmartPointer<vtkNIFTIImageReader>::New();
	niiReader->SetFileName(file_path);
	niiReader->Update();

	// 1.0 Read image data from nii file
	vtkSmartPointer<vtkImageData> image = niiReader->GetOutput();
	//vtk_image_data_ = image;
	int dims[3];
	image->GetDimensions(dims);

	int nx, ny, nz;
	float dx, dy, dz;
	int nvox;

	nx = dims[0], ny = dims[1], nz = dims[2];
	nvox = nx * ny * nz;

	double spacings[3];
	image->GetSpacing(spacings);
	dx = spacings[0], dy = spacings[1], dz = spacings[2];
	
	// 3. Convert to VolData object
	volume_data_->SetBitsPerPixel(16);
	volume_data_->SetBitsStored(16);
	volume_data_->SetPixelData(new VtkPixelData(image));
	volume_data_->GetPixelData()->SetBitsPerPixel(16);
	volume_data_->GetPixelData()->SetDimensions(nx, ny, nz);
	volume_data_->GetPixelData()->SetOrigin(0, 0, 0);
	volume_data_->GetPixelData()->SetSpacing(spacings[0], spacings[1], spacings[2]);
	volume_data_->SetSliceCount(nz);
	volume_data_->SetSliceFOVWidth(460.00f);
	volume_data_->SetSliceFOVHeight(460.00f);
	volume_data_->SetStudyInstanceUID("1.2.840.113619.2.278.3.679457.166.1526860577.400");
	volume_data_->SetSeriesInstanceUID("1.2.840.113619.2.278.3.679457.166.1526860577.404.3");
	volume_data_->SetBoundingBox(0,0,0,nx-1,ny-1,nz-1);
	volume_data_->SetDefaultWindowWidthLevel(413, 82);
	return true;
}

bool NiiImageLoader::LoadVolumeMask(const char* file)
{
	vtkSmartPointer<vtkNIFTIImageReader> niiReader = vtkSmartPointer<vtkNIFTIImageReader>::New();
	niiReader->SetFileName(file);
	niiReader->Update();

	int type = niiReader->GetDataScalarType();
	int com = niiReader->GetNumberOfScalarComponents();

	// 1.0 Read image data from nii file
	vtkSmartPointer<vtkImageData> image = niiReader->GetOutput();
	
	// 2.0 Convert to unsigned char data pointer
	int dims[3];
	image->GetDimensions(dims);

	int nx, ny, nz;
	float dx, dy, dz;
	int nvox;

	nx = dims[0], ny = dims[1], nz = dims[2];
	nvox = nx * ny * nz;

	double spacings[3];
	image->GetSpacing(spacings);
	dx = spacings[0], dy = spacings[1], dz = spacings[2];
	UNITMARK3D *mask_data = new UNITMARK3D[nvox];

	char * image_buf = static_cast<char *>(image->GetScalarPointer());
	long count = 0;
	long count2 = 0;
	int x, y, z, i;
	// from head to feet
	int width = nx;
	int height = ny;
	int x_min = 10000, x_max = 0, y_min = 10000, y_max = 0, z_min = 10000, z_max = 0, val = 0;
	int x_min_clipping = 10000, x_max_clipping = 0, y_min_clipping = 10000, y_max_clipping = 0, z_min_clipping = 10000, z_max_clipping = 0;
	
	int max_nz_clipping = nz * 0.35 + 1;
	int plane_size = width * height;
	for (z=0; z<nz; ++z){
		for (y=0; y<height; ++y){
			for (x=0; x<width; ++x){
				
				// VolumeMask 不需要y轴坐标转换，因为Mask后续还会再做一次转换，与其这里就不做转换了。
				/*mask_data[z*plane_size + y*width + x] = image_buf[(nz-1 - z)*plane_size + (height-1 - y) * width + x];*/
				mask_data[z*plane_size + y*width + x] = image_buf[(nz-1 - z)*plane_size + y* width + x];

				val =  mask_data[z*plane_size + y*width + x];
				if (val > 0){
					if (x_min > x){
						x_min = x;
					}
					if (x_max < x){
						x_max = x;
					}
					if (y_min > y){
						y_min = y;
					}
					if (y_max < y){
						y_max = y;
					}
					if (z_min > z){
						z_min = z;
					}
					if (z_max < z){
						z_max = z;
					}

					if (z < max_nz_clipping){
						if (x_min_clipping > x){
							x_min_clipping = x;
						}
						if (x_max_clipping < x){
							x_max_clipping = x;
						}
						if (y_min_clipping > y){
							y_min_clipping = y;
						}
						if (y_max_clipping < y){
							y_max_clipping = y;
						}
						if (z_min_clipping > z){
							z_min_clipping = z;
						}
						if (z_max_clipping < z){
							z_max_clipping = z;
						}
					}
				}
			}
		}
	}


	volume_data_->SetMark(mask_data);
	volume_data_->SetMarkBoundingBox(x_min, y_min, z_min, x_max, y_max, z_max);
	volume_data_->SetMarkBoundingBoxClipping(x_min_clipping, y_min_clipping, z_min_clipping, x_max_clipping, y_max_clipping, z_max_clipping);

	return true;
}

VolData* NiiImageLoader::GetData()
{
	return volume_data_;
}

void NiiImageLoader::Close()
{

}