#pragma once

#include "Zinet/GameplayLib/ZtSystem.hpp"

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
	};

	TEST_F(SystemTests, VirtualFunctionsTest)
	{
		system.tick(5.f);
	}

	TEST_F(SystemTests, AddNodeTest)
	{
		auto node = std::make_shared<Node>();
		std::weak_ptr<Node> weakNode = node;
		system.addNode(weakNode);

		ASSERT_EQ(node.use_count(), 1);

		auto& nodes = system.getNodes();
		ASSERT_FALSE(nodes.empty());
		ASSERT_EQ(node, nodes.front().lock());
	}
}