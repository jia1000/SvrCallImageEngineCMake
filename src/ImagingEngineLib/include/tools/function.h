/*=========================================================================

  Program:   ImagingEngine
  Module:    function.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once
#include <iostream>
#include <cstdio>

using namespace std;

#ifdef WIN32
#include "windows.h"
#else
#include "unistd.h"
#include "sys/sysinfo.h"
#endif

/// 获取系统CPU逻辑核心数
inline int GetCpuCoreNumber()
{
#ifdef WIN32
	SYSTEM_INFO sysInfo;
	GetSystemInfo( &sysInfo );
	return (int)sysInfo.dwNumberOfProcessors;
#else
	//sysconf函数中输入参数_SC_NPROCESSORS_CONF和_SC_NPROCESSORS_ONLN均可以获取系统CPU个数。 
	//_SC_NPROCESSORS_CONF：返回系统所有的CPU数量，这个值也包括系统中禁止用户使用的CPU个数； 
	//	_SC_NPROCESSORS_ONLN：返回系统中可用的CPU个数；
	return (int)sysconf(_SC_NPROCESSORS_CONF);
#endif
}