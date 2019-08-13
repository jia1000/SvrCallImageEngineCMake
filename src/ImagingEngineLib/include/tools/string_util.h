#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <ctime>

#ifdef WIN32
#include <windows.h>
#include <atlstr.h>
#include <objbase.h>
#else
#include <sys/time.h>
#include <uuid/uuid.h>
#include "data/data_definition.h"

typedef struct _GUID_DW {
	DWORD Data1;
	WORD Data2;
	WORD Data3;
	BYTE Data4[8];
} GUID_DW;
#endif

using namespace std;

/// 模板函数：将string类型变量转换为常用的数值类型
template <class REAL>  
REAL StringToNumber(const string& str)  
{  
	istringstream iss(str);  
	REAL num;  
	iss >> num;  
	return num;      
}  
//
//inline string& ltrim(string &ss)
//{
//	int(*pf)(int) = isspace;
//	string::iterator   p = find_if(ss.begin(), ss.end(), not1(ptr_fun(pf)));
//	ss.erase(ss.begin(), p);
//	return   ss;
//}
//
//inline string& rtrim(string &ss)
//{
//	int(*pf)(int) = isspace;
//	string::reverse_iterator   p = find_if(ss.rbegin(), ss.rend(), not1(ptr_fun(pf)));
//	ss.erase(p.base(), ss.end());
//	return   ss;
//}
//
//inline string& trim(string &st)
//{
//	ltrim(rtrim(st));
//	return   st;
//}

inline std::vector<std::string> Split(const std::string &str, const std::string &pattern)
{
	std::vector<std::string> resVec;

	if (str.empty())
	{
		return resVec;
	}
	/// 方便截取最后一段数据
	std::string strs = str + pattern;

	size_t pos = strs.find(pattern);
	size_t size = strs.size();

	while (pos != std::string::npos)
	{
		std::string x = strs.substr(0, pos);
		resVec.push_back(x);
		strs = strs.substr(pos + 1, size);
		pos = strs.find(pattern);
	}

	return resVec;
}

inline std::string GenerateGUID()
{
#ifdef WIN32
	GUID guid;
	CoCreateGuid(&guid);
#else
	GUID_DW guid;
	uuid_generate(reinterpret_cast<unsigned char *>(&guid));
#endif

	char buf[64] = {0};
#ifdef __GNUC__
	snprintf(
#else // MSVC
	_snprintf_s(
#endif
		buf,
		sizeof(buf),
		"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);

	return string(buf);
}

inline std::string GenerateCurrentTime()
{
#ifdef WIN32
	SYSTEMTIME st;
	::GetLocalTime(&st);
	char chYearMonth[1000] = { 0 };
	printf_s(chYearMonth, _T("%04d%02d"), st.wYear, st.wMonth);
	char chDay[1000] = { 0 };
	printf_s(chDay, _T("%02d"), st.wDay);

	return string(chDay);
#else
	// 基于当前系统的当前日期/时间
	time_t now = time(0);
	tm *ltm = localtime(&now);

	char iyear[50], imonth[50], iday[50], ihour[50], imin[50], isec[50];

	sprintf(iyear, "%d",1900 + ltm->tm_year );
	sprintf(imonth, "%02d", 1 + ltm->tm_mon );
	sprintf(iday, "%02d", ltm->tm_mday );
	sprintf(ihour, "%02d", ltm->tm_hour );
	sprintf(imin, "%02d",  ltm->tm_min);
	sprintf(isec, "%02d",  ltm->tm_sec);
	
	return string(iyear) + string(imonth) + string(iday) + string(ihour) + string(imin) + string(isec);
#endif
}