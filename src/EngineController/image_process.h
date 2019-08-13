/****************************************************************************************************

 * \file image_process.h
 * \date 2019/05/06 11:18
 *
 * \author jiayf
 * Contact: @deepwise.com
 *
 * \brief  

 Copyright(c) 2018-2030 DeepWise Corporation

****************************************************************************************************/

#pragma once

#include <iostream> 
#include <vector>

#include "common_utils.h"
#include "common_utils/common_header.h"

#include "thirdparty/x2struct/x2struct.hpp"// json2struct header file

namespace GIL {
	namespace DICOM {

		class IDcmGenerator;
		class DicomDataset;
	}
}

#include "control/image_control.h"

//////////////////////////////////////////////////////////////////////////
class ImageProcessBase
{
public:
	ImageProcessBase();
	virtual ~ImageProcessBase();

	virtual int Excute(const char* json_data)
	{
		return RET_STATUS_SUCCESS;
	}
	virtual int ParseJsonData() 
	{
		return RET_STATUS_SUCCESS;
	}
	virtual std::string GeneratorDicomFileName(const int iamge_index) { return ""; }
	virtual int GeneratorProcessedDicom(std::string control_type, bool is_mpr, 
		const std::string& generate_series_uid);
	virtual std::string GetDescriptionText(const std::string& control_type);
	virtual int GetDicomFileSuid(int instance_number)  { return instance_number; }
	virtual GIL::DICOM::IDcmGenerator* GetDcmGenerator( GIL::DICOM::DicomDataset& dataset);

protected:
	// 窗口名称
	std::string m_wnd_name;
	std::vector<std::string> generated_files_;
	int max_size_[2];
	#if USE_JSONCPP_LIB
	Json::Value root;
	#endif
};

//////////////////////////////////////////////////////////////////////////
// MPR 请求类型的处理
class ImageMPRProcess : public ImageProcessBase
{
public:
	ImageMPRProcess();
	~ImageMPRProcess();
	virtual int Excute(const char* json_data);

	

private:
	virtual int ParseJsonData(const char* json_data);	
	virtual std::string GeneratorDicomFileName(const int iamge_index);
	virtual std::string GetDescriptionText(const std::string& control_type);
	virtual GIL::DICOM::IDcmGenerator* GetDcmGenerator( GIL::DICOM::DicomDataset& dataset);

	struct stMPRMIPImageParams
	{
		stMPRMIPImageParams()
		{
			image_type				= 0;
			blend_mode				= 0;
			init_orientation		= 0;
			clip_percent			= 0;
			thickness				= 0;
			spacing_between_slices	= 0;
			output_path				= "";
			window_width			= 0;
			window_level			= 0;	
		}
		int image_type;
		int blend_mode;
		int init_orientation;
		float clip_percent;
		float thickness;
		float spacing_between_slices;
		std::string output_path;
		int window_width;
		int window_level;	

		XTOSTRUCT(O(image_type, blend_mode, init_orientation, 
		clip_percent, thickness, spacing_between_slices, 
		output_path, window_width, window_level
		));
	};
	stMPRMIPImageParams params;
	
};

//////////////////////////////////////////////////////////////////////////
// VR 请求类型的处理
class ImageVRProcess : public ImageProcessBase
{
public:
	ImageVRProcess();
	~ImageVRProcess();
	virtual int Excute(const char* json_data);
	
private:
	virtual int ParseJsonData(const char* json_data);
	virtual std::string GeneratorDicomFileName(const int iamge_index);
	virtual std::string GetDescriptionText(const std::string& control_type);
	virtual int GetDicomFileSuid(int instance_number);
	virtual GIL::DICOM::IDcmGenerator* GetDcmGenerator(GIL::DICOM::DicomDataset& dataset);

	struct stVRImageParams
	{
		stVRImageParams()
		{
			Reset();
		}
		int image_type;
		int blend_mode;
		int init_orientation;
		float clip_percent;
		int generate_rule;
		int rotation_direction;
		float rotation_angle;
		int output_image_number;
		std::string output_path;
		int window_width;
		int window_level;	

		XTOSTRUCT(O(image_type, blend_mode, init_orientation, 
		clip_percent, generate_rule,rotation_direction,
		rotation_angle, output_image_number, output_path,
		window_width, window_level
		));

		void Reset()
		{
			image_type			= 0;
			blend_mode			= 0;
			init_orientation	= 0;
			clip_percent		= 1.0f;
			generate_rule		= 0;
			rotation_direction	= 0;
			rotation_angle		= 0;
			output_image_number = 0;
			output_path			= "";
			window_width		= 0;
			window_level		= 0;
		}
	};
	stVRImageParams params;
	int m_rotation_direction;
};

//////////////////////////////////////////////////////////////////////////
// CPR 请求类型的处理
class ImageCPRProcess : public ImageProcessBase
{
public:
	ImageCPRProcess();
	~ImageCPRProcess();
	virtual int Excute(const char* json_data); 

private:
	virtual int ParseJsonData(const char* json_data);
	virtual std::string GeneratorDicomFileName(const int iamge_index);
	virtual std::string GetDescriptionText(const std::string& control_type);
	virtual int GetDicomFileSuid(int instance_number);
	virtual GIL::DICOM::IDcmGenerator* GetDcmGenerator(GIL::DICOM::DicomDataset& dataset);

	int GetVesselNameIndex(const std::string& vessel_name);
	std::string curve_id_;
	struct stCPRImageParams
	{
		stCPRImageParams()
		{
			image_type			= 0;
			vessel_name			= "";
			init_orientation	= 0;
			rotation_direction	= 0;
			rotation_angle		= 0;
			output_image_number = 0;
			output_path			= "";
			output_path_va 		= "";
			window_width		= 0;
			window_level		= 0;	
		}
		int image_type;
		std::string vessel_name;
		int init_orientation;
		int rotation_direction;
		float rotation_angle;
		int output_image_number;
		std::string output_path;    // ica
		std::string output_path_va; // va
		int window_width;
		int window_level;	

		XTOSTRUCT(O(image_type, vessel_name, init_orientation, 
		rotation_direction, rotation_angle, output_image_number, 
		output_path, output_path_va, window_width, window_level
		));
	};
	stCPRImageParams params;
	std::string m_vessel_name;
	std::vector<std::string> vec_vessel_name;
};
