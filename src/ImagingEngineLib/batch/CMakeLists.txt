cmake_minimum_required(VERSION 2.8)

aux_source_directory(. DIR_BATCH_SRCS)
add_library(batch ${DIR_BATCH_SRCS})

# set debug mode
set(CMAKE_BUILD_TYPE Debug)

# 安装静态库
install(TARGETS batch 
    ARCHIVE DESTINATION lib/imaging_engine
)