#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtWorldCommands.hpp"
#include "Zinet/Core/ECS/ZtResource.hpp"

#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtWindowEvents.hpp"

namespace zt::gameplay::system
{
	class ZINET_GAMEPLAY_API Window
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::system::Window");

	public:

		static void Init(core::ecs::WorldCommands);

		static void Update(
			core::ecs::Resource<wd::Window> windowRes, 
			core::ecs::Resource<wd::WindowEvents> windowEventsRes, 
			core::ecs::WorldCommands worldCommands);

		static void Deinit(core::ecs::Resource<wd::Window> windowRes);

	};
}