cmake_minimum_required(VERSION 3.10)

# MSVC runtime library flags are selected by an abstraction.
if(POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif()

message(STATUS "CMAKE_VERSION=${CMAKE_VERSION}")

option(BUILD_SHARED_LIBS "Build shared libs" ON)
option(MSVC_STATIC_RUNTIME "Build static runtime" OFF)
option(NO_PROTOBUF "Use google protobuf" OFF)

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

set(PROJECT_NAME "WinPipe")

project(${PROJECT_NAME} CXX)

set(LIBRARY_TYPE SHARED)

set(DEFINITIONS "-DUNICODE -D_UNICODE")

if(BUILD_SHARED_LIBS)    
    set(DEFINITIONS ${DEFINITIONS} -DWinPipe_EXPORTS)
    
    if(NO_PROTOBUF)
        set(DEFINITIONS ${DEFINITIONS} -DNO_PROTOBUF)
    else()
        set(DEFINITIONS ${DEFINITIONS} -DPROTOBUF_USE_DLLS)
    endif()  
else()
    set(LIBRARY_TYPE STATIC)
    
    set(DEFINITIONS ${DEFINITIONS} -DWinPipe_STATIC)
    
    if(NO_PROTOBUF)
        set(DEFINITIONS ${DEFINITIONS} -DNO_PROTOBUF)
    endif()  
endif()

if(MSVC)
    add_definitions(${DEFINITIONS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")
endif()

set(SOURCE_FILES
    src/PipeWrapper.cpp
    src/PipeWrapper.h
)

if(NOT NO_PROTOBUF)
    message(STATUS "Build use protobuf")
    list(APPEND SOURCE_FILES
        src/pipeCmd.pb.cc
        src/pipeCmd.pb.h
    )
endif()

message(STATUS ${SOURCE_FILES})

add_library(
	${PROJECT_NAME}
	${LIBRARY_TYPE}
	${SOURCE_FILES}
)

if(NOT NO_PROTOBUF)
    set(PROTOBUF_LIB_PATH "")
    
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        message(STATUS "Target is 64 bits")
        
        if(BUILD_SHARED_LIBS)
            set(PROTOBUF_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/libs/x64)
        else()
            set(PROTOBUF_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/libs/StaticRuntime/x64)
        endif()
        
    else()
        message(STATUS "Target is 32 bits")
        
        if(BUILD_SHARED_LIBS)
            set(PROTOBUF_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/libs/Win32)
        else()
            set(PROTOBUF_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/libs/StaticRuntime/Win32)
        endif()
    endif()
    
    target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/libs/includes
    )
    
    target_link_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${PROTOBUF_LIB_PATH}
    )
endif()