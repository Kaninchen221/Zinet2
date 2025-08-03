#pragma once

#include "Zinet/Gameplay/Nodes/ZtNodeCamera.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class NodeCameraTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		ObjectHandle<NodeCamera> node = CreateObject<NodeCamera>("NodeName");
	};

	TEST_F(NodeCameraTests, PassTest)
	{
		[[maybe_unused]] const NodeCamera::CameraT& 
		camera = node->getCamera();
	}
}