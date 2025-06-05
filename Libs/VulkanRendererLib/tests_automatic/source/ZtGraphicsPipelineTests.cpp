#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"

#include <gtest/gtest.h>

namespace zt::vulkan_renderer::tests
{
	class GraphicsPipelineTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		static_assert(std::is_default_constructible_v<GraphicsPipeline>);
		static_assert(!std::is_copy_constructible_v<GraphicsPipeline>);
		static_assert(!std::is_copy_assignable_v<GraphicsPipeline>);
		static_assert(std::is_move_constructible_v<GraphicsPipeline>);
		static_assert(std::is_move_assignable_v<GraphicsPipeline>);
		static_assert(std::is_destructible_v<GraphicsPipeline>);
	};

	TEST_F(GraphicsPipelineTests, PassTest)
	{}
}