# CMake build script for ImagingEngineLib

# Requires version
cmake_minimum_required (VERSION 2.8)

add_definitions(-std=c++11)


# project name
project (${PROJECT_NAME_IMAGINGENGINELIB})

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
#########################################################################################################
# >>引用VTK
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	find_package(VTK 7.1.0 REQUIRED)
ELSE()
	find_package(VTK 8.0 REQUIRED)  # Linux上使用vtk8.0 （8.0需要使用vs2013以上编译)
ENDIF()

#find_package(glog REQUIRED)
find_package (Threads)

IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	# do nothing
ELSE()
	vtk_module_config(VTK
	  vtkCommonCore
	  vtkIOImage
	  vtkIOXML
	  vtkInteractionWidgets
	  vtkRenderingVolume${VTK_RENDERING_BACKEND}
	)
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
# >>调用xxx子目录的CMakeLists.txt执行
##add_subdirectory(actor)
##add_subdirectory(builder)
#add_subdirectory(batch)
#add_subdirectory(control)
#add_subdirectory(data)
#add_subdirectory(filter)
#add_subdirectory(io)
#add_subdirectory(orientation)
#add_subdirectory(processing)
#add_subdirectory(render)
#add_subdirectory(tools)
#add_subdirectory(bitmap)
#add_subdirectory(thread)
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
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	add_library(${PROJECT_NAME} STATIC ${SRC_LIST})
ELSE()
	add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
ENDIF()


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
	target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME_GLOG_LIB})
ELSE()
	target_link_libraries(${PROJECT_NAME} libuuid.so)
	target_link_libraries(${PROJECT_NAME} /usr/local/lib/libgflags.so)
	target_link_libraries(${PROJECT_NAME} /usr/local/lib/libglog.so)

ENDIF()

#########################################################################################################


# 但是可以通过下面的命令更改静态库target生成的库名，这样就和动态库的名字一样的了
#SET_TARGET_PROPERTIES(mymath_static PROPERTIES OUTPUT_NAME "mymath") 


#########################################################################################################
# CAMKE INSTALL 指令格式
# >>目标文件的安装
#INSTALL(TARGETS targets...
#        [[ARCHIVE|LIBRARY|RUNTIME]
#                   [DESTINATION <dir>]
#                   [PERMISSIONS permissions...]
#                   [CONFIGURATIONS
#        [Debug|Release|...]]
#                   [COMPONENT <component>]
#                   [OPTIONAL]
#                ] [...])
# 安装静态库
#install(TARGETS ImagingEngineLib
#    ARCHIVE DESTINATION lib/imaging_engine
#)
# 安装动态库
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	# do nothing
ELSE()
	install(TARGETS ImagingEngineLib
		LIBRARY DESTINATION lib/imaging_engine
	)
ENDIF()


# >>普通文件的安装
# 可用于安装一般文件，并可以指定访问权限，文件名是此指令所在路径下的相对路径。
# 如果默认不定义权限PERMISSIONS，安装后的权限为,OWNER_WRITE,OWNER_READ,
# GROUP_READ,和WORLD_READ，即644权限。
#INSTALL(FILES files... DESTINATION <dir>
#         [PERMISSIONS permissions...]
#         [CONFIGURATIONS [Debug|Release|...]]
#         [COMPONENT <component>]
#         [RENAME <name>] [OPTIONAL])


# >>非目标文件的可执行程序安装(比如脚本之类) 
#INSTALL(PROGRAMS files... DESTINATION <dir>
#     [PERMISSIONS permissions...]
#     [CONFIGURATIONS [Debug|Release|...]]
#     [COMPONENT <component>]
#     [RENAME <name>] [OPTIONAL])

# >>目录的安装
#INSTALL(DIRECTORY dirs... DESTINATION <dir>
#     [FILE_PERMISSIONS permissions...]
#     [DIRECTORY_PERMISSIONS permissions...]
#     [USE_SOURCE_PERMISSIONS]
#     [CONFIGURATIONS [Debug|Release|...]]
#     [COMPONENT <component>]
#     [[PATTERN <pattern> | REGEX <regex>]
#      [EXCLUDE] [PERMISSIONS permissions...]] [...])

# 如果目录名不以/结尾，那么这个目录将被安装为目标路径下的abc，如果目录名以/结尾， 
# 代表将这个目录中的内容安装到目标路径，但不包括这个目录本身
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	# do nothing
ELSE()
	install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ 
		DESTINATION include/imaging_engine
		)
ENDIF()
#########################################################################################################


