#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"

#include <gtest/gtest.h>

#include "Zinet/Core/ZtPaths.hpp"

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
	};

	TEST_F(VulkanRendererTests, PassTest)
	{

	}
}