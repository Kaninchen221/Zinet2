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

		core::ObjectsStorage objectsStorage;
		ObjectHandle<NodeSprite> node = objectsStorage.createObject<NodeSprite>("node");
	};

	TEST_F(NodeSpriteTests, Test)
	{
		[[maybe_unused]]
		auto& transform = node->transform;
	}
}