
#  项目名
PROJECT(dicom_engine)

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

# 添加 DicomEngine 项目文件
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/api 				DIRSRCSdicom_engine)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/io					DIRSRCSdicom_engine)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/iod  				DIRSRCSdicom_engine)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/api/dicom 			DIRSRCSdicom_engine)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/main/controllers  	DIRSRCSdicom_engine)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/tools				DIRSRCSdicom_engine)

# 设置头文件搜索目录
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# 生成动态库
ADD_LIBRARY(${PROJECT_NAME} SHARED ${DIRSRCSdicom_engine})

# 增加依赖库
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${DCMTK_LIBRARIES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} glogd.lib)

# 为windows添加宏
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	ADD_DEFINITIONS(-DBASE_EXPORTS)
ENDIF()





