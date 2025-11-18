#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtSchedule.hpp"
#include "Zinet/Core/ECS/ZtTypes.hpp"
#include "Zinet/Core/ECS/ZtQuery.hpp"

#include "Zinet/Core/Tests/ZtTestTypes.hpp"

#include "Zinet/Core/Components/ZtExitReason.hpp"

namespace zt::core::ecs::tests
{
	class ECSScheduleTests : public ::testing::Test
	{

	};

	TEST_F(ECSScheduleTests, BeforeAndAfterDependenciesTest)
	{
		Schedule schedule;

		schedule.addSystem(SystemTest_1{}, EmptySystemTest::EntryPoint);
		schedule.addSystem(SystemTest_2{}, EmptySystemTest::EntryPoint, Before{ SystemTest_1{} }, After{ SystemTest_3{} });
		schedule.addSystem(SystemTest_3{}, EmptySystemTest::EntryPoint);
		schedule.addSystem(SystemTest_4{}, EmptySystemTest::EntryPoint);

		{
			auto& systems = schedule.getSystems();
			ASSERT_EQ(systems.size(), 4);

			{ // Test build graph nodes that will be used to create the final graph
				schedule.buildGraph();
				const Graph& graph = schedule.getGraph();

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

					EXPECT_TRUE(std::ranges::contains(edges, GraphEdge{ .from = GetTypeID<SystemTest_2>(), .to = GetTypeID<SystemTest_1>() }));
					EXPECT_TRUE(std::ranges::contains(edges, GraphEdge{ .from = GetTypeID<SystemTest_3>(), .to = GetTypeID<SystemTest_2>() }));
				}
			}

			// Test build a graph from the nodes
			{
 				schedule.resolveGraph();
 
				const Graph& graph = schedule.getGraph();
				auto& nodes = graph.nodes;
				auto& edges = graph.edges;

				// If the graph is successfully resolved then it doesn't contain any nodes and edges
				ASSERT_TRUE(nodes.empty());
				ASSERT_TRUE(edges.empty());
			}

			{ // Test layers -> We need layers to run systems parallel
				const Graph& graph = schedule.getGraph();
				const std::vector<GraphLayer>& layers = graph.layers;
				ASSERT_EQ(layers.size(), 3);

				// Don't test sequence in the first layer
				ASSERT_EQ(layers[0].nodes.size(), 2);

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
	}

	TEST_F(ECSScheduleTests, GraphMainThreadDependencyTest)
	{
		Schedule schedule;

		schedule.addSystem(SystemTest_2{}, EmptySystemTest::EntryPoint, MainThread{});
		schedule.addSystem(SystemTest_3{}, EmptySystemTest::EntryPoint, MainThread{});
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
		Schedule schedule;

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
		Schedule schedule;

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

	TEST_F(ECSScheduleTests, AddResourceExpectResourceTest)
	{
		Schedule schedule;

		schedule.addSystem(SystemTest_1{}, AddResourceSystemTest::AddPosition, Before{ SystemTest_2{} });
		schedule.addSystem(SystemTest_2{}, ExpectResourceSystemTest::ExpectPosition);

		schedule.buildGraph();
		schedule.resolveGraph();

		const auto& graph = schedule.getGraph();
		// We expect that both systems are in different layers
		ASSERT_EQ(graph.layers.size(), 2);

		World world;

		schedule.runOnce(world);

		auto exitReasonRes = world.getResource<components::ExitReason>();
		ASSERT_TRUE(exitReasonRes);
		ASSERT_FALSE(exitReasonRes->exit);
	}

	// TODO: Test a situation when we have a lot of systems that can be run at the same time
	// In test: The number of systems must exceeds the number of threads pool size
	// What needs to be done: The systems can't be in one layer but must be distributed along all layers
	// Note: It's not a problem for Windows but it's still an invalid situation

}