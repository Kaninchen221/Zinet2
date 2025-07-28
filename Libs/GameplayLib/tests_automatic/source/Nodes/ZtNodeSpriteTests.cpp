#pragma once

#include "Zinet/Gameplay/Nodes/ZtNodeSprite.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class NodeSpriteTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		NodeHandle<NodeSprite> node = CreateNode<NodeSprite>("NodeName");

		static_assert(!std::is_default_constructible_v<NodeSprite>);
		static_assert(std::is_constructible_v<NodeSprite, const std::string_view&>);
	};

	TEST_F(NodeSpriteTests, PassTest)
	{
		ASSERT_EQ(node->getName(), "NodeName");
	}

	TEST_F(NodeSpriteTests, Test)
	{
		[[maybe_unused]]
		auto& transform = node->transform;
	}
}