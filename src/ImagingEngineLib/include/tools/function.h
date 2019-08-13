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

/// ��ȡϵͳCPU�߼�������
inline int GetCpuCoreNumber()
{
#ifdef WIN32
	SYSTEM_INFO sysInfo;
	GetSystemInfo( &sysInfo );
	return (int)sysInfo.dwNumberOfProcessors;
#else
	//sysconf�������������_SC_NPROCESSORS_CONF��_SC_NPROCESSORS_ONLN�����Ի�ȡϵͳCPU������ 
	//_SC_NPROCESSORS_CONF������ϵͳ���е�CPU���������ֵҲ����ϵͳ�н�ֹ�û�ʹ�õ�CPU������ 
	//	_SC_NPROCESSORS_ONLN������ϵͳ�п��õ�CPU������
	return (int)sysconf(_SC_NPROCESSORS_CONF);
#endif
}