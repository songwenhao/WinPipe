cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "WinPipe")

project(${PROJECT_NAME})

add_library(
	${PROJECT_NAME}
	STATIC
	src/pipeCmd.pb.cc
	src/pipeCmd.pb.h
	src/PipeWrapper.cpp
	src/PipeWrapper.h
)

target_include_directories(
	${PROJECT_NAME}
    PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/protobuf/libs/includes
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	message(STATUS "Target is 64 bits")
    target_link_directories(
		${PROJECT_NAME}
		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/protobuf/libs/x64
	)
    
else()
	message(STATUS "Target is 32 bits")
    target_link_directories(
		${PROJECT_NAME}
		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/protobuf/libs/Win32
	)
endif()