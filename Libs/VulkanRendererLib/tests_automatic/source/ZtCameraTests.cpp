#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtCamera.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class CameraTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		Camera camera;

		static_assert(std::is_default_constructible_v<Camera>);
		static_assert(std::is_copy_constructible_v<Camera>);
		static_assert(std::is_copy_assignable_v<Camera>);
		static_assert(std::is_move_constructible_v<Camera>);
		static_assert(std::is_move_assignable_v<Camera>);
		static_assert(std::is_destructible_v<Camera>);
	};

	TEST_F(CameraTests, GetViewMatrixTest)
	{
		const Camera::MatrixT matrix = camera.getViewMatrix();
	}

	TEST_F(CameraTests, SetGetPositionTest)
	{
		const Vector3f position{ 2.f, 1.f, 3.f };
		camera.setPosition(position);
		ASSERT_EQ(camera.getPosition(), position);
	}

	TEST_F(CameraTests, SetGetLookingAtTest)
	{
		const Vector3f lookingAt{ 2.f, 1.f, 3.f };
		camera.setLookingAt(lookingAt);
		ASSERT_EQ(camera.getLookingAt(), lookingAt);
	}

	TEST_F(CameraTests, SetGetUpVectorTest)
	{
		const Vector3f upVector = { 2.f, 2.f, 2.f };
		camera.setUpVector(upVector);
		ASSERT_EQ(camera.getUpVector(), upVector);
	}

	TEST_F(CameraTests, GetPerspectiveMatrix)
	{
		const Camera::MatrixT matrix = camera.getPerspectiveMatrix();
	}

	TEST_F(CameraTests, SetGetFieldOfViewTest)
	{
		const float fov = 45.f;
		camera.setFieldOfView(fov);
		ASSERT_EQ(camera.getFieldOfView(), fov);
	}

	TEST_F(CameraTests, SetGetAspectRatioTest)
	{
		const float aspectRation = 800 / 400;
		camera.setAspectRatio(aspectRation);
		ASSERT_EQ(camera.getAspectRatio(), aspectRation);
	}

	TEST_F(CameraTests, SetGetClippingTest)
	{
		const Vector2f clipping = { 0.01f, 100.0f };
		camera.setClipping(clipping);
		ASSERT_EQ(camera.getClipping(), clipping);
	}
}