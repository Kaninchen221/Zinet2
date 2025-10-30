#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtSchedule.hpp"
#include "Zinet/Core/ECS/ZtTypes.hpp"
#include "Zinet/Core/ECS/ZtQuery.hpp"

#include "Zinet/Core/Tests/ZtTestTypes.hpp"

namespace zt::core::ecs::tests
{
	class ECSScheduleTests : public ::testing::Test
	{
	protected:

		enum Threads : uint8_t
		{
			MainThread,
			RenderThread
		};

	};

	TEST_F(ECSScheduleTests, CreateScheduleTest)
	{
		Schedule schedule = Schedule::Create(Threads::MainThread, Threads::RenderThread);
		const auto& threads = schedule.getThreads();
		ASSERT_EQ(threads.size(), 2);

		ASSERT_EQ(threads[0].getID(), Threads::MainThread);
		ASSERT_EQ(threads[1].getID(), Threads::RenderThread);
	}

	TEST_F(ECSScheduleTests, AddSystemTest)
	{
		Schedule schedule = Schedule::Create(Threads::MainThread, Threads::RenderThread);

		schedule.addSystem(TestSystem::Label{}, TestSystem::entryPoint, Threads::MainThread);

		struct LambdaLabel {};
		auto lambda = []([[maybe_unused]] World& world) -> SystemReturnState { TestSystem::doSomething(); return {}; };

		schedule.addSystem(LambdaLabel{}, lambda, Threads::RenderThread);

		const auto& threads = schedule.getThreads();

		ASSERT_TRUE(threads[Threads::MainThread].getSystems()[0].isEqual(TestSystem::Label{}));
		ASSERT_TRUE(threads[Threads::RenderThread].getSystems()[0].isEqual(LambdaLabel{}));
	}

	TEST_F(ECSScheduleTests, RunTest)
	{
		Schedule schedule = Schedule::Create(Threads::MainThread, Threads::RenderThread);

		schedule.addSystem(TestSystemIncrementar::Label{}, TestSystemIncrementar::entryPoint, Threads::MainThread);

		World world;
		for (size_t i = 0; i < 100; i++)
		{
			world.spawn(Sprite{}, Position{}, Velocity{}, Counter{});
			world.spawn(Sprite{}, Position{}, Counter{});
			world.spawn(Counter{});
		}

		schedule.run(world, Threads::MainThread);
		schedule.requestStop();
		schedule.waitForStop();

		for (const auto [counter] : Query<Counter>(world))
		{
			ASSERT_NE(counter->value, 0);
		}
	}
}