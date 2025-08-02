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

		NodeHandle<NodeCamera> node = CreateNode<NodeCamera>("NodeName");

		static_assert(!std::is_default_constructible_v<NodeCamera>);
		static_assert(std::is_constructible_v<NodeCamera, const std::string_view&>);
	};

	TEST_F(NodeCameraTests, PassTest)
	{
		ASSERT_EQ(node->getName(), "NodeName");

		[[maybe_unused]] const NodeCamera::CameraT& 
		camera = node->getCamera();
	}
}