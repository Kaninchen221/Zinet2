#pragma once

#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class SystemRendererTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			ASSERT_TRUE(systemRenderer.init());
		}

		void TearDown() override
		{
			ASSERT_TRUE(systemRenderer.deinit());
		}

		SystemRenderer systemRenderer;
	};

	TEST_F(SystemRendererTests, PassTest)
	{
		auto node = CreateNode<Node2D>();
		//systemRenderer.addNode(node);

		systemRenderer.update();
	}
}