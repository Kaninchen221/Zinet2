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

		NodeHandle<Node> node = Node::CreateNode<Node>();
	};

	TEST_F(NodeTests, PassTest)
	{}

	TEST_F(NodeTests, ChildrenTest)
	{
		NodeHandle<Node> addedNode = Node::CreateNode();
		ASSERT_EQ(addedNode.use_count(), 1);

		node->addNode(addedNode);
		ASSERT_TRUE(addedNode);
		ASSERT_EQ(addedNode.use_count(), 2);

		const Node::Children& children = node->getChildren();
		ASSERT_FALSE(children.empty());

		int i = 0;
		for ([[maybe_unused]] const auto& childNode : *node)
		{
			++i;
		}
		ASSERT_EQ(i, children.size());
	}
}