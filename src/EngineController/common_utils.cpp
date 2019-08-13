
#include "common_utils.h"
#include "common_utils/common_header.h"
#include "tools/logger.h"

#include <unistd.h>   // 创建文件夹 access 依赖的头文件
#include <sys/stat.h> // 创建文件夹 mkdir  依赖的头文件

#include <dirent.h>		//遍历文件夹下的文件 
#include <sys/types.h>

#if USE_JSONCPP_LIB
int GetJsonDataInt(const Json::Value& root, const std::string key, int& data)
{	
	const Json::Value& value = root[key];
	if (value.isNull()) {
		// json值未找到
		CGLogger::Error("json value(" + key + ") not found");
		return RET_STATUS_JSON_NAME_NOT_FOUND;		
	} else { 
		if (value.isInt())
		{
			data = value.asInt();
			return RET_STATUS_SUCCESS;
		} else {
			// json类型错误
			CGLogger::Error("json value(" + key + ") type error");
			return RET_STATUS_JSON_VALUE_TYPE_ERROR;
		}
	}
	return RET_STATUS_FAILURE;
}

int GetJsonDataString(const Json::Value& root, const std::string key, std::string& data)
{
	const Json::Value& value = root[key];
	if (value.isNull()) {		
		// json值未找到
		CGLogger::Error("json value(" + key + ") not found");
		return RET_STATUS_JSON_NAME_NOT_FOUND;
	} else {
		if (value.isString()) {
			data = value.asString();
			return RET_STATUS_SUCCESS;
		} else {
			// json类型错误
			CGLogger::Error("json value(" + key + ") type error");
			return RET_STATUS_JSON_VALUE_TYPE_ERROR;
		}		
	}
	return RET_STATUS_FAILURE;	
}

int GetJsonDataFloat(const Json::Value& root, const std::string key, float& data)
{
	const Json::Value& value = root[key];
	if (value.isNull()) {		
		// json值未找到
		CGLogger::Error("json value(" + key + ") not found");
		return RET_STATUS_JSON_NAME_NOT_FOUND;
	} else {
		if (value.isDouble())
		{
			data = value.asDouble();
			return RET_STATUS_SUCCESS;
		} else {
			// json类型错误
			CGLogger::Error("json value(" + key + ") type error");
			return RET_STATUS_JSON_VALUE_TYPE_ERROR;
		}		
	}
	return RET_STATUS_FAILURE;	
}
#endif

void SplitString(const std::string& src, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = src.find(c);
	pos1 = 0;
	while(std::string::npos != pos2)
	{
		v.push_back(src.substr(pos1, pos2 - pos1));
		pos1 = pos2 + c.size();
		pos2 = src.find(c, pos1);
	}
	if(pos1 != src.length()) 
	{
		v.push_back(src.substr(pos1) );
	}
}
void TryCreateDir(const std::string& dir)
{
	CGLogger::Info("Save Image to  : " + dir);
	
	std::vector<std::string> v;
	SplitString(dir, v, "/");

	std::string dst_dir_path("" );
	for(auto iter = v.begin(); iter != v.end(); ++iter)
	{
		// 创建文件夹
		dst_dir_path += *iter;
		dst_dir_path += "/";
		if( 0 != access(dst_dir_path.c_str(), 0))
		{
			CGLogger::Error("create folder: " +  dst_dir_path);
			// 如果文件夹不存在，创建
			mkdir(dst_dir_path.c_str(), 0755);
		}
	}
}


std::string TryAppendPathSeparatorInTail(const std::string& path)
{
	if (path.empty())
	{
		return path;
	}
	std::size_t found = path.find_last_of("/\\");
	if (found != path.length() - 1)
	{
		std::string path_add_tail = path + "/";
		return path_add_tail;
	}
	return path;
}

int ListDir(std::string path, std::vector<std::string>& files)
{
	DIR *dir = opendir(path.c_str());
	struct dirent* pDir = NULL;

	if (!dir)
	{
		CGLogger::Error("Effor! can't open this dir : " + path);
		return 0;
	}
	while (1)
	{
		pDir = readdir(dir);
		if (!pDir)
		{
			break;
		}
		
		if (DT_REG == pDir->d_type)
		{
			files.push_back(pDir->d_name);
		}		
	}
	closedir(dir);

	return 1;
}