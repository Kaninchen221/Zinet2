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

	TEST_F(ECSScheduleTests, GraphTest)
	{
		v2::Schedule schedule;

		schedule.addSystem(SystemTest_1{}, SystemTest_1::EntryPoint);
		schedule.addSystem(SystemTest_2{}, SystemTest_2::EntryPoint, v2::Before{ SystemTest_1{} }, v2::After{ SystemTest_3{} });
		schedule.addSystem(SystemTest_3{}, SystemTest_3::EntryPoint);

		auto& systems = schedule.getSystems();
		ASSERT_EQ(systems.size(), 3);

		{ // Test how SystemInfo is filled with data
			auto& systemInfo = systems[1];
			EXPECT_EQ(systemInfo.label, GetTypeID<SystemTest_2>());
			EXPECT_TRUE(systemInfo.system);
			ASSERT_EQ(systemInfo.before.size(), 1);
			EXPECT_EQ(systemInfo.before.front(), GetTypeID<SystemTest_1>());
			ASSERT_EQ(systemInfo.after.size(), 1);
			EXPECT_EQ(systemInfo.after.front(), GetTypeID<SystemTest_3>());

			// TODO: We need to know if the system want to edit or just read the components and reosurces

			{ // Queries
				// Test if queries have correct cunt of types
				ASSERT_EQ(systemInfo.queries.size(), 2);
				EXPECT_EQ(systemInfo.queries[0].types.size(), 2);

				EXPECT_EQ(systemInfo.queries[1].types.size(), 3);

				// Test the obtained types IDs (One query is enough)
				EXPECT_EQ(systemInfo.queries[0].types[0], GetTypeID<Position>());
				EXPECT_EQ(systemInfo.queries[0].types[1], GetTypeID<Sprite>());
			}

			{ // Resources
				// Test if resources have correct cunt of types
				ASSERT_EQ(systemInfo.resources.size(), 3);
				EXPECT_EQ(systemInfo.resources[0].type, GetTypeID<Resource<int>>());
				EXPECT_EQ(systemInfo.resources[1].type, GetTypeID<Resource<float>>());
				EXPECT_EQ(systemInfo.resources[2].type, GetTypeID<Resource<double>>());
			}
		}

		auto buildResult = schedule.buildGraph();
		ASSERT_TRUE(buildResult.getLevel() == SystemReturnState::Level::Info);

		const v2::Graph& graph = schedule.getGraph();

		[[maybe_unused]]
		const std::vector<v2::Thread> threads = graph.getThreads();
		//ASSERT_EQ(threads.size(), 1);
	}
}