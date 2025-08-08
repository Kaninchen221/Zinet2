#pragma once

#include "Zinet/Gameplay/Nodes/ZtNode2D.hpp"

#include "Zinet/Core/ZtObjectsStorage.hpp"

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

		core::ObjectsStorage objectsStorage;
		ObjectHandle<Node2D> node = objectsStorage.createObject<Node2D>("TestNode");

		static_assert(std::is_default_constructible_v<Node2D>);
	};

	TEST_F(Node2DTests, Test)
	{
		[[maybe_unused]]
		auto& transform = node->transform;
	}
}