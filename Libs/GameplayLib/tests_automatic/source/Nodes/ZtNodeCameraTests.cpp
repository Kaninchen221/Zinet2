#pragma once

#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeCamera.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "Zinet/Core/ZtObjectsStorage.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class NodeCameraTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			engineContext.addSystem<SystemWindow>("window", {});
			engineContext.addSystem<SystemRenderer>("renderer", {});

			ASSERT_TRUE(engineContext.init());
			node = CreateObject<NodeCamera>("camera");
		}

		void TearDown() override
		{
			node.destroy();
			engineContext.deinit();
		}

		EngineContext engineContext;
		ObjectHandle<NodeCamera> node;
	};

	TEST_F(NodeCameraTests, PassTest)
	{
		[[maybe_unused]] const NodeCamera::CameraT& 
		camera = node->getCamera();
	}
}