#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"

#include "Zinet/Core/Components/ZtExitReason.hpp"

namespace zt::gameplay
{
	namespace system
	{
		using namespace core;

		void Window::Init(ecs::World& world)
		{
			wd::GLFW::Init(false);

			auto windowRes = world.addResource(wd::Window{});
			if (!windowRes)
			{
				Logger->error("Couldn't find a window resource");
				return;
			}

			if (!windowRes->create(800, 800))
			{
				Logger->error("Couldn't create window");
				return;
			}

			world.addResource(wd::WindowEvents{ *windowRes });
		}

		void Window::Update(ecs::World& world)
		{
			auto windowRes = world.getResource<wd::Window>();
			if (!windowRes)
			{
				Logger->error("Couldn't find a window resource");
				return;
			}

			auto windowEvents = world.getResource<wd::WindowEvents>();
			if (!windowEvents)
			{
				Logger->error("Couldn't find a window events resource");
				return;
			}

			if (windowRes->isOpen())
			{
				windowEvents->pollEvents();
			}
			else
			{
				windowRes->destroyWindow();

				auto exitReason = world.getResource<components::ExitReason>();
				if (exitReason)
				{
					exitReason->exit = true;
					exitReason->reason = "Window closed";
				}
				else
				{
					Logger->error("Couldn't find an exit reason resource");
				}
			}
		}

		void Window::Deinit(ecs::World& world)
		{
			auto windowRes = world.getResource<wd::Window>();
			if (!windowRes)
			{
				Logger->error("Couldn't find a window resource");
				return;
			}

			if (windowRes->isOpen())
				windowRes->destroyWindow();

			wd::GLFW::Deinit();
		}
	}
}