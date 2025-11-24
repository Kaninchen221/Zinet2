#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ECS/ZtResource.hpp"
#include "Zinet/Core/ECS/ZtSchedule.hpp"

namespace zt::wd
{
	class Window;
}

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API Editor
	{
	public:

		static void EntryPoint(core::ecs::ConstResource<wd::Window> windowRes,
			core::ecs::ConstResource<core::ecs::Graph> graphRes);

	private:

		static void MainMenuBar();

		static void ToolsSubMenu();

		inline static bool OpenScheduleGraphWindow = false;
		static void ScheduleGraphWindow(core::ecs::ConstResource<core::ecs::Graph> graphRes);

	};
}