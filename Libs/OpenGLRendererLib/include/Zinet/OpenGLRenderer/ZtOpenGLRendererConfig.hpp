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
#		define ZINET_OPENGL_RENDERER_API ZINET_API_EXPORT
#	else
#		define ZINET_OPENGL_RENDERER_API ZINET_API_IMPORT
#	endif // ZINET_LIB
#else
#	define ZINET_OPENGL_RENDERER_API
#endif // ZINET_STATIC

// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES

// Glad
#define GLAD_API_CALL_EXPORT
#include "glad/gl.hpp"

// GLFW
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>