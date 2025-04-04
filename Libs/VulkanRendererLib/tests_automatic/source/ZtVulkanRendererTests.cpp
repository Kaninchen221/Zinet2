#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"

#include "Zinet/Core/ZtPaths.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer::tests
{
	class VulkanRendererTests : public ::testing::Test
	{
	protected:

		VulkanRendererTests()
		{
		}

		~VulkanRendererTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		VulkanRenderer renderer;

		VkInstance Get() { return nullptr; }
	};

	TEST_F(VulkanRendererTests, Test)
	{

	}
}