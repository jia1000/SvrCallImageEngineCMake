cmake_minimum_required(VERSION 2.8)

aux_source_directory(. DIR_TOOLS_SRCS)
add_library(tools ${DIR_TOOLS_SRCS})

# set debug mode
set(CMAKE_BUILD_TYPE Debug)

# 安装静态库
install(TARGETS tools
    ARCHIVE DESTINATION lib/imaging_engine
)	