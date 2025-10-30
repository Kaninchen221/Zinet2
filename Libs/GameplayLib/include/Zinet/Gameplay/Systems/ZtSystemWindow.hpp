#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"
#include "Zinet/Core/ECS/ZtSystemReturnState.hpp"

#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtWindowEvents.hpp"

namespace zt::gameplay::system
{
	class ZINET_GAMEPLAY_API Window
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::system::Window");

	public:

		static core::ecs::SystemReturnState Init(core::ecs::World& world);

		static core::ecs::SystemReturnState Update(core::ecs::World& world);

		static core::ecs::SystemReturnState Deinit(core::ecs::World& world);

	};
}