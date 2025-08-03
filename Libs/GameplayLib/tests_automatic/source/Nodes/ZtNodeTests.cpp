#pragma once

#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

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

		ObjectHandle<Node> node = CreateObject<Node>("Node");
	};

	TEST_F(NodeTests, PassTest)
	{}

	TEST_F(NodeTests, ChildrenTest)
	{
		{
			ObjectHandle<Node> childNode = CreateObject<Node>("ChildNode");
			ASSERT_EQ(childNode.use_count(), 1);

			node->addChild(childNode);
			ASSERT_TRUE(childNode);
			ASSERT_EQ(childNode.use_count(), 2);
		}

		const Node::Children& children = node->getChildren();
		ASSERT_FALSE(children.empty());

		int i = 0;
		for ([[maybe_unused]] const auto& childNode : *node)
		{
			++i;
		}
		ASSERT_EQ(i, children.size());
	}

	TEST_F(NodeTests, ParentTest)
	{
		auto parent = CreateObject<Node>("parent");
		node->setParent(parent);
		ASSERT_EQ(parent.get(), node->getParent().lock().get());
	
		ASSERT_EQ(parent.use_count(), 1);
	}
}