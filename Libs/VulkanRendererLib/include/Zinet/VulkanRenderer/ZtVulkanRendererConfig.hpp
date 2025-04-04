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
#		define ZINET_VULKAN_RENDERER_API ZINET_API_EXPORT
#	else
#		define ZINET_VULKAN_RENDERER_API ZINET_API_IMPORT
#	endif // ZINET_LIB
#else
#	define ZINET_VULKAN_RENDERER_API
#endif // ZINET_STATIC

// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES

#if ZINET_LINUX
#	define VK_USE_PLATFORM_WAYLAND_KHR
#elif ZINET_WINDOWS
#	define VK_USE_PLATFORM_WIN32_KHR
#else
#	error "Not supported OS"
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>