#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"

#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtWindowEvents.hpp"

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API Engine
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::Engine");

	public:

		Engine() noexcept = default;
		Engine(const Engine& other) noexcept = default;
		Engine(Engine&& other) noexcept = default;
		~Engine() noexcept = default;

		Engine& operator = (const Engine& other) noexcept = default;
		Engine& operator = (Engine&& other) noexcept = default;

		bool init() noexcept;

		bool shouldLoop() const noexcept { return engineContext.window.isOpen(); }

		void turnOff() noexcept { engineContext.window.requestCloseWindow(); }

		void loop() noexcept;

		void deinit() noexcept;

	protected:

		EngineContext engineContext;


	};
}