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

	TEST_F(ECSScheduleTests, GraphOneThreadTest)
	{
		v2::Schedule schedule;

		schedule.addSystem(SystemTest_1{}, SystemTest_1::EntryPoint);
		schedule.addSystem(SystemTest_2{}, SystemTest_2::EntryPoint, v2::Before{ SystemTest_1{} }, v2::After{ SystemTest_3{} });
		schedule.addSystem(SystemTest_3{}, SystemTest_3::EntryPoint);
		schedule.addSystem(SystemTest_4{}, SystemTest_4::EntryPoint);

		{
			auto& systems = schedule.getSystems();
			ASSERT_EQ(systems.size(), 4);

			{ // Test system with const query
				auto& systemInfo = systems[0];
				EXPECT_EQ(systemInfo.label, GetTypeID<SystemTest_1>());
				EXPECT_TRUE(systemInfo.systemAdapter);
				ASSERT_EQ(systemInfo.before.size(), 0);
				ASSERT_EQ(systemInfo.after.size(), 0);

				{ // Queries
					ASSERT_EQ(systemInfo.queries.size(), 1);
					EXPECT_TRUE(systemInfo.queries[0].isConst);
				}
			}

			{ // Test how SystemInfo is filled with data
				auto& systemInfo = systems[1];
				EXPECT_EQ(systemInfo.label, GetTypeID<SystemTest_2>());
				EXPECT_TRUE(systemInfo.systemAdapter);
				ASSERT_EQ(systemInfo.before.size(), 1);
				EXPECT_EQ(systemInfo.before.front(), GetTypeID<SystemTest_1>());
				ASSERT_EQ(systemInfo.after.size(), 1);
				EXPECT_EQ(systemInfo.after.front(), GetTypeID<SystemTest_3>());

				{ // Queries
					// Test if queries have correct cunt of types
					ASSERT_EQ(systemInfo.queries.size(), 2);
					EXPECT_EQ(systemInfo.queries[0].types.size(), 2);
					EXPECT_FALSE(systemInfo.queries[0].isConst);

					EXPECT_EQ(systemInfo.queries[1].types.size(), 3);
					EXPECT_FALSE(systemInfo.queries[1].isConst);

					// Test the obtained types IDs (One query is enough)
					EXPECT_EQ(systemInfo.queries[0].types[0], GetTypeID<Position>());
					EXPECT_EQ(systemInfo.queries[0].types[1], GetTypeID<Sprite>());
				}

				{ // Resources
					// Test if resources have correct cunt of types
					ASSERT_EQ(systemInfo.resources.size(), 3);
					EXPECT_EQ(systemInfo.resources[0].type, GetTypeID<Resource<int>>());
					EXPECT_EQ(systemInfo.resources[0].isConst, false);
					EXPECT_EQ(systemInfo.resources[1].type, GetTypeID<Resource<float>>());
					EXPECT_EQ(systemInfo.resources[1].isConst, false);
					EXPECT_EQ(systemInfo.resources[2].type, GetTypeID<Resource<double>>());
					EXPECT_EQ(systemInfo.resources[2].isConst, false);
				}
			}

			{ // Test empty system
				auto& systemInfo = systems[2];
				EXPECT_TRUE(systemInfo.queries.empty());
				EXPECT_TRUE(systemInfo.resources.empty());
			}

			{ // Test system with const resource
				auto& systemInfo = systems[3];
				ASSERT_EQ(systemInfo.resources.size(), 1);

				EXPECT_EQ(systemInfo.resources[0].type, GetTypeID<ConstResource<int>>());
				EXPECT_TRUE(systemInfo.resources[0].isConst);
			}

			{ // Test build graph nodes that will be used to create the final graph
				schedule.buildGraph();
				const v2::Graph& graph = schedule.getGraph();

				{ // Nodes
					auto& nodes = graph.nodes;
					ASSERT_EQ(nodes.size(), systems.size());

					EXPECT_EQ(nodes[0].typeID, GetTypeID<SystemTest_1>());
					ASSERT_EQ(nodes[0].after.size(), 1);
					EXPECT_EQ(nodes[0].after[0], GetTypeID<SystemTest_2>());
					EXPECT_EQ(nodes[0].before.size(), 0);

					EXPECT_EQ(nodes[1].typeID, GetTypeID<SystemTest_2>());
					ASSERT_EQ(nodes[1].after.size(), 1);
					EXPECT_EQ(nodes[1].after[0], GetTypeID<SystemTest_3>());
					EXPECT_EQ(nodes[1].before.size(), 1);
					EXPECT_EQ(nodes[1].before[0], GetTypeID<SystemTest_1>());

					EXPECT_EQ(nodes[2].typeID, GetTypeID<SystemTest_3>());
					EXPECT_EQ(nodes[2].after.size(), 0);
					EXPECT_EQ(nodes[2].before.size(), 1);
					EXPECT_EQ(nodes[2].before[0], GetTypeID<SystemTest_2>());

					EXPECT_EQ(nodes[3].typeID, GetTypeID<SystemTest_4>());
					EXPECT_EQ(nodes[3].after.size(), 0);
					EXPECT_EQ(nodes[3].before.size(), 0);
				}

				{ // Edges
					auto& edges = graph.edges;
					ASSERT_EQ(edges.size(), 2);

					EXPECT_TRUE(std::ranges::contains(edges, v2::GraphEdge{ .from = GetTypeID<SystemTest_2>(), .to = GetTypeID<SystemTest_1>() }));
					EXPECT_TRUE(std::ranges::contains(edges, v2::GraphEdge{ .from = GetTypeID<SystemTest_3>(), .to = GetTypeID<SystemTest_2>() }));
				}
			}

			// Test build a graph from the nodes
			{
				schedule.resolveGraph();

				const v2::Graph& graph = schedule.getGraph();
				auto& nodes = graph.nodes;
				auto& edges = graph.edges;
				ASSERT_EQ(nodes.size(), 4);
				ASSERT_TRUE(edges.empty());

				if (nodes.front().typeID == GetTypeID<SystemTest_4>())
				{
					EXPECT_EQ(nodes[1].typeID, GetTypeID<SystemTest_3>());
					EXPECT_EQ(nodes[2].typeID, GetTypeID<SystemTest_2>());
					EXPECT_EQ(nodes[3].typeID, GetTypeID<SystemTest_1>());
				}
				else if (nodes.back().typeID == GetTypeID<SystemTest_4>())
				{
					EXPECT_EQ(nodes[0].typeID, GetTypeID<SystemTest_3>());
					EXPECT_EQ(nodes[1].typeID, GetTypeID<SystemTest_2>());
					EXPECT_EQ(nodes[2].typeID, GetTypeID<SystemTest_1>());
				}
				else
				{
					FAIL() << "SystemTest_4 must be first or last";
				}
			}
			// TODO: Write a shorthand for buildGraph and resolveGraph?
		}

		World world;
		world.spawn(Position{ 0, 0 }, Sprite{ 0 }, Velocity{ 0, 0 });
		world.spawn(Position{ 0, 0 }, Sprite{ 0 }, Velocity{ 0, 0 });

		schedule.runOnce(world);

		auto query = Query<Position>(world);

		ASSERT_NE(query.getComponentsCount(), 0);
		for (auto [position] : query)
		{
			ASSERT_EQ(position->x, 2);
			ASSERT_EQ(position->y, 2);
		}
	}
}