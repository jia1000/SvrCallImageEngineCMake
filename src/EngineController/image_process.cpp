//#include "stdafx.h"

#include "image_process.h"
#include "common_utils.h"
#include "common_utils/common_header.h"
#include "series_data_info.h"
#include "data_transfer_control.h"

#include "tools/logger.h"

#include "api/studycontextmy.h"
#include "api/studycontextmy.h"
#include "main/controllers/dicommanager.h"
#include "api/dicom/dicomdataset.h"
#include "api/dicom/dcmdictionary.h"
#include "DicomEngine/io/secondary_capture_image_generator.h"
#include "DicomEngine/io/ct_image_generator.h"
#include "img_source.h"

#include "data_source.h"
#include "render_source.h"
#include "render_facade.h"
#include "io/nii_loader.h"
#include "io/ct_image_generator.h"

#include <algorithm>
#include <fstream> // ifstream, ifstream::in

#include <sstream>

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimage/diargimg.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcdict.h"

using namespace DW;
using namespace DW::IMAGE;
using namespace DW::IO;

//////////////////////////////////////////////////////////////////////////
ImageProcessBase::ImageProcessBase()
{
}

ImageProcessBase::~ImageProcessBase()
{	
}
	
std::string ImageProcessBase::GetDescriptionText(const std::string& control_type)
{
	std::string description_text = "";		

	return description_text;	
}

GIL::DICOM::IDcmGenerator* ImageProcessBase::GetDcmGenerator(GIL::DICOM::DicomDataset& dataset)
{
	return nullptr;
}

int ImageProcessBase::GeneratorProcessedDicom(std::string control_type, bool is_mpr, 
	const std::string& generate_series_uid)
{
	int ret = RET_STATUS_SUCCESS;

	generated_files_.clear();
	max_size_[0] = 0;
	max_size_[1] = 0;
	IImageControl* image_control = RenderSource::Get()->GetImageControl(control_type);
	if (!image_control)
	{
		CGLogger::Error("the pointer of image_control is null.");
		return RET_STATUS_POINTER_IS_NULL;
	}	

	SeriesDataInfo* series_info = DataTransferController::GetInstance()->GerSeriresDataInfo();
	if (!series_info)
	{
		CGLogger::Error("the pointer of series_info is null.");
		return RET_STATUS_POINTER_IS_NULL;
	}
		
	GIL::DICOM::DicomDataset dataset;
	series_info->GetDicomDataSet(dataset, 0);

	bool is_absolutely_ok = true;
	int output_number = image_control->GetOutputNumber();
	for(int i = 0; i < output_number; ++i)
	{
		IBitmap *bmp = image_control->GetOutput(i);
		if(!bmp)
		{
			CGLogger::Error("get bitmap error : the pointer of image_control is null. " + std::to_string(i));
			is_absolutely_ok = false;
			continue;
		}
		IBitmapDcmInfo *bmpInfo  =  image_control->GetOutputInfo(i);
		if(!bmpInfo)
		{
			CGLogger::Error("get bitmap dcm info error : the pointer of image_control is null." + std::to_string(i));
			is_absolutely_ok = false;
			continue;
		}			

		// test git second 3.
		GIL::DICOM::IDcmGenerator* generator = GetDcmGenerator(dataset);
		if (!generator)
		{
			CGLogger::Error("get generator error : the pointer of generator is null." + std::to_string(i));
			is_absolutely_ok = false;
			continue;
		}
		

		// GIL::DICOM::CTImageDcmGenerator *generator = new GIL::DICOM::CTImageDcmGenerator(&dataset);
		//generator->SetTag(DCM_PatientID, "test_patient_id");
		generator->SetTag(DCM_InstanceNumber, bmpInfo->GetInstanceNumber());
		double spacings[2];
		double origins[3];
		double row_v[3], col_v[3];
		bmpInfo->GetOrientation(row_v, col_v);
		bmpInfo->GetOrigin(origins);
		bmpInfo->GetPixelSpacings(spacings);
		string str_spacing = to_string(spacings[0]) + "\\" + to_string(spacings[1]);
		string str_ori = to_string(row_v[0]) + "\\" + to_string(row_v[1]) + "\\" + to_string(row_v[2])
			+ "\\" + to_string(col_v[0]) + "\\" + to_string(col_v[1]) + "\\" + to_string(col_v[2]);
		string str_origin = to_string(origins[0]) + "\\" + to_string(origins[1]) + "\\" + to_string(origins[2]);
		
		int instance_number = bmpInfo->GetInstanceNumber();
		generator->SetTag(DCM_ImageOrientationPatient, str_ori);
		generator->SetTag(DCM_ImagePositionPatient, str_origin);
		generator->SetTag(DCM_PixelSpacing, str_spacing);
		generator->SetTag(DCM_SliceThickness, bmpInfo->GetThickness());

		
		generator->SetTag(DCM_SeriesInstanceUID, generate_series_uid);		

		int dicom_file_suid = GetDicomFileSuid(instance_number);
		generator->SetTag(DCM_SOPInstanceUID, generate_series_uid + "." + to_string(dicom_file_suid));
		generator->SetTag(DCM_InstanceNumber, to_string(dicom_file_suid));

		
		// Series Description
		std::string description_text = GetDescriptionText(control_type);
		generator->SetTag(DCM_SeriesDescription, description_text);
		// if (is_mpr){
		// 	generator->SetTag(DCM_SeriesDescription, "Position: " + to_string((int)bmpInfo->GetStepValue() * (instance_number - 1)) + "mm");
		// }
		// else{
		// 	char deg[3] = {0};
		// 	deg[0] = 0xa1;
		// 	deg[1] = 0xe3;
		// 	char str[30] = {0};
		// 	sprintf(str,"%s", deg);
		// 	generator->SetTag(DCM_SeriesDescription, "Angle: " + to_string((int)bmpInfo->GetStepValue() * (instance_number - 1)));
		// }

		int ww, wl;
		bmpInfo->GetWindowLevel(ww, wl);
		generator->SetTag(DCM_WindowWidth, to_string(ww));
		generator->SetTag(DCM_WindowCenter, to_string(wl));

		PixelDataSource *source = new PixelDataSource(bmp);
		generator->SetPixelData(source);

		std::string dst_file_path = GeneratorDicomFileName(i);
		ret = generator->Write(dst_file_path);
		if (ret < RET_STATUS_SUCCESS ) {
			CGLogger::Error("fail to write generator's dicom " + dst_file_path);
			return ret;
		}

		//////////////////////////////////////////////////////////////////////////
		// 记录生成的文件尺寸和文件路径 [7/23/2019 Modified by zhangjian]
		if (bmp->GetWidth() > max_size_[0]){
			max_size_[0] = bmp->GetWidth();
		}
		if (bmp->GetHeight() > max_size_[1]){
			max_size_[1] = bmp->GetHeight();
		}
		generated_files_.push_back(dst_file_path);
		//////////////////////////////////////////////////////////////////////////
		
		delete generator;
		delete bmp;
		delete bmpInfo;			
	}

	if (false == is_absolutely_ok)
	{
		return RET_STATUS_GENERATOR_DICOM_PART_ERROR;
	}
	return 	RET_STATUS_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
ImageMPRProcess::ImageMPRProcess()
	: ImageProcessBase()
{
	m_wnd_name = IMAGE_WINDOW_NAME_MPR;
}

ImageMPRProcess::~ImageMPRProcess()
{
}

int ImageMPRProcess::ParseJsonData(const char* json_data)
{	
	try
	{
		x2struct::X::loadjson(json_data, params, false, true);
	}catch(...)
	{
		CGLogger::Error("fail to parse json.");
		return RET_STATUS_JSON_PARSE_FAIL;
	}
	
	return RET_STATUS_SUCCESS;	
}

int ImageMPRProcess::Excute(const char* json_data)
{
	int ret = RET_STATUS_SUCCESS;

	CGLogger::Info("Operation : " + m_wnd_name);
	ret = ParseJsonData(json_data);
	if (ret < RET_STATUS_SUCCESS ) {
		CGLogger::Error("ret mpr excute : " + std::to_string(ret));
		return ret;
	}

	// 创建文件夹
	TryCreateDir(params.output_path);

	CGLogger::Info("params.output_path : " + params.output_path);	
	ret = RenderFacade::Get()->CreateMPRSlabBatch (m_wnd_name, 
		params.output_path,
		(BlendMode)params.blend_mode,
		(OrientationType)params.init_orientation,
		params.clip_percent,
		params.thickness,
		params.spacing_between_slices,
		params.window_width,
		params.window_level
		);

	if (ret < RET_STATUS_SUCCESS ) {
		CGLogger::Error("fail to CreateMPRSlabBatch " + std::to_string(ret));
		return ret;
	}

	std::string generate_series_uid = DataTransferController::GetInstance()->GenerateUniqueId();
	
	ret = GeneratorProcessedDicom(IMAGE_WINDOW_NAME_MPR, true, generate_series_uid);
	return ret;
}

std::string ImageMPRProcess::GeneratorDicomFileName(const int iamge_index)
{
	//
	std::string dst_file_path = params.output_path;
	
	std::stringstream ss_orientation;
	ss_orientation << params.init_orientation;
	std::string str_rule = ss_orientation.str();

	std::stringstream ss_index;
	ss_index << iamge_index + 1;
	std::string str_index = ss_index.str();	

	dst_file_path += str_rule;
	dst_file_path += "_";
	dst_file_path += str_index;
	dst_file_path += ".dcm";
	
	return dst_file_path;
}
std::string ImageMPRProcess::GetDescriptionText(const std::string& control_type)
{
	std::string description_text = "";	

	switch (params.init_orientation)
	{
	case AXIAL:
		description_text = "MIP-A";	
		break;
	case SAGITTAL:
		description_text = "MIP-S";	
		break;
	case CORONAL:
		description_text = "MIP-C";	
		break;
	default:
		break;
	}

	return description_text;	
}

GIL::DICOM::IDcmGenerator* ImageMPRProcess::GetDcmGenerator(GIL::DICOM::DicomDataset& dataset)
{
	GIL::DICOM::IDcmGenerator *generator = new GIL::DICOM::CTImageDcmGenerator(&dataset);
	return generator;
}

//////////////////////////////////////////////////////////////////////////
ImageVRProcess::ImageVRProcess()
	: ImageProcessBase()
{
	m_wnd_name = IMAGE_WINDOW_NAME_VR;
}

ImageVRProcess::~ImageVRProcess()
{
}

int ImageVRProcess::ParseJsonData(const char* json_data)
{	
	try
	{
		// 重置参数
		params.Reset();
		x2struct::X::loadjson(json_data, params, false, true);
	}catch(...)
	{
		CGLogger::Error("fail to parse json.");
		return RET_STATUS_JSON_PARSE_FAIL;
	}
	
	return RET_STATUS_SUCCESS;
} 

int ImageVRProcess::Excute(const char* json_data)
{
	int ret = RET_STATUS_SUCCESS;

	CGLogger::Info("Operation : " + m_wnd_name);
	ret = ParseJsonData(json_data);
	if (ret < RET_STATUS_SUCCESS ) {
		CGLogger::Error("ret vr excute : " + std::to_string(ret));
		return ret;
	}

	// 创建文件夹
	TryCreateDir(params.output_path);
	
	
	CGLogger::Info("params.output_path : " + params.output_path);
	// 4. reconstruction VR
	std::vector<int> vec_rotation_direction;
	if (0 == params.generate_rule) // wan zheng
	{
		vec_rotation_direction.push_back(params.rotation_direction);
	}
	else  //lu nei
	{
		vec_rotation_direction.push_back(LEFT_TO_RIGHT);
		vec_rotation_direction.push_back(HEAD_TO_FEET);
	}

	std::string generate_series_uid = DataTransferController::GetInstance()->GenerateUniqueId();
	
	for (size_t i = 0; i < vec_rotation_direction.size(); i++)
	{
		m_rotation_direction = vec_rotation_direction[i];
		ret = RenderFacade::Get()->CreateVRRotationBatch(m_wnd_name, 
			params.output_path,
			(BlendMode)params.blend_mode,
			(OrientationType)params.init_orientation,//OrientationType::CORONAL,
			(RotationDirection)vec_rotation_direction[i],		
			params.rotation_angle,
			params.clip_percent,
			params.output_image_number,
			params.window_width,
			params.window_level
			);
		if (ret < RET_STATUS_SUCCESS ) {
			CGLogger::Error("fail to CreateVRRotationBatch ");
			return ret;
		}

		ret = GeneratorProcessedDicom(IMAGE_WINDOW_NAME_VR, false, generate_series_uid);
		if (ret < RET_STATUS_SUCCESS ) {
			CGLogger::Error("fail to GeneratorProcessedDicom ");
			return ret;
		}
	}	

	return ret;
}

std::string ImageVRProcess::GeneratorDicomFileName(const int iamge_index)
{
	//
	std::string dst_file_path = params.output_path;
	int angle = (int)params.rotation_angle;
	
	//完整0/颅内1
	std::stringstream ss_rule;
	ss_rule << params.generate_rule;
	std::string str_rule = ss_rule.str();
	// 绘制方式：VR 0 ， MIP 1
	std::stringstream ss_blend;
	ss_blend << params.blend_mode;
	std::string str_blend = ss_blend.str();
	// 旋转方向： LEFT_TO_RIGHT 0,  HEAT_TO_FEET 1
	std::stringstream ss_direction;
	ss_direction << m_rotation_direction;//params.rotation_direction;
	std::string str_direction = ss_direction.str();
	// 旋转角度
	std::stringstream ss_angle;
	ss_angle << angle * iamge_index;
	std::string str_angle = ss_angle.str();

	dst_file_path += str_rule;
	dst_file_path += "_";
	dst_file_path += str_blend;
	dst_file_path += "_";
	dst_file_path += str_direction;
	dst_file_path += "_";
	dst_file_path += str_angle;
	dst_file_path += ".dcm";

	return dst_file_path;
}
std::string ImageVRProcess::GetDescriptionText(const std::string& control_type)
{
	std::string description_text = "";	
	if (0 == params.generate_rule && 0 == params.blend_mode)
	{
		description_text = "VR";
	}
	else if (0 == params.generate_rule && 1 == params.blend_mode)
	{
		description_text = "VMIP";
	}
	else if(1 == params.generate_rule && 0 == params.blend_mode)
	{
		description_text = "Intra-VR";
	}
	else if(1 == params.generate_rule && 1 == params.blend_mode)
	{
		description_text = "Intra-VMIP";
	}

	return description_text;	
}

GIL::DICOM::IDcmGenerator* ImageVRProcess::GetDcmGenerator(GIL::DICOM::DicomDataset& dataset)
{
	GIL::DICOM::IDcmGenerator *generator = new GIL::DICOM::SecondaryCaptureImageDcmGenerator(&dataset);
	return generator;
}

int ImageVRProcess::GetDicomFileSuid(int instance_number)  
{ 
	if (HEAD_TO_FEET == m_rotation_direction)
	{
		instance_number += params.output_image_number;
	}
	return instance_number; 
}

//////////////////////////////////////////////////////////////////////////
ImageCPRProcess::ImageCPRProcess()
	: ImageProcessBase()
{
	m_wnd_name = IMAGE_WINDOW_NAME_CPR;
	vec_vessel_name.clear();
	vec_vessel_name.push_back(VESSEL_NAME_L_ICA);
	vec_vessel_name.push_back(VESSEL_NAME_R_ICA);
	vec_vessel_name.push_back(VESSEL_NAME_L_VA);
	vec_vessel_name.push_back(VESSEL_NAME_R_VA);
}

ImageCPRProcess::~ImageCPRProcess()
{
}

int ImageCPRProcess::ParseJsonData(const char* json_data)
{
	try
	{
		x2struct::X::loadjson(json_data, params, false, true);
	}catch(...)
	{
		CGLogger::Error("fail to parse json.");
		return RET_STATUS_JSON_PARSE_FAIL;
	}
	
	return RET_STATUS_SUCCESS;	
}


int ImageCPRProcess::Excute(const char* json_data)
{	
	int ret = RET_STATUS_SUCCESS;

	CGLogger::Info("Operation : " + m_wnd_name);
	ret = ParseJsonData(json_data);
	if (ret < RET_STATUS_SUCCESS ) {
		CGLogger::Error("ret cpr excute : " + std::to_string(ret));
		return ret;
	}
	
	// 创建文件夹
	TryCreateDir(params.output_path);	
	TryCreateDir(params.output_path_va);	

	std::string generate_series_uid = DataTransferController::GetInstance()->GenerateUniqueId();
	
	// std::vector<std::string> vec_vessel_name;
	//////////////////////////////////////////////////////////////////////////
	std::vector<std::string> vec_ica_files;
	std::vector<std::string> vec_va_files;	
	int va_size[2] = { 0 };
	int ica_size[2] = { 0 };
	//////////////////////////////////////////////////////////////////////////

	for (size_t i = 0; i < vec_vessel_name.size(); i++)
	{
		m_vessel_name = vec_vessel_name[i];
		ret = RenderFacade::Get()->CreateCPRRotationBatch(m_wnd_name, 
			params.output_path,
			//DataTransferController::GetInstance()->GetCurveId(),
			vec_vessel_name[i],
			(OrientationType)params.init_orientation,
			(RotationDirection)params.rotation_direction,
			params.rotation_angle,
			params.output_image_number,
			params.window_width,
			params.window_level
			);
		if (ret < RET_STATUS_SUCCESS ) {
			CGLogger::Error("fail to CreateCPRRotationBatch ");
			return ret;
		}

		ret = GeneratorProcessedDicom(IMAGE_WINDOW_NAME_CPR, false, generate_series_uid);
		if (ret < RET_STATUS_SUCCESS ) {
			CGLogger::Error("fail to GeneratorProcessedDicom ");
			return ret;
		}

		//////////////////////////////////////////////////////////////////////////
		// 分别记录ICA和VA的文件尺寸和文件名称 [7/23/2019 Modified by zhangjian]
		auto it=generated_files_.begin();
		for(; it!=generated_files_.end(); ++it){
			if (m_vessel_name == VESSEL_NAME_L_ICA ||
				m_vessel_name == VESSEL_NAME_R_ICA){
				vec_ica_files.push_back((*it));
				for (size_t ii=0; ii<2; ++ii){
					if (max_size_[ii] > ica_size[ii]){
						ica_size[ii] = max_size_[ii];
					}
				}
			}
			else{
				vec_va_files.push_back((*it));
				for (size_t ii=0; ii<2; ++ii){
					if (max_size_[ii] > va_size[ii]){
						va_size[ii] = max_size_[ii];
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
	}

	//////////////////////////////////////////////////////////////////////////
	// 写入tag [7/23/2019 Modified by zhangjian]
	try 
	{
		DcmFileFormat dicom_file;
		DcmTag image_comment(DCM_ImageComments);
		// Write tag for ICA
		auto it=vec_ica_files.begin();
		string ica_val = to_string(ica_size[0]) + "\\" + to_string(ica_size[1]);
		for(; it!=vec_ica_files.end(); ++it){
			OFCondition status = dicom_file.loadFile((*it).c_str());
			if (status.bad()){
				CGLogger::Error("failed to load file " + (*it));
				continue;
			}
			status = dicom_file.loadAllDataIntoMemory();
			if (status.bad()){
				CGLogger::Error("failed to load all data info for ICA");
				continue;
			}
			dicom_file.getDataset()->putAndInsertString(image_comment, ica_val.c_str());
			dicom_file.saveFile((*it).c_str(), EXS_LittleEndianExplicit);
		}
		// Write tag for VA
		it=vec_va_files.begin();
		string va_val = to_string(va_size[0]) + "\\" + to_string(va_size[1]);
		for(; it!=vec_va_files.end(); ++it){
			OFCondition status = dicom_file.loadFile((*it).c_str());
			if (status.bad()){
				CGLogger::Error("failed to load file " + (*it));
				continue;
			}
			status = dicom_file.loadAllDataIntoMemory();
			if (status.bad()){
				CGLogger::Error("failed to load all data info for VA");
				continue;
			}
			dicom_file.getDataset()->putAndInsertString(image_comment, va_val.c_str());
			dicom_file.saveFile((*it).c_str(), EXS_LittleEndianExplicit);
		}
	}
	catch(...)
	{
		CGLogger::Error("error when writing image comments tag for cpr images.");
	}
	//////////////////////////////////////////////////////////////////////////

	return ret;
}

std::string ImageCPRProcess::GeneratorDicomFileName(const int iamge_index)
{
	std::string dst_file_path = "";
	if (VESSEL_NAME_L_ICA == m_vessel_name || VESSEL_NAME_R_ICA == m_vessel_name)
	{
		dst_file_path = params.output_path;
	}
	else
	{
		dst_file_path = params.output_path_va;
	}
	
	
	int angle = (int)params.rotation_angle;

	std::stringstream ss_angle;
	ss_angle << iamge_index * angle;
	std::string str_angle = ss_angle.str();


	dst_file_path += m_vessel_name;//params.vessel_name;
	dst_file_path += "_";
	dst_file_path += str_angle;
	dst_file_path += ".dcm";

	return dst_file_path;
}
std::string ImageCPRProcess::GetDescriptionText(const std::string& control_type)
{
	std::string description_text = "CPR";	

	return description_text;	
}

GIL::DICOM::IDcmGenerator* ImageCPRProcess::GetDcmGenerator(GIL::DICOM::DicomDataset& dataset)
{
	GIL::DICOM::IDcmGenerator *generator = new GIL::DICOM::CTImageDcmGenerator(&dataset);
	return generator;
}

int ImageCPRProcess::GetDicomFileSuid(int instance_number)  
{ 
	int ret_value = 0;
	int vessel_name_index = GetVesselNameIndex(m_vessel_name);

	ret_value = params.output_image_number * vessel_name_index + instance_number;
	
	return ret_value; 
}

int ImageCPRProcess::GetVesselNameIndex(const std::string& vessel_name)
{
	for (size_t i = 0; i < vec_vessel_name.size(); i++)
	{
		if (vessel_name == vec_vessel_name[i])
		{
			return i;
		}		
	}	

	return 0;	
}