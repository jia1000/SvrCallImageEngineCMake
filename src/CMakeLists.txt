cmake_minimum_required(VERSION 2.8.12)
set (CMAKE_CXX_STANDARD 11)

# project name
project (${PROJECT_NAME_APP_EXE})

## 第三方库 GFLAGS
find_package(GFLAGS)
INCLUDE_DIRECTORIES("C:/Program Files (x86)/gflags/include")
LINK_DIRECTORIES("C:/Program Files (x86)/gflags/lib")

## 第三方库 GLOG   GLOG_INCLUDE_DIRS})#这个宏不起作用
find_package(GLOG 0.4.0)
INCLUDE_DIRECTORIES(${THIRD_LIB_GLOG_INCLUDE_DIRECTORIES})
LINK_DIRECTORIES( ${THIRD_LIB_GLOG_LINK_DIRECTORIES})

## 第三方库 DCMTK
find_package(DCMTK 3.6.2)
link_libraries(${DCMTK_LIBRARIES})
INCLUDE_DIRECTORIES(${DCMTK_INCLUDE_DIRS})
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	LINK_DIRECTORIES("C:/Program Files (x86)/DCMTK/lib")
ENDIF()

## 第三方库 VTK
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	find_package(VTK 7.1.0)
ELSE()
	find_package(VTK 8.0 REQUIRED)  # Linux上使用vtk8.0 （8.0需要使用vs2013以上编译)
	vtk_module_config(VTK
	  vtkCommonCore
	  vtkIOImage
	  vtkIOXML
	  vtkInteractionWidgets
	  vtkRenderingVolume${VTK_RENDERING_BACKEND}
	)
	# >>添加头文件include路径，也就是 gcc -I xxx 的意思，或者vs ide中添加头文件包含目录
	include(${VTK_USE_FILE})
ENDIF()
link_libraries(${VTK_LIBRARIES})
INCLUDE_DIRECTORIES(${VTK_INCLUDE_DIRS})

# 包含子项目
ADD_SUBDIRECTORY("DicomEngine")
ADD_SUBDIRECTORY("EngineController")
ADD_SUBDIRECTORY("ImagingEngineLib")

# 包含DicomEngine项目路径
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/DicomEngine)
# 包含ImagingEngineLib项目路径
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/ImagingEngineLib/include)
# 包含common_utils文件夹
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/common_utils)

# 包含lib/debug文件夹下的各个项目库
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../lib/debug)

# jni.h文件的依赖
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	# do nothing
ELSE()
	#include_directories(/usr/local/include/jdk1.8.0_211/include/)
	#include_directories(/usr/local/include/jdk1.8.0_211/include/linux/)
	include_directories(/opt/jdk1.8.0_151/include/)
	include_directories(/opt/jdk1.8.0_151/include/linux/)
ENDIF()

# 设置依赖库的路径
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	# do nothing
ELSE()
	link_directories(./thirdparty/)
	# 添加头文件目录
	include_directories(
		${PROJECT_SOURCE_DIR}/include
		/usr/local/include/glog
		# google breakpad dump
		#${PROJECT_SOURCE_DIR}/thirdparty/breakpad-google/src/
		)
	LINK_DIRECTORIES(
		/usr/lib
		${PROJECT_SOURCE_DIR}/lib
		# google breakpad dump
		#${PROJECT_SOURCE_DIR}/thirdparty/breakpad-google/src/client/linux/
		)
ENDIF()
	
# 指定生成目标
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME_CONTROLLER})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_NAME_DICOM_ENGINE})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ImagingEngineLib.lib)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_NAME_GLOG_LIB})

# 开启调试信息

IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	# do nothing
ELSE()
	set(CMAKE_BUILD_TYPE "Debug")
	set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
	set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
	# 开启所有警告
	set(CMAKE_CXX_FLAGS "-Wall")
ENDIF()



IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	# do nothing
ELSE()
	# 生成SvrCallImageEngine库
	add_compile_options(-fPIC)
	add_library(SvrCallImageEngine SHARED SvrJava/com_deepwise_cta_component_rebuild_SvrCallImageEngine.cpp)
	TARGET_LINK_LIBRARIES(SvrCallImageEngine  controller) 
	# google breakpad dump
	# TARGET_LINK_LIBRARIES(SvrCallImageEngine  breakpad_client.a) 
ENDIF()