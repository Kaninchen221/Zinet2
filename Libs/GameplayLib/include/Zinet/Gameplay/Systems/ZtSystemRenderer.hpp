#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

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

		static void Init(core::ecs::World& world);

		static void Update(core::ecs::World& world);

		static void Deinit(core::ecs::World& world);
	};

}