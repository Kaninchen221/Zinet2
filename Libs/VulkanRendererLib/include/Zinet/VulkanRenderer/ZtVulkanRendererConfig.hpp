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

#if ZINET_LINUX
#	define VK_USE_PLATFORM_WAYLAND_KHR
#	define GLFW_EXPOSE_NATIVE_WAYLAND
#elif ZINET_WINDOWS
#	define VK_USE_PLATFORM_WIN32_KHR
#	define GLFW_EXPOSE_NATIVE_WIN32
#else
#	error "Not supported OS"
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <exception>

// Override VMA_ASSERT
namespace zt::vulkan_renderer
{
    inline void throwException(bool exprResult, const char* exprText)
    {
        if (!exprResult)
        {
            throw std::exception(exprText);
        }
    }
}

#define VMA_ASSERT(_EXPR) do { zt::vulkan_renderer::throwException(_EXPR, #_EXPR); } while (0)