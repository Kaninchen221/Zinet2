#pragma once

#pragma warning(disable:4251) // Ignore warning: STD doesn't have dllexport/dllimport
#pragma warning(disable:4275) // Ignore warning: Vulkan RAII doesn't have dllexport/dllimport

// Reflection macros
#define ZT_GENERATED_BODY(...)
#define ZT_REFLECT_NAMESPACE(...)
#define ZT_REFLECT_CLASS(...)
#define ZT_REFLECT_METHOD(...)
#define ZT_REFLECT_MEMBER(...)
#define ZT_REFLECT_STRUCT(...)

// Turn On/Off Debug UI
#define ZINET_DEBUG_UI

#ifdef _DEBUG
#	define ZINET_DEBUG 1
#else
#	define ZINET_DEBUG 0
#endif // _DEBUG

#ifdef _MSC_VER
#	define ZINET_MSVC 1
#else
#	define ZINET_MSVC 0
#endif // _MSC_VER

#ifdef __GNUC__
#	define ZINET_GCC 1
#else
#	define ZINET_GCC 0
#endif // __GNUC__

#ifdef _WIN32
#	define ZINET_WINDOWS 1
#else
#	define ZINET_WINDOWS 0
#endif // _WIN32

#ifdef __linux__
#	define ZINET_LINUX 1
#else
#	define ZINET_LINUX 0
#endif // __linux__

#ifndef ZINET_STATIC

#	if ZINET_WINDOWS
#		define ZINET_API_EXPORT __declspec(dllexport)
#		define ZINET_API_IMPORT __declspec(dllimport)
#	elif ZINET_LINUX
#		define ZINET_API_EXPORT
#		define ZINET_API_IMPORT
#	else
#		error Not supported OS platform
		// We need this for project reflector
#		define ZINET_API_EXPORT
#		define ZINET_API_IMPORT
#	endif // ZINET_WINDOWS

#	ifdef ZINET_LIB
#		define ZINET_CORE_API ZINET_API_EXPORT
#	else
#		define ZINET_CORE_API ZINET_API_IMPORT
#	endif // ZINET_LIB

#else
#	define ZINET_CORE_API
#endif // ZINET_STATIC

// Uncomment for time tracing info 
//#define ZINET_TIME_TRACE

#include "Zinet/Core/ZtDebug.hpp"