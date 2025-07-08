#pragma once

#include "Zinet/Gameplay/ZtSystem.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class SystemTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		using NodeBase = Node;
		System<Node> system;
	};

	TEST_F(SystemTests, NodesTest)
	{
		auto node = Node::CreateNode();
		system.addNode(node);
		ASSERT_EQ(node.use_count(), 1);

		ASSERT_EQ(system.getNodes().size(), 1);
	}
}