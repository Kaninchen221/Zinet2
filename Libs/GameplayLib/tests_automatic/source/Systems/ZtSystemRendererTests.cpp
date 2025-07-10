#pragma once

#include "Zinet/Gameplay/ZtEngineContext.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class SystemRendererTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			ASSERT_TRUE(engineContext.init());
		}

		void TearDown() override
		{
			engineContext.deinit();
		}

		EngineContext engineContext;
		SystemRenderer& systemRenderer = engineContext.systemRenderer;
	};

	TEST_F(SystemRendererTests, PassTest)
	{
		auto node = CreateNode<Node2D>();
		//systemRenderer.addNode(node);

		systemRenderer.update();
	}
}