#pragma once

#pragma warning(disable:4100) // Ignore warning: Method param is not referenced
#pragma warning(disable:4251) // Ignore warning: STD doesn't have dllexport/dllimport
#pragma warning(disable:4275) // Ignore warning: Vulkan RAII doesn't have dllexport/dllimport

#ifndef ZINET_STATIC

	#ifdef ZINET_WINDOWS
		#define ZINET_API_EXPORT __declspec(dllexport)
		#define ZINET_API_IMPORT __declspec(dllimport)
	#else
		#error Not supported OS platform
		// We need this for project reflector
		#define ZINET_API_EXPORT
		#define ZINET_API_IMPORT
	#endif // ZINET_WINDOWS

	#ifdef ZINET_LIB
		#define ZINET_CORE_API ZINET_API_EXPORT
	#else
		#define ZINET_CORE_API ZINET_API_IMPORT
	#endif // ZINET_LIB


#else

	#define ZINET_CORE_API

#endif // ZINET_STATIC

#define ZT_GENERATED_BODY(...)
#define ZT_REFLECT_NAMESPACE(...)
#define ZT_REFLECT_CLASS(...)
#define ZT_REFLECT_METHOD(...)
#define ZT_REFLECT_MEMBER(...)
#define ZT_REFLECT_STRUCT(...)

// Turn On/Off Debug UI
#define ZINET_DEBUG_UI

#ifdef _DEBUG
	#define ZINET_DEBUG 1
#elif
	#define ZINET_DEBUG 0
#endif // _DEBUG

#ifdef _MSC_VER
	#define ZINET_MSVC 1
#else
	#define ZINET_MSVC 0
#endif // _MSC_VER

#ifdef __GNUC__
	#define ZINET_GCC 1
#else
	#define ZINET_GCC 0
#endif // __GNUC__

#include "Zinet/Core/ZtDebug.hpp"