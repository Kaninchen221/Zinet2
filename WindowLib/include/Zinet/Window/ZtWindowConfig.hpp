#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

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
#define ZINET_WINDOW_LAYER_API ZINET_API_EXPORT
#else
#define ZINET_WINDOW_LAYER_API ZINET_API_IMPORT
#endif // ZINET_LIB


#else

#define ZINET_WINDOW_LAYER_API

#endif