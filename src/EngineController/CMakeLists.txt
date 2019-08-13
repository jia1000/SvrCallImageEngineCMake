


#  项目名
PROJECT(controller)

cmake_minimum_required(VERSION 2.8.12)
set (CMAKE_CXX_STANDARD 11)

## 第三方库 GFLAGS
find_package(GFLAGS)
INCLUDE_DIRECTORIES("C:/Program Files (x86)/gflags/include")
LINK_DIRECTORIES("C:/Program Files (x86)/gflags/lib")

## 第三方库 GLOG   GLOG_INCLUDE_DIRS})#这个宏不起作用
find_package(GLOG 0.4.0)
INCLUDE_DIRECTORIES("C:/Program Files (x86)/glog/include")
LINK_DIRECTORIES("C:/Program Files (x86)/glog/lib")

## 第三方库 DCMTK
find_package(DCMTK 3.6.2)
link_libraries(${DCMTK_LIBRARIES})
INCLUDE_DIRECTORIES(${DCMTK_INCLUDE_DIRS})

## 第三方库 VTK
find_package(VTK 7.1.0)
#find_package(VTK 8.0 REQUIRED)  # Linux上使用vtk8.0 （8.0需要使用vs2013以上编译)
link_libraries(${VTK_LIBRARIES})
INCLUDE_DIRECTORIES(${VTK_INCLUDE_DIRS})

# 添加 controller 项目文件
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} 	DIRSRCS_Controller)

# 设置头文件搜索目录
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# 生成动态库
#ADD_LIBRARY(${PROJECT_NAME} SHARED ${DIRSRCS_Controller})
ADD_LIBRARY(${PROJECT_NAME} STATIC ${DIRSRCS_Controller})
# 包含"tools/logger.h"
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../)
# 包含DicomEngine项目路径
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../DicomEngine)
# 包含ImagingEngineLib项目路径
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../ImagingEngineLib/include)
# 包含common_utils文件夹
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../common_utils)

# 依赖其他项目lib路径
link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty)


# 增加依赖库
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${DCMTK_LIBRARIES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} dicom_engine)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ImagingEngineLib)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} glogd.lib)

# 为windows添加宏
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	ADD_DEFINITIONS(-DBASE_EXPORTS)
ENDIF()





