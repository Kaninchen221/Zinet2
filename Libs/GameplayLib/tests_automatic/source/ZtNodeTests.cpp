#pragma once

#include "Zinet/Gameplay/ZtNode.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class NodeTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		Node node;
	};

	TEST_F(NodeTests, ChildrenTest)
	{
		std::shared_ptr addedNode = node.addNode<Node>();
		ASSERT_TRUE(addedNode);

		const Node::Children& children = node.getChildren();
		ASSERT_FALSE(children.empty());

		int i = 0;
		for ([[maybe_unused]] const auto& childNode : node)
		{
			++i;
		}
		ASSERT_EQ(i, children.size());
	}
}