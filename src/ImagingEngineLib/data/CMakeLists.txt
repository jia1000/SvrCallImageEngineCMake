cmake_minimum_required(VERSION 2.8)

aux_source_directory(. DIR_DATA_SRCS)
add_library(data ${DIR_DATA_SRCS})

# set debug mode
set(CMAKE_BUILD_TYPE Debug)

# 安装静态库
install(TARGETS data
    ARCHIVE DESTINATION lib/imaging_engine
)