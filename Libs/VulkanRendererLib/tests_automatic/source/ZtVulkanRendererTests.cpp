#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"

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

			vertexShaderModule = createShaderModule("simple_triangle_shader.vert", ShaderType::Vertex);
			ASSERT_TRUE(vertexShaderModule.isValid());

			fragmentShaderModule = createShaderModule("simple_triangle_shader.frag", ShaderType::Fragment);
			ASSERT_TRUE(fragmentShaderModule.isValid());
		}

		void TearDown() override
		{
			vertexShaderModule.destroy(renderer.getDevice());
			fragmentShaderModule.destroy(renderer.getDevice());

			renderer.shutdown();

			window.destroyWindow();
			wd::GLFW::Deinit();
		}

		VulkanRenderer renderer;
		wd::Window window;
		wd::WindowEvents windowEvents{ window };
		ShaderModule vertexShaderModule{ nullptr };
		ShaderModule fragmentShaderModule{ nullptr };

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

		shaderModule.create(renderer.getDevice(), compileResult);

		return shaderModule;
	}

	TEST_F(VulkanRendererTests, Test)
	{
		DrawInfo drawInfo
		{
			.vertexShaderModule = vertexShaderModule,
			.fragmentShaderModule = fragmentShaderModule
		};

		while (window.isOpen())
		{
			windowEvents.pollEvents();

			// For now empty because we store draw data in shaders
			renderer.draw(drawInfo); 

			window.requestCloseWindow();
		}

	}
}