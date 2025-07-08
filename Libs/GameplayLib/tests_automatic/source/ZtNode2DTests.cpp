#pragma once

#include "Zinet/Gameplay/ZtNode2D.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class Node2DTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		NodeHandle<Node2D> node = Node::CreateNode<Node2D>();
	};

	TEST_F(Node2DTests, PassTest)
	{}

	TEST_F(Node2DTests, Test)
	{
		auto& transform = node->transform;
	}
}