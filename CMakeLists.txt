cmake_minimum_required(VERSION 3.10)

# MSVC runtime library flags are selected by an abstraction.
if(POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif()

message(STATUS "CMAKE_VERSION=${CMAKE_VERSION}")

option(BUILD_SHARED_LIBS "Build shared libs" ON)
option(MSVC_STATIC_RUNTIME "Build static runtime" OFF)

# The CMAKE_<LANG>_FLAGS(_<BUILD_TYPE>)? is meant to be user controlled.
# Prior to CMake 3.15, the MSVC runtime library was pushed into the same flags
# making programmatic control difficult.  Prefer the functionality in newer
# CMake versions when available.
if(${CMAKE_VERSION} VERSION_GREATER 3.15 OR ${CMAKE_VERSION} VERSION_EQUAL 3.15)  
	if (MSVC_STATIC_RUNTIME)
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	else()
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
	endif()
	
	message(STATUS "CMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY}")
else()
	# In case we are building static libraries, link also the runtime library statically
	# so that MSVCR*.DLL is not required at runtime.
	# https://msdn.microsoft.com/en-us/library/2kzt1wy3.aspx
	# This is achieved by replacing msvc option /MD with /MT and /MDd with /MTd
	# http://www.cmake.org/Wiki/CMake_FAQ#How_can_I_build_my_MSVC_application_with_a_static_runtime.3F
	if (MSVC AND MSVC_STATIC_RUNTIME)
		message(STATUS "MSVC_STATIC_RUNTIME on")
		set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
		CMAKE_CXX_FLAGS_MINSIZEREL
		CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
		CMAKE_CXX_FLAGS_MINSIZEREL
		CMAKE_C_FLAGS_RELWITHDEBINFO
        )
		foreach(flag_var
			CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
			CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
			if(${flag_var} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
			endif(${flag_var} MATCHES "/MD")
		endforeach(flag_var)
	endif()
endif()

if(MSVC)
	add_definitions(-DUNICODE -D_UNICODE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")
endif()

set(PROJECT_NAME "WinPipe")

project(${PROJECT_NAME} CXX)

set(LIBRARY_TYPE SHARED)
if(NOT BUILD_SHARED_LIBS)
	set(LIBRARY_TYPE STATIC)
endif()

message(STATUS "LIBRARY_TYPE=${LIBRARY_TYPE}")

add_library(
	${PROJECT_NAME}
	${LIBRARY_TYPE}
	src/pipeCmd.pb.cc
	src/pipeCmd.pb.h
	src/PipeWrapper.cpp
	src/PipeWrapper.h
)

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