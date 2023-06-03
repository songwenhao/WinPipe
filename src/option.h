#pragma once

#ifndef WinPipe_API
	#ifdef WinPipe_STATIC
		#define WinPipe_API
	#else
		#ifndef PROTOBUF_USE_DLLS
			#define PROTOBUF_USE_DLLS
		#endif
		#ifdef WinPipe_EXPORTS		
			#define WinPipe_API __declspec(dllexport)
		#else
			#define WinPipe_API __declspec(dllimport)
		#endif
	#endif
#endif