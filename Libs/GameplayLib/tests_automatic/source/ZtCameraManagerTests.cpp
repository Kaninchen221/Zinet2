#pragma once

#include <gtest/gtest.h>

#include "Zinet/Gameplay/ZtCameraManager.hpp"

namespace zt::gameplay::system::tests
{
	class CameraManagerTests : public ::testing::Test
	{
	protected:

		CameraManager cameraManager;

		void setupCamera(Camera& camera)
		{
			camera.setPosition(position);
			camera.setFieldOfView(fov);
		}

		const Vector3f position{ 200, 200, 200 };
		const Vector3f lookingAt{ 50, 50, 50 };
		const float fov = 90;
	};

	// Some starting camera
	// BlendToCamera (DestinationCamera, BlendTime)
	// 
	// Update (DeltaTime)

	TEST_F(CameraManagerTests, SetGetTest)
	{
		Camera expectedCamera;
		setupCamera(expectedCamera);

		cameraManager.setCamera(expectedCamera);

		const auto& actualCamera = cameraManager.getCamera();
		ASSERT_EQ(expectedCamera, actualCamera);
	}

	TEST_F(CameraManagerTests, BlendTo)
	{
		Camera startCamera;
		setupCamera(startCamera);

		cameraManager.setCamera(startCamera);

		const Vector3f targetPosition = position + Vector3f{ 100, 100, 100 };
		const Vector3f targetLookingAt = lookingAt + Vector3f{ 100, 100, 100 };
		const float targetFOV = 45.f;
		// Ignore: upVector, aspectRatio and clipping

		Camera target;
		target.setPosition(targetPosition);
		target.setLookingAt(lookingAt);
		target.setFieldOfView(targetFOV);

		const core::Time time = core::Time::FromSeconds(20);

		cameraManager.blendTo(target, time);
		ASSERT_TRUE(cameraManager.isBlending());

		cameraManager.update(core::Time::FromSeconds(10));

		ASSERT_NE(cameraManager.getCamera(), startCamera);
		ASSERT_NE(cameraManager.getCamera(), target);

		cameraManager.update(core::Time::FromSeconds(10));

		ASSERT_NE(cameraManager.getCamera(), startCamera);
		ASSERT_EQ(cameraManager.getCamera(), target);
		ASSERT_FALSE(cameraManager.isBlending());
	}
}