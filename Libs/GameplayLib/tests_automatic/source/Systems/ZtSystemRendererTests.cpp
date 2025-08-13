#pragma once

#include "Zinet/Gameplay/ZtEngineContext.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeCamera.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class SystemRendererTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			SystemRenderer::SetUseImGui(false);
			engineContext.addSystem<SystemRenderer>("SystemRenderer");

			ASSERT_TRUE(engineContext.init());
		}

		void TearDown() override
		{
			engineContext.deinit();
			SystemRenderer::SetUseImGui(true);
		}

		EngineContext engineContext;
	};

	TEST_F(SystemRendererTests, PassTest)
	{
		auto system = engineContext.getSystem<SystemRenderer>();
		ASSERT_TRUE(system);
		auto node = CreateObject<Node2D>("node2d");
		//systemRenderer.addNode(node);

		auto cameraNode = CreateObject<NodeCamera>("Camera");
		system->setCameraNode(cameraNode);
		ASSERT_EQ(cameraNode.get(), system->getCameraNode().get());

		system->update();
	}
}