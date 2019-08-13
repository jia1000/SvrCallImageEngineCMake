cmake_minimum_required(VERSION 2.8)

aux_source_directory(. DIR_THREAD_SRCS)
add_library(thread ${DIR_THREAD_SRCS})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
else()
	target_link_libraries(thread ${CMAKE_THREAD_LIBS_INIT})
endif()

# set debug mode
set(CMAKE_BUILD_TYPE Debug)

# 安装静态库
install(TARGETS thread
    ARCHIVE DESTINATION lib/imaging_engine
)	