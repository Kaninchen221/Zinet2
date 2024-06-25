#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

// GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Vulkan
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_raii.hpp>

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
#define ZINET_RHI_API ZINET_API_EXPORT
#else
#define ZINET_RHI_API ZINET_API_IMPORT
#endif // ZINET_LIB


#else

#define ZINET_RHI_API

#endif

// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES

// STB
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// IMGUI
//#undef IMGUI_IMPL_API
//#undef IMGUI_API
//#define IMGUI_IMPL_API ZINET_RHI_API
//#define IMGUI_API ZINET_RHI_API

#ifdef IMGUI_STATIC_DEFINE
#error IMGUI must be shared
#endif
