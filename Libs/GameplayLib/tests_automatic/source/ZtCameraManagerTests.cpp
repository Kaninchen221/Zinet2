#pragma once

#include <gtest/gtest.h>

#include "Zinet/Gameplay/ZtCameraManager.hpp"

namespace zt::gameplay::system::tests
{
	class CameraManagerTests : public ::testing::Test
	{
	protected:

	};

	TEST_F(CameraManagerTests, PassTest)
	{
		// Some starting camera
		// BlendToCamera (DestinationCamera, BlendTime)
		// 
		// Update (DeltaTime)
	}
}