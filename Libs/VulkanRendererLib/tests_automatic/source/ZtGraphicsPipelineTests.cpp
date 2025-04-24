#pragma once

#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtShadersCompiler.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtRenderPass.hpp"
#include "Zinet/VulkanRenderer/ZtPipelineLayout.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Core/ZtPaths.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer::tests
{
	class GraphicsPipelineTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			wd::GLFW::Init();

			window.create(2, 2);

			instance.setEnableValidationLayers(true);
			ASSERT_TRUE(instance.create());

			ASSERT_TRUE(debugUtilsMessenger.create(instance));

			auto physicalDevices = instance.getPhysicalDevices();
			physicalDevice = PhysicalDevice::TakeBestPhysicalDevice(physicalDevices);
			ASSERT_TRUE(physicalDevice.isValid());

			ASSERT_TRUE(surface.create(instance, window));

			ASSERT_TRUE(device.create(physicalDevice, surface));

			ASSERT_TRUE(swapChain.create(device, physicalDevice, surface, window));

			const auto renderPassCreateInfo = RenderPass::GetPresentCreateInfo(VK_FORMAT_B8G8R8A8_SRGB);
			ASSERT_TRUE(renderPass.create(device, renderPassCreateInfo));

			ASSERT_TRUE(pipelineLayout.create(device));

			const auto contentFolderPath = core::Paths::CurrentProjectRootPath() / "test_files";

			const auto vertexShaderFilePath = contentFolderPath / "simple_triangle_shader.vert";
			const auto vertexShaderResult = shadersCompiler.compileFromFile(vertexShaderFilePath, ShaderType::Vertex);
			ASSERT_EQ(vertexShaderResult.GetCompilationStatus(), shaderc_compilation_status_success);

			ASSERT_TRUE(vertexShaderModule.create(device, vertexShaderResult));
			shadersStages.push_back(vertexShaderModule.createPipelineShaderStageCreateInfo(ShaderType::Vertex));

			const auto fragmentShaderFilePath = contentFolderPath / "simple_triangle_shader.frag";
			const auto fragmentShaderResult = shadersCompiler.compileFromFile(fragmentShaderFilePath, ShaderType::Fragment);
			ASSERT_EQ(fragmentShaderResult.GetCompilationStatus(), shaderc_compilation_status_success);

			ASSERT_TRUE(fragmentShaderModule.create(device, fragmentShaderResult));
			shadersStages.push_back(fragmentShaderModule.createPipelineShaderStageCreateInfo(ShaderType::Fragment));

			const auto viewportSize = window.getFramebufferSize();
			viewport = { 0, 0, static_cast<float>(viewportSize.x), static_cast<float>(viewportSize.y), 0.f, 1.f };

			scissor = { { 0, 0 }, { static_cast<std::uint32_t>(viewportSize.x), static_cast<std::uint32_t>(viewportSize.y) } };

			ASSERT_TRUE(
				graphicsPipeline.create(device, pipelineLayout, renderPass, viewport, scissor, shadersStages)
			);

			ASSERT_TRUE(graphicsPipeline.isValid());
		}

		void TearDown() override
		{
			graphicsPipeline.destroy(device);
			ASSERT_FALSE(graphicsPipeline.isValid());

			vertexShaderModule.destroy(device);
			fragmentShaderModule.destroy(device);
			pipelineLayout.destroy(device);
			renderPass.destroy(device);
			swapChain.destroy(device);
			device.destroy();
			surface.destroy(instance);
			debugUtilsMessenger.destroy(instance);
			instance.destroy();
			window.destroyWindow();

			wd::GLFW::Deinit();
		}

		wd::Window window;
		Instance instance{ nullptr };
		DebugUtilsMessenger debugUtilsMessenger{ nullptr };
		PhysicalDevice physicalDevice{ nullptr };
		Surface surface{ nullptr };
		Device device{ nullptr };
		SwapChain swapChain{ nullptr };
		RenderPass renderPass{ nullptr };
		PipelineLayout pipelineLayout{ nullptr };
		VkViewport viewport{};
		VkRect2D scissor{};
		ShadersCompiler shadersCompiler;
		ShaderModule vertexShaderModule{ nullptr };
		ShaderModule fragmentShaderModule{ nullptr };
		std::vector<VkPipelineShaderStageCreateInfo> shadersStages;
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