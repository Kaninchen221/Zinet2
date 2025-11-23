#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ECS/ZtResource.hpp"

namespace zt::wd
{
	class Window;
}

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API Editor
	{
	public:

		static void EntryPoint(core::ecs::ConstResource<wd::Window> windowRes);

	};
}