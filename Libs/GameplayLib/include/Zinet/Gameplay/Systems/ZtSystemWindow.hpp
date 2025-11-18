#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"

#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtWindowEvents.hpp"

namespace zt::gameplay::system
{
	class ZINET_GAMEPLAY_API Window
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::system::Window");

	public:

		static void Init(core::ecs::World& world);

		static void Update(core::ecs::World& world);

		static void Deinit(core::ecs::World& world);

	};
}