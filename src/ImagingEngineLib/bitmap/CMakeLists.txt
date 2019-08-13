cmake_minimum_required(VERSION 2.8)

aux_source_directory(. DIR_BITMAP_SRCS)
add_library(bitmap ${DIR_BITMAP_SRCS})

# set debug mode
set(CMAKE_BUILD_TYPE Debug)

# 安装静态库
install(TARGETS bitmap
    ARCHIVE DESTINATION lib/imaging_engine
)