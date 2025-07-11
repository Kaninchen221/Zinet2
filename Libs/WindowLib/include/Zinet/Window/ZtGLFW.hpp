#pragma once

#include "Zinet/Window/ZtWindowConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::wd
{
	class ZINET_WINDOW_LAYER_API GLFW
	{

	protected:

		inline static core::ConsoleLogger Logger = zt::core::ConsoleLogger::Create("GLFW");

	public:

		GLFW() { Init(); }
		GLFW(const GLFW& other) = default;
		GLFW(GLFW&& other) = default;

		GLFW& operator = (const GLFW& other) = default;
		GLFW& operator = (GLFW&& other) = default;

		~GLFW() ZINET_API_POST { Deinit(); };

		static bool Init(bool hideWindow = true);
		static void Deinit();

		static bool IsInitialized();

		static void HideWindow();
		static void UnhideWindow();

		static bool IsVulkanSupported();

	private:

		static void ErrorCallback(int errorCode, const char* errorDescription);

		inline static bool Initialized = false;

	};
}