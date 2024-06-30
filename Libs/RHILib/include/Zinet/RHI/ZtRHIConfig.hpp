#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#ifdef ZINET_USE_OPENGL
#	include <glbinding/gl/gl.h>
#	include <glbinding/glbinding.h>
#endif

#ifdef ZINET_USE_VULCAN
#	define GLFW_INCLUDE_VULKAN
#	include <vulkan/vulkan.h>
#	include <vulkan/vulkan_raii.hpp>
#endif
#	define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifndef ZINET_STATIC
#	ifdef ZINET_WINDOWS
#		define ZINET_API_EXPORT __declspec(dllexport)
#		define ZINET_API_IMPORT __declspec(dllimport)
#	else
#		error Not supported OS platform
#		define ZINET_API_EXPORT
#		define ZINET_API_IMPORT
#	endif // ZINET_WINDOWS

#	ifdef ZINET_LIB
#		define ZINET_RHI_API ZINET_API_EXPORT
#	else
#		define ZINET_RHI_API ZINET_API_IMPORT
#	endif // ZINET_LIB
#else
#	define ZINET_RHI_API
#endif // ZINET_STATIC

// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES

// STB
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifdef IMGUI_STATIC_DEFINE
#	error IMGUI can't be static
#endif
