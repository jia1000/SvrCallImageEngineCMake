cmake_minimum_required(VERSION 2.8)

aux_source_directory(. DIR_IO_SRCS)
add_library(io ${DIR_IO_SRCS})

# set debug mode
set(CMAKE_BUILD_TYPE Debug)

# 安装静态库
install(TARGETS io
    ARCHIVE DESTINATION lib/imaging_engine
)