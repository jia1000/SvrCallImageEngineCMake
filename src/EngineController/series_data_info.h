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

#include "main/controllers/dicommanager.h"
#include "api/dicom/dicomdataset.h"
#include "api/dicom/dcmdictionary.h"
#include "api/dicom/idicom.h"

#include <map>
#include <functional> // 包含greater

#define USE_CALCULATE_CTV

const int POSITION_LENGHT = 3;
const int SPACING_LENGHT = 3;

struct DicomInfo
{
	std::string file_name;
	GIL::DICOM::DicomDataset base;
	GIL::DICOM::TagPrivadoUndefined tagBinary;
	//double position[POSITION_LENGHT];
};
struct DicomParas
{
	double position[POSITION_LENGHT];
	double spacing[SPACING_LENGHT];
	int window_width;
	int window_level;
	int bits_per_pixel;
	int bits_stored;
	int height;
	int width;
	int slice_count;
	int bits_allocated;
	double slice_location;
	float rescale_offset;
	float rescale_slope;
	int pixel_representation;


	std::string patient_id;
	std::string study_id;
	std::string series_iuid; 
};

class SeriesDataInfo
{
public:
	SeriesDataInfo(const std::string path);
	~SeriesDataInfo();
	
	int ReadDicomFilesFromDir(bool is_folder);

	void GetTag(const std::string& tag, int& i);
	void GetTag(const std::string& tag, double& d);
	void GetTag(const std::string& tag, std::string& s);

	int GetSeriesDicomFileCount();
	int GetDicomDataSet(GIL::DICOM::DicomDataset& base, const int slice_index);
	int GetDicomDicomParas(DicomParas& paras, 
		const int slice_index = 0);

	unsigned char* GetPixelDataBuffer();
	unsigned int GetPixelDataLength();
	
private:
	int ReadFolder(const std::string& path);
	int ReadFile(const std::string& path);
	bool GetSpacing(const int indice, 
			double& x, double& y, double& z, 
			bool isGetZAsThinkness = false);

	int GetPosition(GIL::DICOM::DicomDataset& base, double position[POSITION_LENGHT]);
	const std::string m_src_path;

	// Dicom数据，按照床位值，从大到小排序 
	std::map<double, DicomInfo*, std::greater<double>> m_bases;
	unsigned char* m_pixel_data_buffer;
	int m_pixel_data_length;	
};