cmake_minimum_required(VERSION 2.8)

aux_source_directory(. DIR_ORIENTATION_SRCS)
add_library(orientation ${DIR_ORIENTATION_SRCS})

# set debug mode
set(CMAKE_BUILD_TYPE Debug)

# 安装静态库
install(TARGETS orientation
    ARCHIVE DESTINATION lib/imaging_engine
)