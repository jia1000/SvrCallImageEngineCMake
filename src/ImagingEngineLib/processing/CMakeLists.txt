cmake_minimum_required(VERSION 2.8)

aux_source_directory(. DIR_PROCESSING_SRCS)
add_library(processing ${DIR_PROCESSING_SRCS})

# set debug mode
set(CMAKE_BUILD_TYPE Debug)

# 安装静态库
install(TARGETS processing
    ARCHIVE DESTINATION lib/imaging_engine
)