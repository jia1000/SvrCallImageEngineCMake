# CMakeList.txt: SvrCallImageEngineCMake 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。
# add_executable (SvrCallImageEngineCMake "SvrCallImageEngineCMake.cpp" "SvrCallImageEngineCMake.h")

# TODO: 如有需要，请添加测试并安装目标。

PROJECT ("SvrCallImageEngineCMake")

 # 设置安装目录
 SET(CMAKE_INSTALL_PREFIX /usr/local)

 # 设置对c++11支持
 SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

 # 设置第三方cmake脚本所在目录
 SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

 # 设置编译目录
 SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
 SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
 SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 设置各个项目名称
SET(PROJECT_NAME_APP_EXE 			"appexe")
SET(PROJECT_NAME_CONTROLLER 		"controller")
SET(PROJECT_NAME_DICOM_ENGINE 		"dicom_engine")
SET(PROJECT_NAME_IMAGINGENGINELIB 	"ImagingEngineLib")

# 设置第三方依赖库的名称
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	SET(PROJECT_NAME_GLOG_LIB 	"glogd.lib")
ELSE()
	SET(PROJECT_NAME_GLOG_LIB 	"glog")
ENDIF()


# 设置第三方库的路径
SET(THIRD_LIB_GLOG_INCLUDE_DIRECTORIES   "C:/Program Files (x86)/glog/include")
SET(THIRD_LIB_GLOG_LINK_DIRECTORIES   	 "C:/Program Files (x86)/glog/lib")

# 包含源文件。
ADD_SUBDIRECTORY ("src")