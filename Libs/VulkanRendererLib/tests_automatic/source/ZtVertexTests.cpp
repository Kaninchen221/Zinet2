#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtVertex.hpp"

#include <gtest/gtest.h>

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class VertexTests : public ::testing::Test
	{
	protected:

		VertexTests()
		{
		}

		~VertexTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		Vertex vertex{};

		static_assert(std::is_default_constructible_v<Vertex>);
		static_assert(std::is_copy_constructible_v<Vertex>);
		static_assert(std::is_copy_assignable_v<Vertex>);
		static_assert(std::is_move_constructible_v<Vertex>);
		static_assert(std::is_move_assignable_v<Vertex>);
		static_assert(std::is_destructible_v<Vertex>);
	};

	TEST_F(VertexTests, PassTest)
	{
		const VkVertexInputBindingDescription bindingDescription = Vertex::GetInputBindingDescription();
		const std::array<VkVertexInputAttributeDescription, 3> attributesDescriptions = Vertex::GetInputAttributesDescriptions();
	}
}