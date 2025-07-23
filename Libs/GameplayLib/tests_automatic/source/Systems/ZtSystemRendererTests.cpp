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
			engineContext.addSystem<SystemRenderer>();

			ASSERT_TRUE(engineContext.init());
		}

		void TearDown() override
		{
			engineContext.deinit();
		}

		EngineContext engineContext;
	};

	TEST_F(SystemRendererTests, PassTest)
	{
		SystemRenderer& system = engineContext.getSystem<SystemRenderer>();
		auto node = CreateNode<Node2D>();
		//systemRenderer.addNode(node);

		system.update();
	}
}