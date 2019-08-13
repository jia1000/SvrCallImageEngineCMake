/*=========================================================================

  Program:   ImagingEngine
  Module:    data_definition.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

// Windows 头文件: 
#ifdef WIN32
#include <windows.h>
#else

#endif
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
//#include <tchar.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <stdint.h>
#include <string>
#include "tools/logger.h"
#include "global_define.h"

#define UNITDATA3D			unsigned char
#define UNITMARK3D			unsigned char
#define UNITDATASHOW		char
#define MAXIMIUM_CT_VALUE	4096
#define MAXIMIUM_THREADS	16

#ifdef WIN32

#else
typedef unsigned char	BYTE;
typedef int				BOOL;
typedef unsigned long	DWORD;
typedef unsigned short	WORD;
#endif

using namespace std;


struct Point3d {
	float x;
	float y;
	float z;
};

struct Point3f {
	float x;
	float y;
	float z;
};

struct Point3i {
	int x;
	int y;
	int z;
};

struct Point2 {
	int x;
	int y;
};