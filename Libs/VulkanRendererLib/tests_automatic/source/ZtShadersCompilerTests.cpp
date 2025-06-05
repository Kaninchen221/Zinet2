#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include <gtest/gtest.h>

#include "Zinet/VulkanRenderer/ZtShadersCompiler.hpp"

#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtFile.hpp"

namespace zt::vulkan_renderer::tests
{
	class ShadersCompilerTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		ShadersCompiler shadersCompiler;

		static_assert(std::is_default_constructible_v<ShadersCompiler>);
		static_assert(std::is_copy_constructible_v<ShadersCompiler>);
		static_assert(std::is_copy_assignable_v<ShadersCompiler>);
		static_assert(std::is_move_constructible_v<ShadersCompiler>);
		static_assert(std::is_move_assignable_v<ShadersCompiler>);
		static_assert(std::is_destructible_v<ShadersCompiler>);
	};

	TEST_F(ShadersCompilerTests, PassTest)
	{}
	
	TEST_F(ShadersCompilerTests, CompileShaderTest)
	{
		const auto contentFolderPath = core::Paths::CurrentProjectRootPath() / "test_files";

		const auto vertShaderFilePath = contentFolderPath / "simple_triangle_shader.vert";
		const auto vertShaderResult = shadersCompiler.compileFromFile(vertShaderFilePath, ShaderType::Vertex);
		ASSERT_EQ(vertShaderResult.GetCompilationStatus(), shaderc_compilation_status_success);

		const auto fragShaderFilePath = contentFolderPath / "simple_triangle_shader.frag";
		const auto fragShaderResult = shadersCompiler.compileFromFile(fragShaderFilePath, ShaderType::Fragment);
		ASSERT_EQ(fragShaderResult.GetCompilationStatus(), shaderc_compilation_status_success);
	}
}