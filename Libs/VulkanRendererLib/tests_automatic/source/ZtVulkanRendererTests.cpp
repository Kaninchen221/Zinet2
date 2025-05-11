#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

#include "Zinet/Core/ZtPaths.hpp"

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
			window.create(800, 800);

			renderer.start(window);

			vertexShaderModule = createShaderModule("shader.vert", ShaderType::Vertex);
			ASSERT_TRUE(vertexShaderModule.isValid());

			fragmentShaderModule = createShaderModule("shader.frag", ShaderType::Fragment);
			ASSERT_TRUE(fragmentShaderModule.isValid());

			auto& vma = renderer.getRendererContext().vma;

			// Vertex Buffer
			const DrawInfo::Vertices vertices = {
				{{-0.5f, -0.5f, 1.f}, {1.0f, 0.0f, 0.0f}},
				{{0.5f,  -0.5f, 1.f}, {0.0f, 1.0f, 0.0f}},
				{{0.5f,  0.5f,  1.f}, {0.0f, 0.0f, 1.0f}},
				{{-0.5f, 0.5f,  1.f}, {1.0f, 1.0f, 1.0f}}
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
		}

		void TearDown() override
		{
			auto& device = renderer.getRendererContext().device;
			auto& vma = renderer.getRendererContext().vma;

			device.waitIdle();

			vertexBuffer.destroy(vma);
			indexBuffer.destroy(vma);

			vertexShaderModule.destroy(device);
			fragmentShaderModule.destroy(device);

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

		ShaderModule createShaderModule(std::string_view sourceCodeFileName, ShaderType shaderType);
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

	TEST_F(VulkanRendererTests, Test)
	{
		const DrawInfo drawInfo
		{
			.vertexShaderModule = vertexShaderModule,
			.fragmentShaderModule = fragmentShaderModule,
			.vertexBuffer = vertexBuffer,
			.indexBuffer = indexBuffer,
			.indexCount = static_cast<std::uint32_t>(indices.size())
		};

		while (window.isOpen())
		{
			windowEvents.pollEvents();

			renderer.draw(drawInfo);

			window.requestCloseWindow();
		}
	}
}