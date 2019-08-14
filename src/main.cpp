

#include "EngineController/data_transfer_control.cpp"

#include <string.h>
#include <iostream>

// using namespace std;

int loadSeries(std::string str);
int switchSeries(std::string str);
int unloadSeries(std::string str);
int processSeries(std::string str);

int callCommand(std::string str_cmd_name, std::string str_cmd_paras);

class DataTransferController;

int main(int argc, char** args)
{	
	CGLogger::Info("begin debug.");
	bool opened = true;
	
	// command called
	if(opened)
	{			
		std::string json_cmd_name = "{";
		json_cmd_name += "\"cmd_name\":\"set_glog\",";
		json_cmd_name += "\"last_variable\":888"; 		// 
		json_cmd_name += "}";

		std::string json_cmd_paras = "{";
		json_cmd_paras += "\"glog_file_path\":\"./build/log/\",";
		json_cmd_paras += "}";
		json_cmd_paras += "\"last_variable\":888"; 		// 
		callCommand(json_cmd_name, json_cmd_paras);
	}
	
	const int MAX_TEST = 2;
		std::string series_uid[MAX_TEST] = {
			"\"series_uid\":\"1\"",
			"\"series_uid\":\"2\""
		};
	for (size_t hh = 0; hh < 1; hh++)
	{	
	for (int i = 0; i < MAX_TEST - 1; i++)
	{
		// 加载Series
		if(opened)
		{
			std::string dicom_path[MAX_TEST] = {
				"\"dicom_path\":\"./dicom_source/dicom_file/\",",
				"\"dicom_path\":\"./dicom_source2/dicom_file/\","
			};
			std::string mask_path[MAX_TEST] = {
				"\"mask_path\":\"./dicom_source/mask_source/\",",
				"\"mask_path\":\"./dicom_source2/mask_source/\","
			};
			std::string curve_path[MAX_TEST] = {
				"\"curve_path\":\"./dicom_source/curve_source/\",",
				"\"curve_path\":\"./dicom_source2/curve_source/\","
			};

			
			std::string json_data = "{";
			json_data += dicom_path[i];//"\"dicom_path\":\"./dicom_source/\",";
			json_data += mask_path[i];//"\"mask_path\":\"./mask_source/\",";
			json_data += curve_path[i];//"\"curve_path\":\"./curve_source/\",";
			json_data += "\"patient_id\":\"zhangsan\",";
			json_data += "\"study_uid\":\"111\",";
			json_data += series_uid[i];//"\"series_uid\":\"222\"";
			json_data += "}";
			loadSeries(json_data);
		}
		// 切换Series
		if(opened)
		{			
			std::string json_data = "{";
			json_data += "\"patient_id\":\"lilsi\",";
			json_data += "\"study_uid\":\"333\",";
			json_data += series_uid[i];//"\"series_uid\":\"444\"";
			json_data += "}";
			switchSeries(json_data);
		}
		if(!opened) // 完整 VR
		{
			std::string output_path[MAX_TEST] = {
				"\"output_path\":\"./build/vr_full/\",",
				"\"output_path\":\"./build/vr_full2/\","
			};
			std::string output_image_number[MAX_TEST] = {
				"\"output_image_number\":12,",
				"\"output_image_number\":12,"
			};
			
			std::string json_data = "{";
			json_data += "\"image_type\":0,"; 			// 图像类型
			json_data += "\"blend_mode\":0,"; 			// 绘制方式 : VR为0；  VRMIP为1
			json_data += "\"init_orientation\":2,"; 	// 正位（起始位置）标识
			json_data += "\"generate_rule\":0,"; 		// generate_rule
			json_data += "\"clip_percent\":1.0,"; 		// 截取比例;  若为0.35，则表示颅内;其它为完整
			json_data += "\"rotation_direction\":0,"; 	// 旋转方向
			json_data += "\"rotation_angle\":30.0,"; 	// 旋转角度
			json_data += output_image_number[i];//"\"output_image_number\":12,";	// 输出图像数目c
			json_data += output_path[i];//"\"output_path\":\"./build/vr8/\","; // 输出图像目录
			json_data += "\"window_width\":2000,"; 		// 窗宽
			json_data += "\"window_level\":400,"; 		// 窗位
			json_data += "\"last_variable\":888"; 		// 
			json_data += "}";
			processSeries(json_data);
		}
		if(!opened) //完整 VMIP
		{
			std::string output_path[MAX_TEST] = {
				"\"output_path\":\"./build/vr_mip_full/\",",
				"\"output_path\":\"./build/vr_mip_full2/\","
			};
			std::string output_image_number[MAX_TEST] = {
				"\"output_image_number\":12,",
				"\"output_image_number\":16,"
			};
			
			std::string json_data = "{";
			json_data += "\"image_type\":0,"; 			// 图像类型
			json_data += "\"blend_mode\":1,"; 			// 绘制方式 : VR为0；  VRMIP为1
			json_data += "\"init_orientation\":2,"; 	// 正位（起始位置）标识
			json_data += "\"generate_rule\":0,"; 		// generate_rule
			json_data += "\"clip_percent\":1.0,"; 		// 截取比例;  若为0.35，则表示颅内;其它为完整
			json_data += "\"rotation_direction\":0,"; 	// 旋转方向
			json_data += "\"rotation_angle\":30.0,"; 	// 旋转角度
			json_data += output_image_number[i];//"\"output_image_number\":12,";	// 输出图像数目c
			json_data += output_path[i];//"\"output_path\":\"./build/vr8/\","; // 输出图像目录
			json_data += "\"window_width\":2000,"; 		// 窗宽
			json_data += "\"window_level\":400,"; 		// 窗位
			json_data += "\"last_variable\":888"; 		// 
			json_data += "}";
			processSeries(json_data);
		}
		if(!opened)// 颅内 VR
		{
			std::string output_path[MAX_TEST] = {
				"\"output_path\":\"./build/vr_part/\",",
				"\"output_path\":\"./build/vr_part2/\","
			};
			std::string output_image_number[MAX_TEST] = {
				"\"output_image_number\":12,",
				"\"output_image_number\":16,"
			};
			
			std::string json_data = "{";
			json_data += "\"image_type\":0,"; 			// 图像类型
			json_data += "\"blend_mode\":0,"; 			// 绘制方式 : VR为0；  VRMIP为1
			json_data += "\"init_orientation\":2,"; 	// 正位（起始位置）标识
			json_data += "\"generate_rule\":1,"; 		// generate_rule
			json_data += "\"clip_percent\":1.0,"; 		// 截取比例;  若为0.35，则表示颅内;其它为完整
			json_data += "\"rotation_direction\":0,"; 	// 旋转方向
			json_data += "\"rotation_angle\":30.0,"; 	// 旋转角度
			json_data += output_image_number[i];//"\"output_image_number\":12,";	// 输出图像数目c
			json_data += output_path[i];//"\"output_path\":\"./build/vr8/\","; // 输出图像目录
			json_data += "\"window_width\":2000,"; 		// 窗宽
			json_data += "\"window_level\":400,"; 		// 窗位
			json_data += "\"last_variable\":888"; 		// 
			json_data += "}";
			processSeries(json_data);
		}
		if(!opened)// 颅内 VMIP
		{
			std::string output_path[MAX_TEST] = {
				"\"output_path\":\"./build/vr_mip_part/\",",
				"\"output_path\":\"./build/vr_mip_part2/\","
			};
			std::string output_image_number[MAX_TEST] = {
				"\"output_image_number\":12,",
				"\"output_image_number\":12,"
			};
			
			std::string json_data = "{";
			json_data += "\"image_type\":0,"; 			// 图像类型
			json_data += "\"blend_mode\":1,"; 			// 绘制方式 : VR为0；  VRMIP为1
			json_data += "\"init_orientation\":2,"; 	// 正位（起始位置）标识
			json_data += "\"generate_rule\":1,"; 		// generate_rule
			json_data += "\"clip_percent\":1.0,"; 		// 截取比例;  若为0.35，则表示颅内;其它为完整
			json_data += "\"rotation_direction\":0,"; 	// 旋转方向
			json_data += "\"rotation_angle\":30.0,"; 	// 旋转角度
			json_data += output_image_number[i];//"\"output_image_number\":12,";	// 输出图像数目c
			json_data += output_path[i];//"\"output_path\":\"./build/vr8/\","; // 输出图像目录
			json_data += "\"window_width\":2000,"; 		// 窗宽
			json_data += "\"window_level\":400,"; 		// 窗位
			json_data += "\"last_variable\":888"; 		// 
			json_data += "}";
			processSeries(json_data);
		}
		if(!opened)
		{
			std::string output_path[MAX_TEST] = {
				"\"output_path\":\"./build/mpr3/\",",
				"\"output_path\":\"./build/mpr4/\","
			};
			// MPRMIP 操作
			std::string json_data = "{";
			json_data += "\"image_type\":1,"; 			// 图像类型
			json_data += "\"blend_mode\":1,"; 			// 绘制方式 : VR为0；  VRMIP为1
			json_data += "\"init_orientation\":0,"; 	// 正位（起始位置）标识
			json_data += "\"clip_percent\":0.35,"; 		// 截取比例;  
			json_data += "\"thickness\":25.0,"; 		// 层厚
			json_data += "\"spacing_between_slices\":5.0,";// 层间距
			json_data += output_path[i];//"\"output_path\":\"../build/mprmip1/\","; // 输出图像目录
			json_data += "\"window_width\":2000,"; 		// 窗宽
			json_data += "\"window_level\":400,"; 		// 窗位
			json_data += "\"last_variable\":999"; 		// 
			json_data += "}";
			processSeries(json_data);
		}
		
		if(opened)
		{
			std::string output_path[MAX_TEST] = {
				"\"output_path\":\"./build/cpr_ica3/\",",
				"\"output_path\":\"./build/cpr_ica4/\","
			};
			std::string output_path_va[MAX_TEST] = {
				"\"output_path_va\":\"./build/cpr_va3/\",",
				"\"output_path_va\":\"./build/cpr_va/\","
			};
			std::string output_image_number[MAX_TEST] = {
				"\"output_image_number\":4,",
				"\"output_image_number\":4,"
			};
			
			// CPR 操作
			std::string json_data = "{";
			json_data += "\"image_type\":2,"; 			// 图像类型
			json_data += "\"vessel_name\":\"R_ICA\","; 	// 血管名称
			json_data += "\"init_orientation\":2,"; 	// 正位（起始位置）标识
			json_data += "\"rotation_direction\":0,"; 	// 旋转方向
			json_data += "\"rotation_angle\":45.0,"; 	// 旋转角度
			json_data += output_image_number[i];//"\"output_image_number\":4,"; 	// 输出图像数目
			json_data += output_path[i];//"\"output_path\":\"../build/cpr7/\","; // 输出图像目录
			json_data += output_path_va[i];
			json_data += "\"window_width\":2000,"; 		// 窗宽
			json_data += "\"window_level\":400,"; 		// 窗位
			json_data += "\"last_variable\":777"; 		// 
			json_data += "}";
			processSeries(json_data);
		}
		// 卸载Series
		if(opened)
		{
			std::string json_data = "{";
			json_data += "\"patient_id\":\"wangmazi\",";
			json_data += "\"study_uid\":\"333\",";
			json_data += series_uid[i];//"\"series_uid\":\"666\"";
			json_data += "}";
			unloadSeries(json_data);
		}
	}
	CGLogger::Info("end debug.");
	}

	return 0;
}

int loadSeries(std::string str)
{
	CGLogger::Info("LoadSeries: \njson old data : " + str);
	
	int ret = DataTransferController::GetInstance()->ParseLoadSeries(str.c_str());
	return ret; 
}

int switchSeries(std::string str)
{
	CGLogger::Info("switchSeries: \njson old data : " +  str);
	
	int ret = DataTransferController::GetInstance()->ParseSwitchSeries(str.c_str());
	return ret; 
}

int unloadSeries(std::string str)
{
	CGLogger::Info("unloadSerirs: \njson old data : " + str);
	
	int ret = DataTransferController::GetInstance()->ParseUnloadSeries(str.c_str());
	return ret; 
}

int processSeries(std::string str)
{
	CGLogger::Info("ProcessSeries: \njson old data : n" + str);

	int ret = DataTransferController::GetInstance()->ParseImageOperationData(str.c_str());

	return ret;  
}

int callCommand(std::string str_cmd_name, std::string str_cmd_paras)
{
	CGLogger::Info("callCommand: name  : n" + str_cmd_name);
	CGLogger::Info("callCommand: paras : n" + str_cmd_paras);


	int ret = DataTransferController::GetInstance()->ImageEngineCommandCalled(str_cmd_name.c_str(), str_cmd_paras.c_str());

	return ret;  
}