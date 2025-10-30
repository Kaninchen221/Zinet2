﻿#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"

#include "Zinet/Core/Components/ZtExitReason.hpp"

namespace zt::gameplay
{
	namespace system
	{
		using namespace core;

		core::ecs::SystemReturnState Window::Init(ecs::World& world)
		{
			using Level = ecs::SystemReturnState::Level;

			wd::GLFW::Init(false);

			auto windowRes = world.addResource(wd::Window{});
			if (!windowRes)
			{
				return { Level::Error, "Couldn't add a window resource" };
			}

			if (!windowRes->create(800, 800))
			{
				return { Level::Error, "Couldn't create window" };
			}

			auto windowEventsRes = world.addResource(wd::WindowEvents{ *windowRes });
			if (!windowEventsRes)
			{
				return { Level::Error, "Couldn't add a window events resource" };
			}

			return {};
		}

		core::ecs::SystemReturnState Window::Update(ecs::World& world)
		{
			using Level = ecs::SystemReturnState::Level;

			auto windowRes = world.getResource<wd::Window>();
			if (!windowRes)
			{
				return { Level::Error, "Couldn't find a window resource" };
			}

			auto windowEvents = world.getResource<wd::WindowEvents>();
			if (!windowEvents)
			{
				return { Level::Error, "Couldn't find a window events resource" };
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
					return { Level::Error, "Couldn't find an exit reason resource" };
				}
			}

			return {};
		}

		core::ecs::SystemReturnState Window::Deinit(ecs::World& world)
		{
			using Level = ecs::SystemReturnState::Level;

			auto windowRes = world.getResource<wd::Window>();
			if (!windowRes)
			{
				return { Level::Error, "Couldn't find a window resource" };
			}

			if (windowRes->isOpen())
				windowRes->destroyWindow();

			wd::GLFW::Deinit();

			return {};
		}
	}
}