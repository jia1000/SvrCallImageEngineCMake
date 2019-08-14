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
LINK_DIRECTORIES("C:/Program Files (x86)/DCMTK/lib")

## 第三方库 VTK
find_package(VTK 7.1.0)
#find_package(VTK 8.0 REQUIRED)  # Linux上使用vtk8.0 （8.0需要使用vs2013以上编译)
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

# 指定生成目标
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME_CONTROLLER})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_NAME_DICOM_ENGINE})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ImagingEngineLib.lib)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_NAME_GLOG_LIB})