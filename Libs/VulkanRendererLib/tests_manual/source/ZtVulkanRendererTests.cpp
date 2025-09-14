#pragma once

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
#include "Zinet/Core/ZtTimeLog.hpp"
#include "Zinet/Core/ZtGraph.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtWindowEvents.hpp"

#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

#include <implot.h>

// It's just a copy from the automatic tests
// I wanted a fast option to manualy test the renderer
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

			renderer.init(window);

			imGuiIntegration.init(renderer.getRendererContext(), window);

			vertexShaderModule = createShaderModule("shader.vert", ShaderType::Vertex);
			ASSERT_TRUE(vertexShaderModule.isValid());

			fragmentShaderModule = createShaderModule("shader.frag", ShaderType::Fragment);
			ASSERT_TRUE(fragmentShaderModule.isValid());

			auto& vma = renderer.getRendererContext().getVMA();
			auto& device = renderer.getRendererContext().getDevice();

			// Vertex Buffer
			const DrawInfo::Vertices vertices = {
				{{-0.5f, 0.5f, 1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.f, 0.f}},
				{{0.5f,  0.5f, 1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.f, 0.f}},
				{{0.5f,  -0.5f,  1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.f, 1.f}},
				{{-0.5f, -0.5f,  1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.f, 1.f}}
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

				ASSERT_TRUE(device.setDebugName(uniformBuffer, "UniformBuffer_0", VK_OBJECT_TYPE_BUFFER));
			}

			{ // #1
				Buffer& uniformBuffer = uniformBuffers.emplace_back(nullptr);
				const auto uniformBufferCreateInfo = Buffer::GetUniformBufferCreateInfo(uniformData);
				ASSERT_TRUE(uniformBuffer.createBuffer(uniformBufferCreateInfo, vma));

				ASSERT_TRUE(device.setDebugName(uniformBuffer, "UniformBuffer_1", VK_OBJECT_TYPE_BUFFER));
			}

			createTexture();

			const auto samplerCreateInfo = Sampler::GetDefaultCreateInfo();
			ASSERT_TRUE(sampler.create(device, samplerCreateInfo));

			// Set default data for unfiorm buffers
			const auto windowSize = window.getSize();

			transform.setPosition({ 0, 0, 0 });
			transform.setRotation(0);
			transform.setScale({ 50, 50, 1 });

			camera.setPosition(Vector3f(0.00001, 0, 150));
			camera.setLookingAt(Vector3f(0.0f, 0.0f, 0.0f));
			camera.setUpVector(Vector3f(0, 1, 0));

			camera.setFieldOfView(45.f);
			camera.setAspectRatio(windowSize.x / static_cast<float>(windowSize.y));
			camera.setClipping(Vector2f{ 0.0000001f, 10000000.0f });

			transform2.setPosition({ 60, 0, 0 });
			transform2.setRotation(20);
			transform2.setScale({ 50, 50, 1 });
		}

		void TearDown() override
		{
			auto& device = renderer.getRendererContext().getDevice();
			auto& vma = renderer.getRendererContext().getVMA();

			device.waitIdle();

			vertexBuffer.destroy(vma);
			indexBuffer.destroy(vma);
			std::for_each(uniformBuffers.begin(), uniformBuffers.end(), [&vma = vma](auto& buffer) { buffer.destroy(vma); });

			vertexShaderModule.destroy(device);
			fragmentShaderModule.destroy(device);

			texture.destroy(device, vma);
			sampler.destroy(device);

			imGuiIntegration.deinit(renderer.getRendererContext());

			renderer.deinit();

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
		Transform transform;
		Transform transform2;

		std::vector<Buffer> uniformBuffers;
		struct UniformData
		{
			alignas(16) glm::mat4 model;
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 projection;
		};
		UniformData uniformData{};

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
		if (compileResult.empty())
		{
			Ensure(false, "Compile returned empty result");
			return shaderModule;
		}

		const auto& device = renderer.getRendererContext().getDevice();
		shaderModule.create(device, compileResult);

		return shaderModule;
	}

	void VulkanRendererTests::updateUniformBuffersData()
	{
		const auto& vma = renderer.getRendererContext().getVMA();

		uniformData.view = camera.getViewMatrix();
		uniformData.projection = camera.getPerspectiveMatrix();

		uniformData.model = transform.getMatrix();

		auto& firstBuffer = uniformBuffers[0];
		firstBuffer.fillWithObject(uniformData, vma);

		uniformData.model = transform2.getMatrix();

		auto& secondBuffer = uniformBuffers[1];
		secondBuffer.fillWithObject(uniformData, vma);
	}

	void VulkanRendererTests::createTexture()
	{
		const auto& rendererContext = renderer.getRendererContext();
		const auto& vma = rendererContext.getVMA();
		const auto& device = rendererContext.getDevice();
		const auto& queue = rendererContext.getQueue();
		const auto& commandPool = rendererContext.getCommandPool();

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
			.pipelineDescriptorInfo = 
			{ 
				.uniformBuffers = {},
				.texturesInfos = { textureInfo },
			},
			.objectDescriptorInfo =
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

		bool useImGui = true;

		core::Graph beginFrameTimeGraph{ "Next Image", 0.f, 0.1f, 1000 };
		core::Clock nextImageClock;

		core::Graph drawTimeGraph{ "Draw", 0.f, 0.1f, 1000 };
		core::Clock drawClock;

		core::Graph submitTimeGraph{ "Submit", 0.f, 0.1f, 1000 };
		core::Clock submitClock;

		core::Graph endFrameTimeGraph{ "Display Current Image", 0.f, 0.1f, 1000 };
		core::Clock displayCurrentImageClock;

		while (window.isOpen())
		{
			if (useImGui)
			{
				ImGuiIntegration::ImplSpecificNewFrame();

				ImGui::NewFrame();
				ImGui::ShowDemoWindow();

				ImGui::SetNextWindowBgAlpha(0.80f); // Transparent background

				const float padding = 10.0f;
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
				ImVec2 workSize = viewport->WorkSize;

				ImVec2 windowPos, windowPivot;
				windowPos.x = work_pos.x + padding;
				windowPos.y = work_pos.y + padding;
				windowPivot.x = 0.0f;
				windowPivot.y = 0.0f;
				ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, windowPivot);
				//ImGui::SetNextWindowSize(workSize, ImGuiCond_Once);
				ImGui::SetNextWindowSizeConstraints(
					ImVec2(0, workSize.y - padding * 2),
					ImVec2(workSize.x - padding * 2, workSize.y - padding * 2)
				);

				ImGuiWindowFlags windowFlags =
					ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

				bool isOpen = false;
				if (ImGui::Begin("EditorOverlay", &isOpen, windowFlags))
				{
					if (ImGui::CollapsingHeader("Sprite1"))
					{
						transform.show();
					}

					if (ImGui::CollapsingHeader("Sprite2"))
					{
						transform2.show();
					}
				}
				ImGui::End();

				core::Graph::ShowPlotAny();
				beginFrameTimeGraph.show();
				drawTimeGraph.show();
				submitTimeGraph.show();
				endFrameTimeGraph.show();

				ImGui::EndFrame();

				imGuiIntegration.prepareRenderData();
			}

			ASSERT_TRUE(renderer.createPipeline(drawInfo));
			ASSERT_TRUE(renderer.getGraphicsPipeline().isValid());

			nextImageClock.restart();
			ASSERT_TRUE(renderer.nextImage());
			beginFrameTimeGraph.update(nextImageClock.restart().getAsMilliseconds());

			drawClock.restart();
			renderer.draw(drawInfo);
			drawTimeGraph.update(drawClock.restart().getAsMilliseconds());

			submitClock.restart();
			ASSERT_TRUE(renderer.submitDrawInfo());
			submitTimeGraph.update(submitClock.restart().getAsMilliseconds());

			displayCurrentImageClock.restart();
			ASSERT_TRUE(renderer.displayCurrentImage());
			endFrameTimeGraph.update(displayCurrentImageClock.restart().getAsMilliseconds());

			windowEvents.pollEvents();

			// Game logic

			updateUniformBuffersData();

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