#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtTransform.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class TransformTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		Transform transform;

		static_assert(std::is_default_constructible_v<Transform>);
		static_assert(std::is_copy_constructible_v<Transform>);
		static_assert(std::is_copy_assignable_v<Transform>);
		static_assert(std::is_move_constructible_v<Transform>);
		static_assert(std::is_move_assignable_v<Transform>);
		static_assert(std::is_destructible_v<Transform>);
	};

	TEST_F(TransformTests, GetMatrixTest)
	{
		const glm::mat4 matrix = transform.getMatrix();
	}

	TEST_F(TransformTests, SetGetPositionTest)
	{
		const Vector3f position{ 2.f, 1.f, 3.f };
		transform.setPosition(position);
		ASSERT_EQ(transform.getPosition(), position);
	}

	TEST_F(TransformTests, SetGetRotationTest)
	{
		const float rotation = 45.f;
		transform.setRotation(rotation);
		ASSERT_EQ(transform.getRotation(), rotation);
	}

	TEST_F(TransformTests, SetGetScaleTest)
	{
		const Vector3f scale = { 2.f, 2.f, 2.f };
		transform.setScale(scale);
		ASSERT_EQ(transform.getScale(), scale);
	}
}