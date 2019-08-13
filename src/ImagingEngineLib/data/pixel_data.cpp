/*=========================================================================

  Program:   ImagingEngine
  Module:    pixel_data.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "data/pixel_data.h"
#include "tools/vtk_image_data_creator.h"

using namespace DW::IMAGE;

// Macro for a single case of the SWITCH_SCALAR_TYPE macro.
#define SWITCH_SCALAR_TYPE_CASE(vtkTypeName, typeName, code) case VTK_ ## vtkTypeName: \
{                                                                                  \
	typedef typeName ScalarType;                                                   \
	code;                                                                          \
}                                                                                  \
	break;

// This macro implements a switch to handle the different scalar types of pixel data.
// For each supported scalar type, the type is typedefed as ScalarType and code is run.
// For the default case, meaning an unsupported scalar type, defaultCode is run.
#define SWITCH_SCALAR_TYPE(code, defaultCode) switch (scalar_type_)  \
{                                                                    \
	SWITCH_SCALAR_TYPE_CASE(CHAR, char, code)                        \
	SWITCH_SCALAR_TYPE_CASE(SIGNED_CHAR, signed char, code)          \
	SWITCH_SCALAR_TYPE_CASE(UNSIGNED_CHAR, unsigned char, code)      \
	SWITCH_SCALAR_TYPE_CASE(SHORT, short, code)                      \
	SWITCH_SCALAR_TYPE_CASE(UNSIGNED_SHORT, unsigned short, code)    \
	SWITCH_SCALAR_TYPE_CASE(INT, int, code)                          \
	SWITCH_SCALAR_TYPE_CASE(UNSIGNED_INT, unsigned int, code)        \
	SWITCH_SCALAR_TYPE_CASE(LONG, long, code)                        \
	SWITCH_SCALAR_TYPE_CASE(UNSIGNED_LONG, unsigned long, code)      \
	SWITCH_SCALAR_TYPE_CASE(FLOAT, float, code)                      \
	SWITCH_SCALAR_TYPE_CASE(DOUBLE, double, code)                    \
}


RawPixelData::RawPixelData(char* data)
{
	series_data_ = data;
	modality_lut_applied_ = true;
}

RawPixelData::~RawPixelData()
{
	if (series_data_){
		delete [] series_data_;
		series_data_ = NULL;
	}
	//TODO 以vtkSmartPointer<T> obj... = vtkSmartPointer<T>::New()的形式创建的vtk对象，不用手动调用Delete
	// 参考：https://blog.csdn.net/zhc_24/article/details/78664410
	//if (vtk_series_data_){
	//	vtk_series_data_->Delete();
	//}
}

char* RawPixelData::GetPixelData()
{
	return series_data_;
}

vtkImageData* RawPixelData::GetVtkImageData()
{
	if (vtk_series_data_ == NULL){
		//TODO 通过Importer创建vtkImageData
		short* pixel_data = reinterpret_cast<short *>( this->GetPixelData() );
		// 转换到vtkImageData
		int dim[3] = { 0 };
		double origin[3] = { 0.0 }, spacing[3] = { 0.0 };
		this->GetDimensions(dim);
		this->GetOrigin(origin);
		this->GetSpacing(spacing);

		// 并不需要转换？？ [7/6/2019 Modified by zhangjian]
		//// VTK数据的Y轴是反着的，所以此处需要转换一下
		//short *raw_data_for_vtk = new short [dim[0] * dim[1] * dim[2]];
		//int  x, y, z, nz;
		//nz = dim[2];
		//int plane_size = dim[0] * dim[1];
		//for (z=0; z<nz; ++z){
		//	for (y=0; y<dim[1]; ++y){
		//		for (x=0; x<dim[0]; ++x){
		//			raw_data_for_vtk[z*plane_size + y*dim[0] + x] = pixel_data[z*plane_size + (dim[1]-1 - y) * dim[0] + x];
		//		}
		//	}
		//}

		VtkImageDataCreator imageDataCreator;
		imageDataCreator.SetOrigin(origin);
		imageDataCreator.SetSpacing(spacing);
		imageDataCreator.SetDimensions(dim);
		imageDataCreator.SetNumberOfComponents(1);	//bytes_per_pixel_
		vtk_series_data_ = imageDataCreator.Create(pixel_data);

		//delete [] raw_data_for_vtk;
	}
		
	return vtk_series_data_;
}

void *RawPixelData::GetDataPointer(int x, int y, int z)
{
	if (this->Dimensions &&
		x < this->Dimensions[0] &&
		y < this->Dimensions[1] &&
		z < this->Dimensions[2])
	{
		return series_data_ + (x + y * Dimensions[0] + z * Dimensions[0] * Dimensions[1]) * bytes_per_pixel_;
	}
	return NULL;
}

VtkPixelData::VtkPixelData(vtkImageData* data)
	: data_pointer_(data->GetScalarPointer())
	, scalar_type_(data->GetScalarType())
{
	vtk_series_data_ = data;
	modality_lut_applied_ = true;
}

VtkPixelData::~VtkPixelData()
{
	if (vtk_series_data_){
		vtk_series_data_->Delete();
	}
}

vtkImageData* VtkPixelData::GetVtkImageData()
{
	return vtk_series_data_;
}

void *VtkPixelData::GetDataPointer(int x, int y, int z)
{
	// 使用data_pointer_可以降低50%以上的时间消耗
	if (data_pointer_){
		int pos = z * (Dimensions[0] * Dimensions[1]) + (Dimensions[1] - 1 - y) * Dimensions[0] + x;
		return static_cast<char*>(data_pointer_) + pos * ScalarSize();
	}

	if (this->Dimensions &&
		x < this->Dimensions[0] &&
		y < this->Dimensions[1] &&
		z < this->Dimensions[2])
	{
		return vtk_series_data_->GetScalarPointer(x, Dimensions[1] - 1 - y, z);
	}
	return NULL;
}

size_t VtkPixelData::ScalarSize() const
{
	SWITCH_SCALAR_TYPE(return sizeof(ScalarType),
		return 0)
}