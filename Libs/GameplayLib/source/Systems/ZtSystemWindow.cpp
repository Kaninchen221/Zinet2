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

			auto window = world.getResource<wd::Window>();
			if (!window)
			{
				Logger->error("Couldn't find a window resource");
				return;
			}

			if (!window->create(800, 800))
			{
				Logger->error("Couldn't create window");
				return;
			}
		}

		void Window::Update(ecs::World& world)
		{
			auto window = world.getResource<wd::Window>();
			if (!window)
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

			if (window->isOpen())
			{
				windowEvents->pollEvents();
			}
			else
			{
				window->destroyWindow();

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
			auto window = world.getResource<wd::Window>();
			if (!window)
			{
				Logger->error("Couldn't find a window resource");
				return;
			}

			if (window->isOpen())
				window->destroyWindow();

			wd::GLFW::Deinit();
		}
	}
}