#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtShadersCompiler.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtRenderPass.hpp"
#include "Zinet/VulkanRenderer/ZtPipelineLayout.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Core/ZtPaths.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer::tests
{
	class PipelineTests : public ::testing::Test
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
			invalidateAll(physicalDevices);
			ASSERT_TRUE(physicalDevice.isValid());

			ASSERT_TRUE(surface.create(instance, window));

			ASSERT_TRUE(device.create(instance, physicalDevice, surface));

			ASSERT_TRUE(swapChain.create(device, physicalDevice, surface, window.getFramebufferSize()));

			const auto renderPassCreateInfo = RenderPass::GetPresentCreateInfo(VK_FORMAT_B8G8R8A8_SRGB);
			ASSERT_TRUE(renderPass.create(device, renderPassCreateInfo));

			const auto pipelineLayoutCreateInfo = PipelineLayout::GetDefaultCreateInfo({});
			ASSERT_TRUE(pipelineLayout.create(device, pipelineLayoutCreateInfo));

			const auto viewportSize = window.getFramebufferSize();
			viewport = { 0, 0, static_cast<float>(viewportSize.x), static_cast<float>(viewportSize.y), 0.f, 1.f };

			scissor = { { 0, 0 }, { static_cast<std::uint32_t>(viewportSize.x), static_cast<std::uint32_t>(viewportSize.y) } };

			// Shaders
			const auto contentFolderPath = core::Paths::CurrentProjectRootPath() / "test_files";

			const auto vertexShaderFilePath = contentFolderPath / "simple_triangle_shader.vert";
			const auto vertexShaderResult = shadersCompiler.compileFromFile(vertexShaderFilePath, ShaderType::Vertex);
			ASSERT_FALSE(vertexShaderResult.empty());

			ASSERT_TRUE(vertexShaderModule.create(device, vertexShaderResult));

			const auto fragmentShaderFilePath = contentFolderPath / "simple_triangle_shader.frag";
			const auto fragmentShaderResult = shadersCompiler.compileFromFile(fragmentShaderFilePath, ShaderType::Fragment);
			ASSERT_FALSE(fragmentShaderResult.empty());

			ASSERT_TRUE(fragmentShaderModule.create(device, fragmentShaderResult));

			uniformBuffers.emplace_back(nullptr);

			// DrawInfo
			DrawInfo drawInfo =
			{
				.vertexShaderModule = &vertexShaderModule,
				.fragmentShaderModule = &fragmentShaderModule,
				.vertexBuffer = &vertexBuffer,
				.indexBuffer = &indexBuffer,
				.indexCount = 0,
				.pipelineDescriptorInfo = 
				{ 
					.buffersPerType = 
					{ 
						{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, { &uniformBuffers[0] } } 
					}
				}
			};

			ASSERT_TRUE(
				pipeline.create(device, pipelineLayout, renderPass, viewport, scissor, drawInfo)
			);

			ASSERT_TRUE(pipeline.isValid());
		}

		void TearDown() override
		{
			pipeline.destroy(device);
			ASSERT_FALSE(pipeline.isValid());

			vertexShaderModule.destroy(device);
			fragmentShaderModule.destroy(device);

			pipelineLayout.destroy(device);
			renderPass.destroy(device);
			swapChain.destroy(device);
			device.destroy();
			physicalDevice.invalidate();
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
		Buffer vertexBuffer{ nullptr };
		Buffer indexBuffer{ nullptr };
		std::vector<Buffer> uniformBuffers;
		Pipeline pipeline{ nullptr };

		static_assert(VulkanObjectDecoratorStaticTest<Pipeline, VkPipeline>());
	};

	TEST_F(PipelineTests, PassTest)
	{}

	TEST(Pipeline, CreateVkPipelineDynamicStateCreateInfoTest)
	{
		const std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		Pipeline pipeline{ nullptr };
		const VkPipelineDynamicStateCreateInfo createInfo =
			pipeline.createVkPipelineDynamicStateCreateInfo(dynamicStates);

		EXPECT_EQ(createInfo.pDynamicStates, dynamicStates.data());
		EXPECT_EQ(createInfo.dynamicStateCount, dynamicStates.size());
	}

	TEST(Pipeline, CreateVkPipelineVertexInputStateCreateInfoTest)
	{
		const auto bindingDescriptions = GetVertexInputBindingDescription();
		const auto attributesDescriptions = GetVertexInputAttributesDescriptions();

		Pipeline pipeline{ nullptr };
		const VkPipelineVertexInputStateCreateInfo createInfo =
			pipeline.createVkPipelineVertexInputStateCreateInfo(&bindingDescriptions, &attributesDescriptions);
	}

	TEST(Pipeline, CreateVkPipelineInputAssemblyStateCreateInfoTest)
	{
		Pipeline pipeline{ nullptr };
		const VkPipelineInputAssemblyStateCreateInfo createInfo =
			pipeline.createVkPipelineInputAssemblyStateCreateInfo();
	}

	TEST(Pipeline, CreateVkPipelineViewportStateCreateInfoTest)
	{
		Pipeline pipeline{ nullptr };

		VkViewport viewport{};
		VkRect2D scissor{};

		const VkPipelineViewportStateCreateInfo createInfo =
			pipeline.createVkPipelineViewportStateCreateInfo(viewport, scissor);

		EXPECT_EQ(createInfo.pViewports, &viewport);
		EXPECT_EQ(createInfo.viewportCount, 1);
		EXPECT_EQ(createInfo.pScissors, &scissor);
		EXPECT_EQ(createInfo.scissorCount, 1);
	}

	TEST(Pipeline, CreateVkPipelineRasterizationStateCreateInfoTest)
	{
		Pipeline pipeline{ nullptr };
		const VkPipelineRasterizationStateCreateInfo createInfo =
			pipeline.createVkPipelineRasterizationStateCreateInfo();
	}

	TEST(Pipeline, CreateVkPipelineMultisampleStateCreateInfoTest)
	{
		Pipeline pipeline{ nullptr };
		const VkPipelineMultisampleStateCreateInfo createInfo =
			pipeline.createVkPipelineMultisampleStateCreateInfo();
	}

	TEST(Pipeline, CreateVkPipelineColorBlendAttachmentStateTest)
	{
		Pipeline pipeline{ nullptr };
		const VkPipelineColorBlendAttachmentState attachmentState =
			pipeline.createVkPipelineColorBlendAttachmentState();
	}

	TEST(Pipeline, CreateVkPipelineColorBlendStateCreateInfoTest)
	{
		Pipeline pipeline{ nullptr };

		const VkPipelineColorBlendAttachmentState attachmentState =
			pipeline.createVkPipelineColorBlendAttachmentState();

		const VkPipelineColorBlendStateCreateInfo createInfo =
			pipeline.createVkPipelineColorBlendStateCreateInfo(attachmentState);
	}
}
