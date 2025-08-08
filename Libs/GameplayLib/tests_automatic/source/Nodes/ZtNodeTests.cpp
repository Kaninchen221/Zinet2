#pragma once

#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtObjectsStorage.hpp"

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

		core::ObjectsStorage objectsStorage;
		ObjectHandle<Node> node = objectsStorage.createObject<Node>("TestNode");
	};

	TEST_F(NodeTests, PassTest)
	{}

	TEST_F(NodeTests, ChildrenTest)
	{
		{
			ObjectHandle<Node> childNode = objectsStorage.createObject<Node>("ChildNode");
			ASSERT_EQ(childNode.getRefCount(), 1);

			node->addChild(childNode);
			ASSERT_TRUE(childNode);
			ASSERT_EQ(childNode.getRefCount(), 2);
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
		auto parent = objectsStorage.createObject<Node>("parent");
		node->setParent(parent.createWeakHandle());
		ASSERT_EQ(parent.get(), node->getParent().get());
	
		ASSERT_EQ(parent.getRefCount(), 1);
	}
}