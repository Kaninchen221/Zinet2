#pragma once

#include "Zinet/Gameplay/Nodes/ZtNode2D.hpp"

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

		NodeHandle<Node2D> node = CreateNode<Node2D>("NodeName");

		static_assert(!std::is_default_constructible_v<Node2D>);
		static_assert(std::is_constructible_v<Node2D, const NodeNameView&>);
	};

	TEST_F(Node2DTests, PassTest)
	{
		ASSERT_EQ(node->getName(), "NodeName");
	}

	TEST_F(Node2DTests, Test)
	{
		[[maybe_unused]]
		auto& transform = node->transform;
	}
}