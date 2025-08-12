#pragma once

#include "Zinet/Window/ZtWindowConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::wd
{
	class GLFW
	{

	protected:

		inline static core::ConsoleLogger Logger = zt::core::ConsoleLogger::Create("GLFW");

	public:

		GLFW() { Init(); }
		ZINET_WINDOW_LAYER_API GLFW(const GLFW& other) noexcept = default;
		ZINET_WINDOW_LAYER_API GLFW(GLFW&& other) noexcept = default;
		~GLFW() { Deinit(); }

		ZINET_WINDOW_LAYER_API GLFW& operator = (const GLFW& other) noexcept = default;
		ZINET_WINDOW_LAYER_API GLFW& operator = (GLFW&& other) noexcept = default;

		ZINET_WINDOW_LAYER_API static bool Init(bool hideWindow = true);
		ZINET_WINDOW_LAYER_API static void Deinit();

		ZINET_WINDOW_LAYER_API static bool IsInitialized();

		ZINET_WINDOW_LAYER_API static void HideWindow();
		ZINET_WINDOW_LAYER_API static void UnhideWindow();

		ZINET_WINDOW_LAYER_API static bool IsVulkanSupported();

	private:

		static void ErrorCallback(int errorCode, const char* errorDescription);

		inline static bool Initialized = false;

	};
}