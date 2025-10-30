#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtSystemReturnState.hpp"

namespace zt::core::ecs
{
	class World;
}

namespace zt::gameplay::system
{
	class ZINET_GAMEPLAY_API Renderer
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::system::Renderer");
	
	public:

		static core::ecs::SystemReturnState Init(core::ecs::World& world);

		static core::ecs::SystemReturnState Update(core::ecs::World& world);

		static core::ecs::SystemReturnState Deinit(core::ecs::World& world);
	};

}