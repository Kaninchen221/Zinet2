#pragma once

#include "Zinet/Gameplay/Nodes/ZtNode2D.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"
#include <Zinet/Gameplay/Systems/ZtSystemRenderer.hpp>

#include "Zinet/Core/ZtObjectsStorage.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class Node2DTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			engineContext.addSystem<SystemRenderer>("SystemRenderer");
			engineContext.init();
		}

		void TearDown() override
		{
			node.destroy();
			engineContext.deinit();
		}

		EngineContext engineContext;
		ObjectHandle<Node2D> node;

		static_assert(std::is_default_constructible_v<Node2D>);
	};

	TEST_F(Node2DTests, Test)
	{
		node = CreateObject<Node2D>("TestNode");
		ASSERT_TRUE(node);


	}
}