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
	class  Engine
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::Engine");

	public:

		Engine() = default;
		Engine(const Engine& other) = default;
		Engine(Engine&& other) noexcept = default;
		~Engine() noexcept = default;

		Engine& operator = (const Engine& other) = default;
		Engine& operator = (Engine&& other) noexcept = default;

		bool init();

		bool shouldLoop() const { return engineContext.shouldLoop(); }

		void turnOff() { engineContext.turnOff(); }

		void loop();

		void deinit();

	protected:

		EngineContext engineContext;


	};
}