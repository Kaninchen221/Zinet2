#pragma once

#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtShadersCompiler.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Core/ZtPaths.hpp"

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

		GraphicsPipeline graphicsPipeline{ nullptr };

		static_assert(std::is_base_of_v<VulkanObject<VkPipeline>, GraphicsPipeline>);

		static_assert(std::is_constructible_v<GraphicsPipeline, VkPipeline>);
		static_assert(!std::is_default_constructible_v<GraphicsPipeline>);
		static_assert(!std::is_copy_constructible_v<GraphicsPipeline>);
		static_assert(!std::is_copy_assignable_v<GraphicsPipeline>);
		static_assert(std::is_move_constructible_v<GraphicsPipeline>);
		static_assert(std::is_move_assignable_v<GraphicsPipeline>);
		static_assert(std::is_destructible_v<GraphicsPipeline>);
	};

	TEST_F(GraphicsPipelineTests, PassTest)
	{}

	TEST(GraphicsPipeline, CreateVkPipelineDynamicStateCreateInfoTest)
	{
		const std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		GraphicsPipeline graphicsPipeline{ nullptr };
		const VkPipelineDynamicStateCreateInfo createInfo =
			graphicsPipeline.createVkPipelineDynamicStateCreateInfo(dynamicStates);

		EXPECT_EQ(createInfo.pDynamicStates, dynamicStates.data());
		EXPECT_EQ(createInfo.dynamicStateCount, dynamicStates.size());
	}

	TEST(GraphicsPipeline, CreateVkPipelineVertexInputStateCreateInfoTest)
	{
		GraphicsPipeline graphicsPipeline{ nullptr };
		const VkPipelineVertexInputStateCreateInfo createInfo =
			graphicsPipeline.createVkPipelineVertexInputStateCreateInfo();
	}

	TEST(GraphicsPipeline, CreateVkPipelineInputAssemblyStateCreateInfoTest)
	{
		GraphicsPipeline graphicsPipeline{ nullptr };
		const VkPipelineInputAssemblyStateCreateInfo createInfo =
			graphicsPipeline.createVkPipelineInputAssemblyStateCreateInfo();
	}

	TEST(GraphicsPipeline, CreateVkPipelineViewportStateCreateInfoTest)
	{
		GraphicsPipeline graphicsPipeline{ nullptr };

		VkViewport viewport{};
		VkRect2D scissor{};

		const VkPipelineViewportStateCreateInfo createInfo =
			graphicsPipeline.createVkPipelineViewportStateCreateInfo(viewport, scissor);

		EXPECT_EQ(createInfo.pViewports, &viewport);
		EXPECT_EQ(createInfo.viewportCount, 1);
		EXPECT_EQ(createInfo.pScissors, &scissor);
		EXPECT_EQ(createInfo.scissorCount, 1);
	}

	TEST(GraphicsPipeline, CreateVkPipelineRasterizationStateCreateInfoTest)
	{
		GraphicsPipeline graphicsPipeline{ nullptr };
		const VkPipelineRasterizationStateCreateInfo createInfo =
			graphicsPipeline.createVkPipelineRasterizationStateCreateInfo();
	}

	TEST(GraphicsPipeline, CreateVkPipelineMultisampleStateCreateInfoTest)
	{
		GraphicsPipeline graphicsPipeline{ nullptr };
		const VkPipelineMultisampleStateCreateInfo createInfo =
			graphicsPipeline.createVkPipelineMultisampleStateCreateInfo();
	}

	TEST(GraphicsPipeline, CreateVkPipelineColorBlendAttachmentStateTest)
	{
		GraphicsPipeline graphicsPipeline{ nullptr };
		const VkPipelineColorBlendAttachmentState attachmentState =
			graphicsPipeline.createVkPipelineColorBlendAttachmentState();
	}

	TEST(GraphicsPipeline, CreateVkPipelineColorBlendStateCreateInfoTest)
	{
		GraphicsPipeline graphicsPipeline{ nullptr };

		const VkPipelineColorBlendAttachmentState attachmentState =
			graphicsPipeline.createVkPipelineColorBlendAttachmentState();

		const VkPipelineColorBlendStateCreateInfo createInfo =
			graphicsPipeline.createVkPipelineColorBlendStateCreateInfo(attachmentState);
	}
}