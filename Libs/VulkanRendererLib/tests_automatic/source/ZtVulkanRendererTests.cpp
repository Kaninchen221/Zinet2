#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtTexture.hpp"
#include "Zinet/VulkanRenderer/ZtQueueUtils.hpp"
#include "Zinet/VulkanRenderer/ZtSampler.hpp"
#include "Zinet/VulkanRenderer/ZtTransform.hpp"
#include "Zinet/VulkanRenderer/ZtCamera.hpp"
#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"

#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtClock.hpp"
#include "Zinet/Core/ZtImage.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtWindowEvents.hpp"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"

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
			window.makeWindowTransparentWhileUsingVulkan();

			renderer.start(window);

			imGuiIntegration.init(renderer.getRendererContext(), window);

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
				const auto uniformBufferCreateInfo = Buffer::GetUniformBufferCreateInfo(uniformData);
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

			imGuiIntegration.deinit(renderer.getRendererContext());

			renderer.shutdown();

			window.destroyWindow();
			wd::GLFW::Deinit();
		}

		VulkanRenderer renderer;
		ImGuiIntegration imGuiIntegration;
		wd::Window window;
		wd::WindowEvents windowEvents{ window };
		ShaderModule vertexShaderModule{ nullptr };
		ShaderModule fragmentShaderModule{ nullptr };
		Buffer vertexBuffer{ nullptr };
		Buffer indexBuffer{ nullptr };
		DrawInfo::Indices indices;

		std::vector<Buffer> uniformBuffers;
		struct UniformData
		{
			alignas(16) glm::mat4 model;
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 projection;
		};
		UniformData uniformData;

		Texture texture;
		Sampler sampler{ nullptr };

		Camera camera;

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
		const auto windowSize = window.getSize();

		Transform transform;
		transform.setPosition({ 0, 0, 0 });
		transform.setRotation(0);
		transform.setScale({ 1, 1, 1 });

		camera.setPosition(Vector3f(0.00001, 0, 8));
		camera.setLookingAt(Vector3f(0.0f, 0.0f, 0.0f));
		camera.setUpVector(Vector3f(0, 0, 1));

		camera.setFieldOfView(45.f);
		camera.setAspectRatio(windowSize.x / static_cast<float>(windowSize.y));
		camera.setClipping(Vector2f{ 0.01f, 100.0f });

		uniformData.model = transform.getMatrix();
		uniformData.view = camera.getViewMatrix();
		uniformData.projection = camera.getPerspectiveMatrix();

		auto& firstBuffer = uniformBuffers[0];
		firstBuffer.fillWithObject(uniformData, vma);

		transform.setPosition({ 2, 0, 0 });
		transform.setRotation(20);
		transform.setScale({ 1, 1, 1 });
		uniformData.model = transform.getMatrix();

		auto& secondBuffer = uniformBuffers[1];
		secondBuffer.fillWithObject(uniformData, vma);
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
			},
			.additionalCommands = { ImGuiIntegration::DrawCommand }
		};

		core::Clock fpsClock;
		size_t fpsCount = 0;
		fpsClock.start();

		core::Clock turnOffTest;
		turnOffTest.start();

		while (window.isOpen())
		{
			if (turnOffTest.getElapsedTime().getAsSeconds() > 4.f)
				window.requestCloseWindow();

			windowEvents.pollEvents();

			// Game logic

			imGuiIntegration.implSpecificNewFrame();

			ImGui::NewFrame();


			ImGui::EndFrame();

			updateUniformBuffersData();

			// Rendering logic

			ASSERT_TRUE(renderer.createPipeline(drawInfo));
			ASSERT_TRUE(renderer.getGraphicsPipeline().isValid());

			ASSERT_TRUE(renderer.beginFrame());

			imGuiIntegration.prepareRenderData();

			renderer.draw(drawInfo);

			ASSERT_TRUE(renderer.submit());

			ASSERT_TRUE(renderer.endFrame());

			// Post logic

			fpsCount++;
			if (fpsClock.getElapsedTime().getAsSeconds() >= 1.0f)
			{
				const std::string title = fmt::format("Zinet FPS: {}", fpsCount);
				window.setTitle(title);
				fpsCount = 0u;
				fpsClock.restart();
			}
		}
	}
}