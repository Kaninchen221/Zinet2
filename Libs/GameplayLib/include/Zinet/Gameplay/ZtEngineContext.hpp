#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"

#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtWindowEvents.hpp"

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API EngineContext
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EngineContext");

	public:

		EngineContext() noexcept = default;
		EngineContext(const EngineContext& other) noexcept = default;
		EngineContext(EngineContext&& other) noexcept = default;
		~EngineContext() noexcept = default;

		EngineContext& operator = (const EngineContext& other) noexcept = default;
		EngineContext& operator = (EngineContext&& other) noexcept = default;

		bool init() noexcept;

		void loop() noexcept;

		void deinit() noexcept;

		SystemRenderer systemRenderer;

		vulkan_renderer::ImGuiIntegration imGuiIntegration;
		wd::Window window;
		wd::WindowEvents windowEvents{ window };

	};
}