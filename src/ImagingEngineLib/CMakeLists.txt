# CMake build script for ImagingEngineLib

# Requires version
cmake_minimum_required (VERSION 2.8)

add_definitions(-std=c++11)


# project name
project (ImagingEngineLib)

# 设置编译器选项：O3优化，显示所有警告
set(CMAKE_C_FLAGS "$ENV{CFLAGS} -O3 -Wall") 
# 定义头文件的路径变量
set(INCLUDE_PATH ${PROJECT_SOURCE_DIR}/include)
# 定义源文件路径变量
set(SOURCE_PATH ${PROJECT_SOURCE_DIR}/src) 
# 设置静态库输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../thirdparty)


#########################################################################################################
## 第三方库 GFLAGS
find_package(GFLAGS)
INCLUDE_DIRECTORIES("C:/Program Files (x86)/gflags/include")
LINK_DIRECTORIES("C:/Program Files (x86)/gflags/lib")

## 第三方库 GLOG   GLOG_INCLUDE_DIRS})#这个宏不起作用
find_package(GLOG 0.4.0)
INCLUDE_DIRECTORIES("C:/Program Files (x86)/glog/include")
LINK_DIRECTORIES("C:/Program Files (x86)/glog/lib")

# >>引用VTK
find_package(VTK 7.1 REQUIRED)
#find_package(VTK 8.0 REQUIRED)  # Linux上使用vtk8.0 （8.0需要使用vs2013以上编译)
#find_package(glog REQUIRED)
find_package (Threads)

IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
ELSE()
#	vtk_module_config(VTK
#	  vtkCommonCore
#	  vtkIOImage
#	  vtkIOXML
#	  vtkInteractionWidgets
#	  vtkRenderingVolume${VTK_RENDERING_BACKEND}
#	)
ENDIF()
#########################################################################################################


#########################################################################################################
# >>添加头文件include路径，也就是 gcc -I xxx 的意思，或者vs ide中添加头文件包含目录
include(${VTK_USE_FILE})
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../common_utils)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

ELSE()
	# 添加头文件目录
	include_directories(
		../common_utils
		${PROJECT_SOURCE_DIR}/include
		/usr/local/include/glog
		)

	LINK_DIRECTORIES(
		/usr/lib
		${PROJECT_SOURCE_DIR}/lib
		)
ENDIF()
#########################################################################################################

#########################################################################################################
# >>添加源文件路径下所有源文件存放到变量中(*.c && *.cpp)，当然也可以手动一个个文件添加进来
aux_source_directory(. SRC_LIST) 
aux_source_directory(./batch SRC_LIST) 
aux_source_directory(./control SRC_LIST) 
aux_source_directory(./data SRC_LIST) 
aux_source_directory(./filter SRC_LIST) 
aux_source_directory(./io SRC_LIST) 
aux_source_directory(./orientation SRC_LIST) 
aux_source_directory(./processing SRC_LIST) 
aux_source_directory(./render SRC_LIST) 
aux_source_directory(./tools SRC_LIST) 
aux_source_directory(./bitmap SRC_LIST) 
aux_source_directory(./thread SRC_LIST) 
aux_source_directory(./exception SRC_LIST) 

# >>给编译器添加库目录，也就是 gcc -L xxx 的意思，或者vs ide中添加库的包含目录
#link_directories()

# 加fPIC选项的 源文件对于，它引用的函数头文件编写有很宽松的尺度。
add_compile_options(-fPIC)

# >>生成静态库
#add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
else()
	target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
endif()

# set debug mode
set(CMAKE_BUILD_TYPE Debug)

#########################################################################################################


#########################################################################################################
# >>链接到输出的lib库
# 给目标添加依赖库，类似与gcc -l lib_name，此指令有两个用处，一个是给可执行target_name 添加库依赖，二是给库target_name 添加库依赖
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})

IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	target_link_libraries(${PROJECT_NAME} glogd.lib)
ELSE()
	target_link_libraries(${PROJECT_NAME} libuuid.so)
	target_link_libraries(${PROJECT_NAME} /usr/local/lib/libgflags.so)
	target_link_libraries(${PROJECT_NAME} /usr/local/lib/libglog.so)

ENDIF()
