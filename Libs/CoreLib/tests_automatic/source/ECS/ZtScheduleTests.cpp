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

	// TODO: Test there only 'Before' and 'After', add test for components and resources dependencies
	// And then remove testing components and resources from this test
	TEST_F(ECSScheduleTests, BeforeAndAfterDependenciesTest)
	{
		v2::Schedule schedule;

		schedule.addSystem(SystemTest_1{}, SystemTest_1::EntryPoint);
		schedule.addSystem(SystemTest_2{}, SystemTest_2::EntryPoint, v2::Before{ SystemTest_1{} }, v2::After{ SystemTest_3{} });
		schedule.addSystem(SystemTest_3{}, SystemTest_3::EntryPoint);
		schedule.addSystem(EmptySystemTest{}, EmptySystemTest::EntryPoint);

		{
			auto& systems = schedule.getSystems();
			ASSERT_EQ(systems.size(), 4);

			{ // Test system with const query
				auto& systemInfo = systems[0];
				ASSERT_EQ(systemInfo.label, GetTypeID<SystemTest_1>());
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
			}

			{ // Test empty system
				auto& systemInfo = systems[2];
				EXPECT_TRUE(systemInfo.queries.empty());
				EXPECT_TRUE(systemInfo.resources.empty());
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

					EXPECT_EQ(nodes[3].typeID, GetTypeID<EmptySystemTest>());
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

				// If the graph is successfully resolved then it doesn't contain any nodes and edges
				ASSERT_TRUE(nodes.empty());
				ASSERT_TRUE(edges.empty());
			}
			// TODO: Write a shorthand for buildGraph and resolveGraph?

			{ // Test layers -> We need layers to run systems parallel
				const v2::Graph& graph = schedule.getGraph();
				const std::vector<v2::GraphLayer>& layers = graph.layers;
				ASSERT_EQ(layers.size(), 3);

				ASSERT_EQ(layers[0].nodes.size(), 2);
				// Don't test sequence in a layer
// 				EXPECT_EQ(layers[0].nodes[0].typeID, GetTypeID<SystemTest_3>());
//  			EXPECT_EQ(layers[0].nodes[1].typeID, GetTypeID<SystemTest_4>());

				ASSERT_EQ(layers[1].nodes.size(), 1);
				EXPECT_EQ(layers[1].nodes[0].typeID, GetTypeID<SystemTest_2>());

				ASSERT_EQ(layers[2].nodes.size(), 1);
				EXPECT_EQ(layers[2].nodes[0].typeID, GetTypeID<SystemTest_1>());
			}
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

	TEST_F(ECSScheduleTests, GraphMainThreadDependencyTest)
	{
		v2::Schedule schedule;

		schedule.addSystem(SystemTest_2{}, EmptySystemTest::EntryPoint, v2::MainThread{});
		schedule.addSystem(SystemTest_3{}, EmptySystemTest::EntryPoint, v2::MainThread{});
		schedule.addSystem(SystemTest_1{}, EmptySystemTest::EntryPoint);

		// If you have more than one system in the same layer and both of them must be run on the main thread then just run them in sync and don't generate additional edges

		{
			auto& systems = schedule.getSystems();
			ASSERT_EQ(systems.size(), 3);

			{
				auto& systemInfo = systems[0];
				EXPECT_EQ(systemInfo.label, GetTypeID<SystemTest_2>());
				EXPECT_TRUE(systemInfo.systemAdapter);
				EXPECT_EQ(systemInfo.mainThread, true);
			}

			{
				auto& systemInfo = systems[1];
				EXPECT_EQ(systemInfo.label, GetTypeID<SystemTest_3>());
				EXPECT_TRUE(systemInfo.systemAdapter);
				EXPECT_EQ(systemInfo.mainThread, true);
			}

			{
				auto& systemInfo = systems[2];
				ASSERT_EQ(systemInfo.label, GetTypeID<SystemTest_1>());
				EXPECT_TRUE(systemInfo.systemAdapter);
				EXPECT_EQ(systemInfo.mainThread, false);
			}
		}

		schedule.buildGraph();
		schedule.resolveGraph();

		auto& graph = schedule.getGraph();
		auto& layers = graph.layers;

		ASSERT_EQ(layers.size(), 1);

		auto& nodes = layers.front().nodes;
		ASSERT_EQ(nodes.size(), 3);
		EXPECT_EQ(nodes[0].typeID, GetTypeID<SystemTest_1>());
		EXPECT_TRUE(nodes[1].typeID == GetTypeID<SystemTest_2>() ||
					nodes[1].typeID == GetTypeID<SystemTest_3>());
	}

	TEST_F(ECSScheduleTests, ResourcesDependenciesTest)
	{
		v2::Schedule schedule;

		schedule.addSystem(SystemTest_3{}, ReadOnlyPositionResSystemTest::EntryPoint);

		schedule.addSystem(SystemTest_1{}, ReadWritePositionResSystemTest::EntryPoint);
		schedule.addSystem(SystemTest_2{}, ReadWritePositionResSystemTest::EntryPoint);

		schedule.buildGraph();
		schedule.resolveGraph();

		auto& graph = schedule.getGraph();

		// Graph must be resolved
		ASSERT_TRUE(graph.edges.empty());

		auto& layers = graph.layers;

		ASSERT_EQ(layers.size(), 3);

		ASSERT_EQ(layers[0].nodes.size(), 1);
		ASSERT_EQ(layers[1].nodes.size(), 1);
		ASSERT_EQ(layers[2].nodes.size(), 1);

		// Systems added first have higher priority
		EXPECT_EQ(layers[0].nodes[0].typeID, GetTypeID<SystemTest_1>());
		EXPECT_EQ(layers[1].nodes[0].typeID, GetTypeID<SystemTest_2>());
		EXPECT_EQ(layers[2].nodes[0].typeID, GetTypeID<SystemTest_3>());
	}

	TEST_F(ECSScheduleTests, QueriesDependenciesTest)
	{
		v2::Schedule schedule;

		schedule.addSystem(SystemTest_3{}, ReadOnlyPositionVelocitySpriteComponentsSystemTest::EntryPoint);

		schedule.addSystem(SystemTest_1{}, ReadWritePositionVelocitySpriteComponentsSystemTest::EntryPoint);
		schedule.addSystem(SystemTest_2{}, ReadWritePositionVelocitySpriteComponentsSystemTest::EntryPoint);

		schedule.buildGraph();
		schedule.resolveGraph();

		auto& graph = schedule.getGraph();

		// Graph must be resolved
		ASSERT_TRUE(graph.edges.empty());

		auto& layers = graph.layers;

		ASSERT_EQ(layers.size(), 3);

		ASSERT_EQ(layers[0].nodes.size(), 1);
		ASSERT_EQ(layers[1].nodes.size(), 1);
		ASSERT_EQ(layers[2].nodes.size(), 1);

		// Systems added first have higher priority
		EXPECT_EQ(layers[0].nodes[0].typeID, GetTypeID<SystemTest_1>());
		EXPECT_EQ(layers[1].nodes[0].typeID, GetTypeID<SystemTest_2>());
		EXPECT_EQ(layers[2].nodes[0].typeID, GetTypeID<SystemTest_3>());
	}

	// TODO: Create GraphEdges from components dependencies

	// TODO: Test a situation when we have a lot of systems that can be run at the same time
	// The number of systems must exceeds the number of threads pool size
	// The systems can't be in one layer but must be distributed along all layers
	// It's not a problem for Windows but it's still an invalid situation

}