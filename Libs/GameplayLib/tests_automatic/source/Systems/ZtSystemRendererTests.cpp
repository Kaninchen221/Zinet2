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
			SystemRenderer::UseImGui = false;
			engineContext.addSystem<SystemRenderer>();

			ASSERT_TRUE(engineContext.init());
		}

		void TearDown() override
		{
			engineContext.deinit();
			SystemRenderer::UseImGui = true;
		}

		EngineContext engineContext;
	};

	TEST_F(SystemRendererTests, PassTest)
	{
		auto system = engineContext.getSystem<SystemRenderer>();
		ASSERT_TRUE(system);
		auto node = CreateNode<Node2D>();
		//systemRenderer.addNode(node);

		system->update();
	}
}