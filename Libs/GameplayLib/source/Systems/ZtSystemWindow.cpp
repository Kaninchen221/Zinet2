#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"

#include "Zinet/Core/Components/ZtExitReason.hpp"

namespace zt::gameplay
{
	namespace system
	{
		using namespace core;

		void Window::Init(ecs::WorldCommands worldCommands)
		{
			wd::GLFW::Init(false);

			wd::Window windowRes;

			if (!windowRes.create(800, 800))
			{
				worldCommands.addResource(ExitReason{ true, "Couldn't create window" });
			}

			worldCommands.addResource(wd::WindowEvents{ windowRes });
 			worldCommands.addResource(windowRes);
		}

		void Window::Update(
			ecs::Resource<wd::Window> windowRes, 
			ecs::Resource<wd::WindowEvents> windowEventsRes, 
			ecs::WorldCommands worldCommands)
		{
			if (!windowRes)
			{
				worldCommands.addResource(ExitReason{ true, "Couldn't find a window resource" });
				return;
			}

			if (!windowEventsRes)
			{
				worldCommands.addResource(ExitReason{ true, "Couldn't find a window events resource" });
				return;
			}
			
			if (windowRes->isOpen())
			{
				windowEventsRes->pollEvents();
			}
			else
			{
				windowRes->requestCloseWindow();
				windowRes->destroyWindow();
				worldCommands.addResource(ExitReason{ true, "Window closed" });
			}
		}

		void Window::Deinit(ecs::Resource<wd::Window> windowRes)
		{
			if (!windowRes)
				return;

			if (windowRes->isOpen())
				windowRes->destroyWindow();

			wd::GLFW::Deinit();
		}
	}
}