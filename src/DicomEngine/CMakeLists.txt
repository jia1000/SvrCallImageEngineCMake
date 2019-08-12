
#  项目名
PROJECT(dicom_engine)

# 源文件
# FILE(GLOB DIRSRCSdicom_engine "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
# 添加 DicomEngine 项目文件
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/api/*.cpp 			DIRSRCSdicom_engine)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/io/*.cpp 			DIRSRCSdicom_engine)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/iod/*.cpp  			DIRSRCSdicom_engine)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/api/dicom/*.cpp  		DIRSRCSdicom_engine)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/main/controllers/*.cpp  	DIRSRCSdicom_engine)

# 增加动态库
ADD_LIBRARY(${PROJECT_NAME} SHARED ${DIRSRCSdicom_engine})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${DCMTK_LIBRARIES})

# 为windows添加宏
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	ADD_DEFINITION(-DBASE_EXPORTS)
ENDIF()

# 设置头文件搜索目录
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src)

