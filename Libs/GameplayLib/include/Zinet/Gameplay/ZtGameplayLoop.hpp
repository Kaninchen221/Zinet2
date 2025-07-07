#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"

#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtWindowEvents.hpp"

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API GameplayLoop
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("GameplayLoop");

	public:

		GameplayLoop() noexcept = default;
		GameplayLoop(const GameplayLoop& other) noexcept = default;
		GameplayLoop(GameplayLoop&& other) noexcept = default;
		~GameplayLoop() noexcept = default;

		GameplayLoop& operator = (const GameplayLoop& other) noexcept = default;
		GameplayLoop& operator = (GameplayLoop&& other) noexcept = default;

		bool init() noexcept;

		bool shouldLoop() const noexcept { return window.isOpen(); }

		void turnOff() noexcept { window.requestCloseWindow(); }

		void loop() noexcept;

		void shutdown() noexcept;

	protected:

		vulkan_renderer::VulkanRenderer renderer;
		vulkan_renderer::ImGuiIntegration imGuiIntegration;
		wd::Window window;
		wd::WindowEvents windowEvents{ window };

	};
}