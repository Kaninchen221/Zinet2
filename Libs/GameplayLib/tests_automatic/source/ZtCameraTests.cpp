#pragma once

#include "Zinet/GameplayLib/ZtCamera.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay_lib::tests
{

	class CameraTests : public ::testing::Test
	{
	protected:

		CameraTests()
		{
		}

		~CameraTests() override {
		}

		void SetUp() override 
		{
		}

		void TearDown() override {
		}

		Camera camera;
	};

	TEST_F(CameraTests, LookAtTest)
	{
		Vector2f lookAtExpected = { 200, 300 };
		camera.setLookAt(lookAtExpected);

		const Vector2f& lookAtActual = camera.getLookAt();
		EXPECT_EQ(lookAtExpected, lookAtActual);

		auto& position = camera.getPosition();
		EXPECT_EQ(lookAtExpected, position);
	}

	TEST_F(CameraTests, MousePositionToWorldTest)
	{
		camera.create({ 512, 512 });
		camera.setLookAt({ 100.f, 200.f });
		const auto mousePositionNorm = Vector2f{ .5f, .5f };

		const Vector2f mousePositionWorld = camera.mousePositionNormToWorld(mousePositionNorm);
		const Vector2f expected = { 100.f, 200.f };
		ASSERT_EQ(mousePositionWorld, expected);
	}
}