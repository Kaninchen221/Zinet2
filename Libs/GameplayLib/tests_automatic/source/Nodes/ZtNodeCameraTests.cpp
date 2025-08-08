#pragma once

#include "Zinet/Gameplay/Nodes/ZtNodeCamera.hpp"

#include "Zinet/Core/ZtObjectsStorage.hpp"

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

		core::ObjectsStorage objectsStorage;
		ObjectHandle<NodeCamera> node = objectsStorage.createObject<NodeCamera>("TestNode");
	};

	TEST_F(NodeCameraTests, PassTest)
	{
		[[maybe_unused]] const NodeCamera::CameraT& 
		camera = node->getCamera();
	}
}