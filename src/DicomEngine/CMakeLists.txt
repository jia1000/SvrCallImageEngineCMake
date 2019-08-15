
#  项目名
PROJECT(${PROJECT_NAME_DICOM_ENGINE})

# 设置静态库输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../thirdparty)

## 第三方库 GFLAGS
find_package(GFLAGS)
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	INCLUDE_DIRECTORIES("C:/Program Files (x86)/gflags/include")
	LINK_DIRECTORIES("C:/Program Files (x86)/gflags/lib")
ELSE()
	# do nothing
ENDIF()


## 第三方库 GLOG   GLOG_INCLUDE_DIRS})#这个宏不起作用
find_package(GLOG 0.4.0)
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	INCLUDE_DIRECTORIES(${THIRD_LIB_GLOG_INCLUDE_DIRECTORIES})
	LINK_DIRECTORIES( ${THIRD_LIB_GLOG_LINK_DIRECTORIES})
ELSE()
	# do nothing
ENDIF()


## 第三方库 DCMTK
find_package(DCMTK 3.6.2)
link_libraries(${DCMTK_LIBRARIES})
INCLUDE_DIRECTORIES(${DCMTK_INCLUDE_DIRS})

# 添加 DicomEngine 项目文件
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/api 				DIRSRCSdicom_engine)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/io					DIRSRCSdicom_engine)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/iod  				DIRSRCSdicom_engine)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/api/dicom 			DIRSRCSdicom_engine)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/main/controllers  	DIRSRCSdicom_engine)

# 设置头文件搜索目录
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
# 包含"tools/logger.h"
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../)

# 生成动态库
ADD_LIBRARY(${PROJECT_NAME} STATIC ${DIRSRCSdicom_engine})

# 增加依赖库
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${DCMTK_LIBRARIES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_NAME_GLOG_LIB})

# 为windows添加宏
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	ADD_DEFINITIONS(-DBASE_EXPORTS)
ELSE()
ENDIF()





