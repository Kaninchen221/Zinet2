#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtSprite.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class SpriteTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		Sprite sprite;
		RendererContext rendererContext;

		static_assert(std::is_default_constructible_v<Sprite>);
		static_assert(!std::is_copy_constructible_v<Sprite>);
		static_assert(!std::is_copy_assignable_v<Sprite>);
		static_assert(std::is_move_constructible_v<Sprite>);
		static_assert(std::is_move_assignable_v<Sprite>);
		static_assert(std::is_destructible_v<Sprite>);
	};

	TEST_F(SpriteTests, Test)
	{
	}
}