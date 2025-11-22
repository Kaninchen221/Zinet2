#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtWorld.hpp"
#include "Zinet/Core/ECS/ZtSchedule.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::gameplay::system::tests
{
	using namespace zt::core;

	class WindowTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}
	};

	TEST_F(WindowTests, Test)
	{
		ecs::World world;
		ecs::Schedule schedule;

		{ // Init
			schedule.addSystem(Window{}, Window::Init);

			schedule.buildGraph();
			schedule.resolveGraph();
			schedule.runOnce(world);

			auto windowRes = world.getResource<wd::Window>();
			ASSERT_TRUE(windowRes);
			ASSERT_TRUE(windowRes->isOpen());

			auto windowEventsRes = world.getResource<wd::WindowEvents>();
			ASSERT_TRUE(windowEventsRes);

			schedule.clear();
		}

		{ // Update
			schedule.addSystem(Window{}, Window::Update);

			schedule.buildGraph();
			schedule.resolveGraph();
			schedule.runOnce(world);

			schedule.clear();
		}

		{ // Deinit
			schedule.addSystem(Window{}, Window::Deinit);

			schedule.buildGraph();
			schedule.resolveGraph();
			schedule.runOnce(world);

			auto windowRes = world.getResource<wd::Window>();
			ASSERT_TRUE(windowRes);
			ASSERT_FALSE(windowRes->getInternal());
		}
	}
}