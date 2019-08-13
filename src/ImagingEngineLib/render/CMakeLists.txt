cmake_minimum_required(VERSION 2.8)

aux_source_directory(. DIR_RENDER_SRCS)
add_library(render ${DIR_RENDER_SRCS})

# set debug mode
set(CMAKE_BUILD_TYPE Debug)

# 安装静态库
install(TARGETS render
    ARCHIVE DESTINATION lib/imaging_engine
)