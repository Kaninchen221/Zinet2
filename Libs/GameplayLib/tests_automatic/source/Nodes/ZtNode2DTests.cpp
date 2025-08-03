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

		ObjectHandle<Node2D> node = CreateObject<Node2D>("NodeName");

		static_assert(std::is_default_constructible_v<Node2D>);
	};

	TEST_F(Node2DTests, Test)
	{
		[[maybe_unused]]
		auto& transform = node->transform;
	}
}