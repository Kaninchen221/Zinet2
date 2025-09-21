#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtSampler.hpp"

#include "Zinet/Window/ZtWindow.hpp"

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

		GraphicsPipeline graphicsPipeline;

		static_assert(std::is_default_constructible_v<GraphicsPipeline>);
		static_assert(!std::is_copy_constructible_v<GraphicsPipeline>);
		static_assert(!std::is_copy_assignable_v<GraphicsPipeline>);
		static_assert(std::is_move_constructible_v<GraphicsPipeline>);
		static_assert(std::is_move_assignable_v<GraphicsPipeline>);
		static_assert(std::is_destructible_v<GraphicsPipeline>);
	};

	TEST_F(GraphicsPipelineTests, CreateTest)
	{
		ASSERT_FALSE(graphicsPipeline.isValid());

		wd::GLFW::Init();

		wd::Window window;
		ASSERT_TRUE(window.create(2, 2));

		RendererContext rendererContext;
		ASSERT_TRUE(rendererContext.create(window));

		auto vertexShaderModule = CreateShaderModule("simple_triangle_shader.vert", ShaderType::Vertex, rendererContext.getDevice());
		ASSERT_TRUE(vertexShaderModule);

		auto fragmentShaderModule = CreateShaderModule("simple_triangle_shader.frag", ShaderType::Fragment, rendererContext.getDevice());
		ASSERT_TRUE(fragmentShaderModule);

		struct UniformData
		{
			alignas(16) glm::mat4 model;
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 projection;
		};
		UniformData uniformData{};

		std::vector<Buffer> uniformBuffers;
		uniformBuffers.emplace_back(CreateUniformBuffer(rendererContext.getVMA(), uniformData));
		uniformBuffers.emplace_back(CreateUniformBuffer(rendererContext.getVMA(), uniformData));

		Texture texture = CreateTexture(rendererContext);
		Sampler sampler = CreateSampler(rendererContext.getDevice());

		TextureInfo textureInfo
		{
			.texture = &texture,
			.sampler = &sampler,
			.shaderType = ShaderType::Fragment
		};

		Buffer vertexBuffer{ nullptr };
		Buffer indexBuffer{ nullptr };

		DrawInfo drawInfo
		{
			.vertexShaderModule = &vertexShaderModule,
			.fragmentShaderModule = &fragmentShaderModule,
			.vertexBuffer = &vertexBuffer,
			.indexBuffer = &indexBuffer,
			.indexCount = 0,
			.instances = 1,
			.objectDescriptorInfo =
			{
				.buffersPerType = {},
				.texturesInfos = {},
			},
			.pipelineDescriptorInfo =
			{
				.buffersPerType = {},
				.texturesInfos = {},
			}
		};

		GraphicsPipelineCreateInfo createInfo
		{
			.rendererContext = rendererContext,
			.drawInfo = drawInfo,
			.descriptorSetsCount = rendererContext.getDisplayImagesCount()
		};

		graphicsPipeline.create(createInfo);
		graphicsPipeline.destroy(rendererContext);

		texture.destroy(rendererContext.getDevice(), rendererContext.getVMA());
		sampler.destroy(rendererContext.getDevice());

		vertexShaderModule.destroy(rendererContext.getDevice());
		fragmentShaderModule.destroy(rendererContext.getDevice());

		destroyAll(uniformBuffers, rendererContext.getVMA());

		rendererContext.destroy();
		window.destroyWindow();
		wd::GLFW::Deinit();
	}
}