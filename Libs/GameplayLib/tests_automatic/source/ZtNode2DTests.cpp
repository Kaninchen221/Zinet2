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

		NodeHandle<Node2D> node = CreateNode<Node2D>();
	};

	TEST_F(Node2DTests, PassTest)
	{}

	TEST_F(Node2DTests, Test)
	{
		[[maybe_unused]]
		auto& transform = node->transform;
	}
}