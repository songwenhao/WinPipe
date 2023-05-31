cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "WinPipe")

project(${PROJECT_NAME})

add_library(
	${PROJECT_NAME}
	SHARED
	src/pipeCmd.pb.cc
	src/pipeCmd.pb.h
	src/PipeWrapper.cpp
	src/PipeWrapper.h
)
  
if(MSVC)
	add_definitions(-DUNICODE -D_UNICODE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")
endif()
  
target_include_directories(
	${PROJECT_NAME}
    PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/protobuf/libs/includes
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	message(STATUS "Target is 64 bits")
    target_link_directories(
		${PROJECT_NAME}
		PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/protobuf/libs/x64
	)
    
else()
	message(STATUS "Target is 32 bits")
    target_link_directories(
		${PROJECT_NAME}
		PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/protobuf/libs/Win32
	)
endif()