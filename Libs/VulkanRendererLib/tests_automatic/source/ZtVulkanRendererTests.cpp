#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtTexture.hpp"
#include "Zinet/VulkanRenderer/ZtQueueUtils.hpp"
#include "Zinet/VulkanRenderer/ZtSampler.hpp"

#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtClock.hpp"
#include "Zinet/Core/ZtImage.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtWindowEvents.hpp"

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
			wd::GLFW::Init(false);

			zt::wd::Window::SetTransparentFramebuffer(true);

			window.create(800, 800);

			renderer.start(window);

			vertexShaderModule = createShaderModule("shader.vert", ShaderType::Vertex);
			ASSERT_TRUE(vertexShaderModule.isValid());

			fragmentShaderModule = createShaderModule("shader.frag", ShaderType::Fragment);
			ASSERT_TRUE(fragmentShaderModule.isValid());

			auto& vma = renderer.getRendererContext().vma;
			auto& device = renderer.getRendererContext().device;

			// Vertex Buffer
			const DrawInfo::Vertices vertices = {
				{{-0.5f, -0.5f, 1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.f, 0.f}},
				{{0.5f,  -0.5f, 1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.f, 0.f}},
				{{0.5f,  0.5f,  1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.f, 1.f}},
				{{-0.5f, 0.5f,  1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.f, 1.f}}
			};

			const auto vertexBufferCreateInfo = Buffer::GetVertexBufferCreateInfo(vertices);
			ASSERT_TRUE(vertexBuffer.createBuffer(vertexBufferCreateInfo, vma));
			ASSERT_TRUE(vertexBuffer.fillWithSTDContainer(vertices, vma));

			// Index Buffer
			indices =
			{
				0, 1, 2,
				2, 3, 0
			};

			const auto indexBufferCreateInfo = Buffer::GetIndexBufferCreateInfo(indices);
			ASSERT_TRUE(indexBuffer.createBuffer(indexBufferCreateInfo, vma));
			ASSERT_TRUE(indexBuffer.fillWithSTDContainer(indices, vma));

			// Uniform Buffers
			{ // #0
				Buffer& uniformBuffer = uniformBuffers.emplace_back(nullptr);
				const auto uniformBufferCreateInfo = Buffer::GetUniformBufferCreateInfo(uniformData);
				ASSERT_TRUE(uniformBuffer.createBuffer(uniformBufferCreateInfo, vma));

				ASSERT_TRUE(device.setDebugName(uniformBuffer, "UniformBuffer_0"));
			}

			{ // #1
				Buffer& uniformBuffer = uniformBuffers.emplace_back(nullptr);
				const auto uniformBufferCreateInfo = Buffer::GetUniformBufferCreateInfo(uniformData2);
				ASSERT_TRUE(uniformBuffer.createBuffer(uniformBufferCreateInfo, vma));

				ASSERT_TRUE(device.setDebugName(uniformBuffer, "UniformBuffer_1"));
			}

			createTexture();

			const auto samplerCreateInfo = Sampler::GetDefaultCreateInfo();
			ASSERT_TRUE(sampler.create(device, samplerCreateInfo));
		}

		void TearDown() override
		{
			auto& device = renderer.getRendererContext().device;
			auto& vma = renderer.getRendererContext().vma;

			device.waitIdle();

			vertexBuffer.destroy(vma);
			indexBuffer.destroy(vma);
			std::for_each(uniformBuffers.begin(), uniformBuffers.end(), [&vma = vma](auto& buffer) { buffer.destroy(vma); });

			vertexShaderModule.destroy(device);
			fragmentShaderModule.destroy(device);

			texture.destroy(device, vma);
			sampler.destroy(device);

			renderer.shutdown();

			window.destroyWindow();
			wd::GLFW::Deinit();
		}

		VulkanRenderer renderer;
		wd::Window window;
		wd::WindowEvents windowEvents{ window };
		ShaderModule vertexShaderModule{ nullptr };
		ShaderModule fragmentShaderModule{ nullptr };
		Buffer vertexBuffer{ nullptr };
		Buffer indexBuffer{ nullptr };
		DrawInfo::Indices indices;

		std::vector<Buffer> uniformBuffers;
		/// TODO: Transform and Camera
		struct UniformData
		{
			alignas(8) Vector2f position{ -0.5, 0.f };
			alignas(4) float colorScalar{ 0.f };
		};
		UniformData uniformData;
		UniformData uniformData2;
		float uniformDataScalar = 1.f;

		Texture texture;
		Sampler sampler{ nullptr };

		ShaderModule createShaderModule(std::string_view sourceCodeFileName, ShaderType shaderType);

		void updateUniformBuffersData();

		void createTexture();
	};

	ShaderModule VulkanRendererTests::createShaderModule(std::string_view sourceCodeFileName, ShaderType shaderType)
	{
		ShadersCompiler shadersCompiler;
		ShaderModule shaderModule{ nullptr };

		const auto contentFolderPath = core::Paths::CurrentProjectRootPath() / "test_files";

		const auto fullPath = contentFolderPath / sourceCodeFileName;
		const auto compileResult = shadersCompiler.compileFromFile(fullPath, shaderType);
		if (compileResult.GetCompilationStatus() != shaderc_compilation_status_success)
			return shaderModule;

		const auto& device = renderer.getRendererContext().device;
		shaderModule.create(device, compileResult);

		return shaderModule;
	}

	void VulkanRendererTests::updateUniformBuffersData()
	{
		const auto& vma = renderer.getRendererContext().vma;
		
		// First uniform buffer
		{
			auto& position = uniformData.position;
			if (position.x <= -0.5f || position.x >= 0.5f)
			{
				uniformDataScalar *= -1.f;
				position.x = std::clamp(position.x, -0.5f, 0.5f);
			}
			position.x += uniformDataScalar * 0.0001f;
		}

		{
			auto& colorScalar = uniformData.colorScalar;
			colorScalar += uniformDataScalar * 0.00015f;
			colorScalar = std::clamp(colorScalar, 0.1f, 0.50f);
		}
		uniformBuffers[0].fillWithObject(uniformData, vma);
		
		// Second uniform buffer
		uniformData2.position = uniformData.position * -1.f;
		uniformData2.colorScalar = 1.f;
		uniformBuffers[1].fillWithObject(uniformData2, vma);
	}

	void VulkanRendererTests::createTexture()
	{
		const auto& rendererContext = renderer.getRendererContext();
		const auto& vma = rendererContext.vma;
		const auto& device = rendererContext.device;
		const auto& queue = rendererContext.queue;
		const auto& commandPool = rendererContext.commandPool;

		const std::filesystem::path testFolderPath = core::Paths::CurrentProjectRootPath() / "test_files";
		const std::filesystem::path imagePath = testFolderPath / "image.png";

		core::Image sourceImage;
		ASSERT_TRUE(sourceImage.loadFromFile(imagePath, 4));

		Buffer buffer{ nullptr };
		const auto bufferCreateInfo = Buffer::GetImageBufferCreateInfo(sourceImage);
		ASSERT_TRUE(buffer.createBuffer(bufferCreateInfo, vma));

		ASSERT_TRUE(buffer.fillWithImage(sourceImage, vma));

		ASSERT_TRUE(texture.create(device, vma, { sourceImage.getWidth(), sourceImage.getHeight() }));
		ASSERT_TRUE(texture.isValid());

		const auto commands = [&texture = texture, &sourceImage = sourceImage, &buffer = buffer](const CommandBuffer& commandBuffer)
		{
			FillWithImageBufferInput input
			{
				.buffer = buffer,
				.commandBuffer = commandBuffer,
				.imageExtent = { sourceImage.getWidth(), sourceImage.getHeight() }
			};
			texture.fillWithImageBuffer(input);
		};
		SubmitSingleCommandBufferWaitIdle(device, queue, commandPool, commands);

		buffer.destroy(vma);
	}

	TEST_F(VulkanRendererTests, VulkanRendererTest)
	{
		using namespace std::chrono_literals;

		TextureInfo textureInfo
		{
			.texture = &texture,
			.sampler = &sampler,
			.shaderType = ShaderType::Fragment
		};

		DrawInfo drawInfo
		{
			.vertexShaderModule = &vertexShaderModule,
			.fragmentShaderModule = &fragmentShaderModule,
			.vertexBuffer = &vertexBuffer,
			.indexBuffer = &indexBuffer,
			.indexCount = static_cast<std::uint32_t>(indices.size()),
			.instances = 2u,
			.pipelineDescriptorInfo = { {}, { textureInfo }},
			.drawCallDescriptorInfo =
			{
				.uniformBuffers = { { &uniformBuffers[0] }, { &uniformBuffers[1] } },
				.texturesInfos{}
			}
		};

		core::Clock fpsClock;
		size_t fpsCount = 0;
		fpsClock.start();

		core::Clock turnOffTest;
		turnOffTest.start();

		while (window.isOpen())
		{
			if (turnOffTest.getElapsedTime().getAsSeconds() > 4.f)
				break;

			windowEvents.pollEvents();

			updateUniformBuffersData();

			ASSERT_TRUE(renderer.createPipeline(drawInfo));
			ASSERT_TRUE(renderer.getGraphicsPipeline().isValid());

			ASSERT_TRUE(renderer.beginFrame());

			renderer.draw(drawInfo);

			ASSERT_TRUE(renderer.submit());

			ASSERT_TRUE(renderer.endFrame());

			fpsCount++;
			if (fpsClock.getElapsedTime().getAsSeconds() >= 1.0f)
			{
				const std::string title = fmt::format("Zinet FPS: {}", fpsCount);
				window.setTitle(title);
				fpsCount = 0u;
				fpsClock.restart();
			}

			//window.requestCloseWindow();
		}
	}
}