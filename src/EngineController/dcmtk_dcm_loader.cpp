
#include "data_transfer_control.h"

#include <vtkNIFTIImageReader.h>

using namespace DW::IMAGE;
using namespace DW::IO;

DcmtkDcmLoader::DcmtkDcmLoader()
 : volume_data_(NULL)
{
    
}

DcmtkDcmLoader::~DcmtkDcmLoader()
{
	    
}

bool DcmtkDcmLoader::LoadDirectory(const char* dir) 
{   
	CGLogger::Debug("LoadDirectory...");
    DicomParas paras;
    memset(&paras, 0, sizeof(DicomParas));

    SeriesDataInfo* series_info = DataTransferController::GetInstance()->GerSeriresDataInfo();
    if (!series_info)
    {
		CGLogger::Error("pointer is null : series_info");
        return false;
    }
    
    series_info->GetDicomDicomParas(paras); 
    
    unsigned char* pixel_data_buffer = series_info->GetPixelDataBuffer();  
	if (!pixel_data_buffer)
    {
		CGLogger::Error("pointer is null : pixel_data_buffer");
        return false;
    }

	// 转换为CT值
	unsigned long length = series_info->GetPixelDataLength();
	ConvertStoredValue2CTValue(pixel_data_buffer, length, paras);
	
	CGLogger::Debug("create VolData0...");
    volume_data_ = new VolData();

    volume_data_->SetBitsPerPixel(paras.bits_per_pixel);
    volume_data_->SetBitsStored(paras.bits_stored);
    volume_data_->SetSliceWidth(paras.window_width);
	CGLogger::Debug("create VolumeData1...");
    volume_data_->SetPixelData(new RawPixelData((char *)pixel_data_buffer));
	CGLogger::Debug("create VolumeData2...");
    volume_data_->GetPixelData()->SetBitsPerPixel(paras.bits_per_pixel);
    volume_data_->GetPixelData()->SetDimensions(paras.height, paras.width, paras.slice_count);
    volume_data_->GetPixelData()->SetOrigin(0,0,0);//
    volume_data_->GetPixelData()->SetSpacing(paras.spacing[0], paras.spacing[1], paras.spacing[2]);
    volume_data_->SetSliceWidth(paras.width);
    volume_data_->SetSliceHeight(paras.height);
    volume_data_->SetSliceCount(paras.slice_count);
    volume_data_->SetStudyInstanceUID(paras.study_id);
    volume_data_->SetSeriesInstanceUID(paras.series_iuid);
    volume_data_->SetBoundingBox(0, 0, 0, paras.height - 1, paras.width - 1, paras.slice_count - 1);
    volume_data_->SetDefaultWindowWidthLevel(paras.window_width, paras.window_level);
	CGLogger::Debug("create VolData End...");
    return true;
}

bool DcmtkDcmLoader::LoadFiles(std::vector<const char*> files)
{
    return true;
}
bool DcmtkDcmLoader::LoadDicomData(const char* dir)
{
    return true;
}
bool DcmtkDcmLoader::LoadVolumeMask(const char* file) 
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
VolData* DcmtkDcmLoader::GetData() 
{
    return  volume_data_;
}

void DcmtkDcmLoader::Close() 
{
    
}

void DcmtkDcmLoader::ConvertStoredValue2CTValue(unsigned char *data, unsigned long length, DicomParas &paras)
{
	// 此函数完全参考VTK的DICOMAppHelper::PixelDataCallback实现
	// 去掉了Float数据的判断，计算后CT值更新到输入参数里
	int ptrIncr = int(paras.bits_per_pixel/8.0);
	int numPixels = paras.width * paras.height * paras.slice_count;
	if (length / ptrIncr < numPixels)
	{
		numPixels = length / ptrIncr;
	}
	if (numPixels < 0)
	{
		numPixels = 0;
	}

	float slope = paras.rescale_slope;
	float offset = paras.rescale_offset;
	if (fabs(slope - 1.0f) < 0.0001 &&
		fabs(offset - 0.0f) < 0.0001){
		CGLogger::Info("Dicom image: rescale slope is 1 and offset is 0. no ct value was transformed.");
		return;
	}

	unsigned char* ucharInputData = data;
	short* shortInputData = reinterpret_cast<short*> (data);

	if (ptrIncr == 1)
	{
		unsigned long uNumPixels=static_cast<unsigned long>(numPixels);
		char newCharPixel;
		for (int i = 0; i < numPixels; i++)
		{
			newCharPixel = char(static_cast<double>(slope) * ucharInputData[i] + static_cast<double>(offset));
			ucharInputData[i] = newCharPixel;
		}
	}
	else if (ptrIncr == 2)
	{
		unsigned long uNumPixels=static_cast<unsigned long>(numPixels);
		short newShortPixel;
		for (int i = 0; i < numPixels; i++)
		{
			newShortPixel = short(static_cast<double>(slope) * shortInputData[i] + static_cast<double>(offset));
			shortInputData[i] = newShortPixel;
		}
	}
}
