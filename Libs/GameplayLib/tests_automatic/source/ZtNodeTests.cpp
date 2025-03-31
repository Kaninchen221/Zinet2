#pragma once

#include "Zinet/GameplayLib/ZtNode.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay_lib::tests
{

	class NodeTests : public ::testing::Test
	{
	protected:

		NodeTests()
		{
		}

		~NodeTests() override {
		}

		void SetUp() override 
		{
		}

		void TearDown() override {
		}

		Node node;

		class TestNodeClass : public Node {};
	};

	TEST_F(NodeTests, VirtualFunctionsTest)
	{
		node.tick(1.f);
		sf::DrawInfo drawInfo = node.getDrawInfo();
		ASSERT_TRUE(node.isDrawInfoDirty());
	}

	TEST_F(NodeTests, GetAbsolutePositionTest)
	{
		const Vector2f parentNode_1_position{ 100.f, 100.f };
		const auto parentNode_1 = std::make_shared<TestNodeClass>();
		parentNode_1->setPosition(parentNode_1_position);
		EXPECT_EQ(parentNode_1->getAbsolutePosition(), parentNode_1_position);

		const Vector2f parentNode_2_position{ 200.f, 200.f };
		const auto parentNode_2 = std::make_shared<TestNodeClass>();
		parentNode_2->setPosition(parentNode_2_position);
		parentNode_2->setParentNode(parentNode_1);
		EXPECT_EQ(parentNode_2->getAbsolutePosition(), parentNode_1_position + parentNode_2_position);

		const Vector2f parentNode_3_position{ 300.f, 300.f };
		const auto parentNode_3 = std::make_shared<TestNodeClass>();
		parentNode_3->setPosition(parentNode_3_position);
		parentNode_3->setParentNode(parentNode_2);
		EXPECT_EQ(parentNode_3->getAbsolutePosition(), parentNode_1_position + parentNode_2_position + parentNode_3_position);
	}
}