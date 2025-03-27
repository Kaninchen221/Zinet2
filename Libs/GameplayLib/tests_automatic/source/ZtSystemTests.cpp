#pragma once

#include "Zinet/GameplayLib/Systems/ZtSystem.hpp"
#include "Zinet/GameplayLib/Systems/ZtTickableSystem.hpp"
#include "Zinet/GameplayLib/Systems/ZtDrawableSystem.hpp"
#include "Zinet/GameplayLib/Systems/ZtWindowEventsSystem.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay_lib::tests
{

	class SystemTests : public ::testing::Test
	{
	protected:

		SystemTests()
		{
		}

		~SystemTests() override {
		}

		void SetUp() override 
		{
		}

		void TearDown() override {
		}

		System system;

	public:

		static void TestSystemAddSomeNodeFunc(System& system, auto addNodeFunc, auto getNodesFunc)
		{
			auto node = std::make_shared<Node>();
			std::weak_ptr<Node> weakNode = node;
			std::invoke(addNodeFunc, weakNode);

			ASSERT_EQ(node.use_count(), 1);

			auto& nodes = std::invoke(getNodesFunc);
			ASSERT_FALSE(nodes.empty());
			ASSERT_EQ(node, nodes.front().lock());
		}
	};

	TEST_F(SystemTests, VirtualFunctionsTest)
	{
		system.tick(5.f);
	}

	TEST(TickableSystemTest, AddNodeTest)
	{
		TickableSystem tickableSystem;
		auto addNodeFunc = std::bind(&TickableSystem::addNode, &tickableSystem, std::placeholders::_1);
		auto getNodesFunc = std::bind(&TickableSystem::getNodes, &tickableSystem);

		SystemTests::TestSystemAddSomeNodeFunc(tickableSystem, addNodeFunc, getNodesFunc);
	}

	TEST(DrawableSystemTests, AddNodeTest)
	{
		DrawableSystem drawableSystem;
		auto addNodeFunc = std::bind(&DrawableSystem::addNode, &drawableSystem, std::placeholders::_1);
		auto getNodesFunc = std::bind(&DrawableSystem::getNodes, &drawableSystem);

		SystemTests::TestSystemAddSomeNodeFunc(drawableSystem, addNodeFunc, getNodesFunc);
	}

	TEST(WindowEventsSystemTests, AddDragableNodeTest)
	{
		WindowEventsSystem windowEventsSystem;
		auto addNodeFunc = std::bind(&WindowEventsSystem::addDragableNode, &windowEventsSystem, std::placeholders::_1);
		auto getNodesFunc = std::bind(&WindowEventsSystem::getDragableNodes, &windowEventsSystem);

		SystemTests::TestSystemAddSomeNodeFunc(windowEventsSystem, addNodeFunc, getNodesFunc);
	}

	TEST(WindowEventsSystemTests, AddClickableNodeTest)
	{
		WindowEventsSystem windowEventsSystem;

		auto clickCallback = [](const std::weak_ptr<Node>& clickedNode) -> void {};
		auto addNodeFunc = std::bind(&WindowEventsSystem::addClickableNode, &windowEventsSystem, std::placeholders::_1, clickCallback);
		auto getNodesFunc = std::bind(&WindowEventsSystem::getClickableNodes, &windowEventsSystem);

		SystemTests::TestSystemAddSomeNodeFunc(windowEventsSystem, addNodeFunc, getNodesFunc);

		const auto& callbacks = windowEventsSystem.getClickableNodesCallbacks();
		ASSERT_EQ(callbacks.size(), 1);
	}
}