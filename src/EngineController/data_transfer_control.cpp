//#include "stdafx.h"

#include "data_transfer_control.h"
#include "image_process.h"
#include "common_utils.h"
#include "series_data_info.h"
#include "render_facade.h"
#include "common_utils/common_header.h"
#include "ginkgouid.h"

#include "data_source.h"
#include "render_source.h"
#include "render_facade.h"
#include "tools/logger.h"
#include "io/txt_reader.h"
#include "tools/string_util.h"
#include "curve_source.h"
#include "tools/logger.h"

#include <algorithm>

#include <fstream> // ifstream, ifstream::in

#if USE_GOOGLE_BREAKPAD_LIB
#include "thirdparty/breakpad-google/src/client/linux/handler/exception_handler.h"
#endif

using namespace DW;
using namespace DW::IMAGE;
using namespace DW::IO;

#if USE_GOOGLE_BREAKPAD_LIB
google_breakpad::ExceptionHandler* exceptionHandler;
#endif

DataTransferController* DataTransferController::instance = nullptr;

DataTransferController* DataTransferController::GetInstance()
{
	if (instance == nullptr)
		instance = new DataTransferController();
	return instance;
}

DataTransferController::DataTransferController()
 : dcm_loader(nullptr)
 , series_info(nullptr)
{
	for (size_t i = 0; i < JSON_VALUE_REQUEST_TYPE_MAX; i++)
	{		
		switch (i)
		{
		case JSON_VALUE_REQUEST_TYPE_VR:
			arr_image_process[i] = new ImageVRProcess();
			break;
		case JSON_VALUE_REQUEST_TYPE_MPR:
			arr_image_process[i] = new ImageMPRProcess();
			break;
		case JSON_VALUE_REQUEST_TYPE_CPR:
			arr_image_process[i] = new ImageCPRProcess();
			break;
		default:
			arr_image_process[i] = nullptr;
			break;
		}				
	}
 }

DataTransferController::~DataTransferController()
{
	for (size_t i = 0; i < JSON_VALUE_REQUEST_TYPE_MAX; i++)
	{
		if (arr_image_process[i])
		{
			delete arr_image_process[i];
			arr_image_process[i] = nullptr;
		}		
	}

	if(dcm_loader)
	{
		delete dcm_loader;
		dcm_loader = nullptr;
	}

	//卸载序列时，释放资源
	if (series_info)
	{
		delete series_info;
		series_info = nullptr;
	}
}

#if USE_GOOGLE_BREAKPAD_LIB
// 写完minidump后的回调函数
static bool dumpCallback2(const google_breakpad::MinidumpDescriptor& descriptor,
                         void* context, bool succeeded) {
  printf("Dump path: %s\n", descriptor.path());
  return succeeded;
}

// 触发crash来测试
void crash2() {
  volatile int* a = (int*)(NULL);
  *a = 1;
}
#endif

int DataTransferController::ParseLoadSeries(const char* json_data)
{
	int ret = RET_STATUS_SUCCESS;
	// initialize logger
	static bool glog_loaded = false;
	if(false == glog_loaded)
	{
		// std::string glog_path = "./glog/";
		// CGLogger::InitGLog("", glog_path);
		// // 创建文件夹
		// TryCreateDir(glog_path);
		glog_loaded = true;

		#if USE_GOOGLE_BREAKPAD_LIB
		// 初始化ExceptionHandler
		std::string breakpad_path = "./dumbp/";
		google_breakpad::MinidumpDescriptor descriptor(breakpad_path); // minidump文件写入到的目录
		exceptionHandler = new google_breakpad::ExceptionHandler(descriptor, NULL, dumpCallback2, NULL, true, -1);
		//crash2();
		#endif
	}

	


	// 解析json字符串命令	
	try
	{
		x2struct::X::loadjson(json_data, series_process_paras, false, true);
	}catch(...)
	{
		CGLogger::Error("fail to parse json.");
		return RET_STATUS_JSON_PARSE_FAIL;
	}	
	CGLogger::Debug("load series dicom path : " + GetDicomPath());
	
	//卸载序列时，释放资源
	if (series_info)
	{
		delete series_info;
		series_info = nullptr;
	}	
	
	// 读取源文件夹下的Dicom文件
	series_info = new SeriesDataInfo(GetDicomPath());

    if (!series_info)
    {
		CGLogger::Error("the pointer of series_info is null.");
        return RET_STATUS_FAILURE;
    }
	ret = series_info->ReadDicomFilesFromDir(true);
	if (ret < RET_STATUS_SUCCESS ) {
		CGLogger::Error("fail to read dicom file.");
		return ret;
	}

	CGLogger::Debug("dcm loader.....");
	// 利用从Dicom文件中读取到的pixeldata 构造VolumeData数据
	// 1.1 read dcm image from directory	
	if (!dcm_loader) {
		dcm_loader = new DcmtkDcmLoader();
		CGLogger::Info("Dcm Loader....");
	}

	bool load_flag = dcm_loader->LoadDirectory(GetDicomPath().c_str());	// only once
	if (false == load_flag)
	{
		CGLogger::Info("dcm loader fail to load directory : " + GetDicomPath());
		return RET_STATUS_DICOM_LOAD_SERIES;
	}
	
	VolData* vol_data = dcm_loader->GetData();
	if (vol_data == NULL) 
	{
		CGLogger::Error("fail to get volume data .");
		return RET_STATUS_ACQUIRE_VOLUME_DATA_ERROR;
	}
	CGLogger::Debug("Add Volume Data to ImageDataSource..begin");
	ImageDataSource::Get()->AddVolData(DataTransferController::GetInstance()->GetSeriesuid(), vol_data);	
	CGLogger::Debug("Add Volume Data to ImageDataSource..end");

	CGLogger::Debug("Load volume mask.");
	// 1.2 load mask file
	std::string mask_file_path = GetMaskPath();
	if (!mask_file_path.empty())
	{
		// traversal the whole folder
		std::vector<std::string> vFiles;
		ListDir(mask_file_path, vFiles);
		if (vFiles.size() > 0)
		{
			mask_file_path += "/"  + vFiles[0];
			dcm_loader->LoadVolumeMask(mask_file_path.c_str());
		}		
	}
	
	CGLogger::Debug("Load curve data.");
	// 1.3 load curve file
	// move mpr to specified locations
	std::vector<std::string> vec_vessel_names;
	vec_vessel_names.push_back(VESSEL_NAME_L_ICA);
	vec_vessel_names.push_back(VESSEL_NAME_R_ICA);
	vec_vessel_names.push_back(VESSEL_NAME_L_VA );
	vec_vessel_names.push_back(VESSEL_NAME_R_VA );
	for (size_t i = 0; i < vec_vessel_names.size(); i++)
	{
		ret = ReadCurveAndCreateCurve(vec_vessel_names[i], vol_data);
		if (ret < RET_STATUS_SUCCESS ) {
			CGLogger::Error("fail to create curve " + vec_vessel_names[i]);
			return ret;
		}
	}	

	CGLogger::Debug("Create Image Control begin...");
	// 2.create image control  
	static bool is_create_wnd = false;
	if (is_create_wnd == false)
	{
		// only once
		RenderSource::Get()->CreateTwodImageControl(IMAGE_WINDOW_NAME_VR, RenderControlType::VR);
		RenderSource::Get()->CreateTwodImageControl(IMAGE_WINDOW_NAME_MPR, RenderControlType::MPR);
		RenderSource::Get()->CreateTwodImageControl(IMAGE_WINDOW_NAME_CPR, RenderControlType::STRETECHED_CPR);
		is_create_wnd = true;
	}
	CGLogger::Debug("Create Image Control end...");
	
	RenderFacade::Get()->SetCPRCurveID(IMAGE_WINDOW_NAME_CPR, curve_id);
	CGLogger::Debug("Set curve id.");

	return RET_STATUS_SUCCESS;
}

int DataTransferController::ReadCurveAndCreateCurve(const std::string vessel_name, VolData* vol_data)
{
	int ret = RET_STATUS_SUCCESS;

	if (vol_data == NULL) 
	{
		CGLogger::Error("fail to get volume data .");
		return RET_STATUS_ACQUIRE_VOLUME_DATA_ERROR;
	}

	std::string curve_file_path = GetCurvePath();
	if (!curve_file_path.empty())
	{
		std::string curve_full_path = curve_file_path + vessel_name + std::string(".txt");
		vector<string> curve_data = ReadTxt(curve_full_path.c_str());
		vector<Point3f> points;
		auto it = curve_data.begin();
		while (it != curve_data.end()){

			vector<string> arr_data = Split(*it, " ");
			if (arr_data.size() >= 3){
				Point3f pnt;
				pnt.x = atof(arr_data[0].c_str());
				pnt.y = atof(arr_data[1].c_str());
				pnt.z = vol_data->GetSliceCount() - 1 -atof(arr_data[2].c_str());

				points.push_back(pnt);
			}
			++it;
		}	
		// 1.4 create curve
		double spacings[3] = {0.0f};
		vol_data->GetPixelData()->GetSpacing(spacings);
		// found exception
		// 使用vessel_name区分curve, vessel_name共4个，为：L_ICA/R_ICA/L_VA/R_VA
		ret = CurveSource::Get()->CreateCurve(GetSeriesuid(), vessel_name, points, spacings);
		if (ret < RET_STATUS_SUCCESS ) {
			CGLogger::Error("fail to CreateCurve.");
			return ret;
		}
	}
	return RET_STATUS_SUCCESS;
}

int DataTransferController::ParseSwitchSeries(const char* json_data)
{
	int ret = RET_STATUS_SUCCESS;

	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。		
	SeriesIds ids;
	try
	{
		x2struct::X::loadjson(json_data, ids, false, true);
	}catch(...)
	{
		CGLogger::Error("fail to parse switchserires's json.");
		return RET_STATUS_JSON_PARSE_FAIL;
	}	 
	CGLogger::Info("switch series dicom path : " + GetDicomPath());
	SetSeriedIds(ids);
	
	// 3. change series 
	// 在后端调用的“切换序列”命令里，已经执行了切换操作。此处可以去掉
	ret = RenderFacade::Get()->ChangeSeries(GetSeriesuid());
		
	return ret;
}

int DataTransferController::ParseUnloadSeries(const char* json_data)
{
	int ret = RET_STATUS_SUCCESS;

	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。
	SeriesIds ids;
	try
	{
		x2struct::X::loadjson(json_data, ids, false, true);
	}catch(...)
	{
		CGLogger::Error("fail to parse unloadserires's json.");
		return RET_STATUS_JSON_PARSE_FAIL;
	}	
	SetSeriedIds(ids);
	
	CGLogger::Info("unload series dicom path : " + GetDicomPath());

	ret = RenderFacade::Get()->UnloadSeries(GetSeriesuid());

	return ret;
}

int DataTransferController::ParseImageOperationData(const char* json_data)
{
	int ret = RET_STATUS_SUCCESS;
#if USE_JSONCPP_LIB
	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。
	Json::Value root;
	Json::Reader reader;

	if (!reader.parse(json_data, root))
	{
		CGLogger::Error("fail to parse imageoperation's json.");
		return RET_STATUS_JSON_PARSE_FAIL;
	}
	
	// 获得关键性的参数
	int request_type = 0;
	ret = GetJsonDataInt(root, JSON_KEY_IMAGE_TYPE, request_type);
	
	if (ret < RET_STATUS_SUCCESS)
	{
		CGLogger::Error("fail to parse iamge type's json.");
		return ret;
	}		
#endif
	stImageOperationParams params;
	try
	{
		x2struct::X::loadjson(json_data, params, false, true);
	}catch(...)
	{
		CGLogger::Error("fail to parse unloadserires's json.");
		return RET_STATUS_JSON_PARSE_FAIL;
	}

	int request_type = params.image_type;
	if (request_type >= 0 && request_type < JSON_VALUE_REQUEST_TYPE_MAX)
	{
		if (arr_image_process[request_type])
		{
			ret = arr_image_process[request_type]->Excute(json_data);
			if (ret < RET_STATUS_SUCCESS) {
				CGLogger::Error("error to operate the image.");
				return ret;
			}
		}		
	}	

	return RET_STATUS_SUCCESS;
}

int DataTransferController::ImageEngineCommandCalled(const char* cmd_name, const char* cmd_paras)
{
	return RET_STATUS_SUCCESS;
}

// std::string DataTransferController::GetCurveId()
// {
// 	return curve_id;
// }

std::string DataTransferController::GetDicomPath()
{
	return TryAppendPathSeparatorInTail(series_process_paras.dicom_path);
}

std::string DataTransferController::GetMaskPath()
{
	return TryAppendPathSeparatorInTail(series_process_paras.mask_path);
}

std::string DataTransferController::GetCurvePath()
{
	return TryAppendPathSeparatorInTail(series_process_paras.curve_path);
}

std::string DataTransferController::GetPatientid()
{
	return series_process_paras.patient_id;
}

std::string DataTransferController::GetStudyuid()
{
	return series_process_paras.study_uid;
}

std::string DataTransferController::GetSeriesuid()
{
	return series_process_paras.series_uid;
}

// void DataTransferController::SetCurveId(const std::string& id)
// {
// 	curve_id = id;
// }

void DataTransferController::SetSeriedIds(const struct SeriesIds& ids)
{
	SetPatientid(ids.patient_id);
	SetStudyuid(ids.study_uid);
	SetSeriesuid(ids.series_uid);
}

void DataTransferController::SetPatientid(const std::string& id)
{
	series_process_paras.patient_id = id;
}

void DataTransferController::SetStudyuid(const std::string& id)
{
	series_process_paras.study_uid = id;
}


void DataTransferController::SetSeriesuid(const std::string& id)
{
	series_process_paras.series_uid = id;
}

SeriesDataInfo* DataTransferController::GerSeriresDataInfo() 
{ 
	return series_info; 
}

std::string DataTransferController::GenerateUniqueId()
{
	if (!series_info)
	{
		CGLogger::Error("the pointer of series_info is null.");
		return "";
	}
	
	int series_counter = series_info->GetSeriesDicomFileCount();
	std::string generate_series_uid = GIL::DICOM::MakeUID(GIL::DICOM::GUID_PrivateGenericFileSOPClass, series_counter);
	
	return generate_series_uid;
}