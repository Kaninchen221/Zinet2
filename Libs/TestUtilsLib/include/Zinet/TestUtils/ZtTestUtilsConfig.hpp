#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#ifndef ZINET_STATIC
#	if ZINET_WINDOWS
#		define ZINET_API_EXPORT __declspec(dllexport)
#		define ZINET_API_IMPORT __declspec(dllimport)
#	elif ZINET_LINUX
#		define ZINET_API_EXPORT
#		define ZINET_API_IMPORT
#	else
#		error Not supported OS platform
#		define ZINET_API_EXPORT
#		define ZINET_API_IMPORT
#	endif // ZINET_WINDOWS

#	ifdef ZINET_LIB
#		define ZINET_TEST_UTILS_API ZINET_API_EXPORT
#	else
#		define ZINET_TEST_UTILS_API ZINET_API_IMPORT
#	endif // ZINET_LIB
#else
#	define ZINET_TEST_UTILS_API
#endif // ZINET_STATIC