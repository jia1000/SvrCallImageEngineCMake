cmake_minimum_required(VERSION 2.8)

aux_source_directory(. DIR_CONTROL_SRCS)
add_library(control ${DIR_CONTROL_SRCS})

# set debug mode
set(CMAKE_BUILD_TYPE Debug)

# 安装静态库
install(TARGETS control
    ARCHIVE DESTINATION lib/imaging_engine
)