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

		Engine() ZINET_API_POST = default;
		Engine(const Engine& other) ZINET_API_POST = default;
		Engine(Engine&& other) ZINET_API_POST = default;
		~Engine() ZINET_API_POST = default;

		Engine& operator = (const Engine& other) ZINET_API_POST = default;
		Engine& operator = (Engine&& other) ZINET_API_POST = default;

		bool init() ZINET_API_POST;

		bool shouldLoop() const ZINET_API_POST { return engineContext.window.isOpen(); }

		void turnOff() ZINET_API_POST { engineContext.window.requestCloseWindow(); }

		void loop() ZINET_API_POST;

		void deinit() ZINET_API_POST;

	protected:

		EngineContext engineContext;


	};
}