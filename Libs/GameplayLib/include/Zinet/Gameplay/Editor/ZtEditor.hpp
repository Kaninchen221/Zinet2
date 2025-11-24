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
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::Editor");

	public:

		using GraphResT = core::ecs::ConstResource<const core::ecs::Graph*>;

		static void EntryPoint(core::ecs::ConstResource<wd::Window> windowRes,
			GraphResT graphRes);

	private:

		static void MainMenuBar();

		static void ToolsSubMenu();

		inline static bool OpenScheduleGraphWindow = false;
		static void ScheduleGraphWindow(GraphResT graphRes);

	};
}