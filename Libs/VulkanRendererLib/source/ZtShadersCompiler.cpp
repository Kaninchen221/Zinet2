#include "Zinet/VulkanRenderer/ZtShadersCompiler.hpp"

#include "Zinet/Core/ZtFile.hpp"

namespace zt::vulkan_renderer
{
	shaderc_shader_kind ShaderTypeToShaderCShaderKind(const ShaderType shaderType) noexcept
	{
		switch (shaderType)
		{
		case ShaderType::Compute: return shaderc_shader_kind::shaderc_compute_shader;
		case ShaderType::Fragment: return shaderc_shader_kind::shaderc_glsl_fragment_shader;
		case ShaderType::Vertex: return shaderc_shader_kind::shaderc_glsl_vertex_shader;
		}

		auto Logger = core::ConsoleLogger::Create("ShaderTypeToShaderCShaderKind");
		Logger->error("Not supported ShaderType");
		return shaderc_shader_kind::shaderc_glsl_infer_from_source;
	}

	VkShaderStageFlagBits ShaderTypeToVkShaderStage(const ShaderType shaderType) noexcept
	{
		switch (shaderType)
		{
		case ShaderType::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
		case ShaderType::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
		case ShaderType::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
		}

		auto Logger = core::ConsoleLogger::Create("ShaderTypeToVkShaderStage");
		Logger->error("Not supported ShaderType");
		return {};
	}

	shaderc::SpvCompilationResult ShadersCompiler::compileFromFile(const std::filesystem::path& filePath, const ShaderType shaderType) const noexcept
	{
		core::File file;
		file.open(filePath, core::FileOpenMode::Read);
		if (!file.isOpen())
		{
			Logger->error("Couldn't open file: {}", filePath.string());
			return {};
		}

		const auto sourceCode = file.readAll();
		const auto sourceFileName = filePath.filename().string();

		shaderc::Compiler compiler;
		auto result = compiler.CompileGlslToSpv(sourceCode.c_str(), ShaderTypeToShaderCShaderKind(shaderType), sourceFileName.c_str());

		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			Logger->error("Error message: {}", result.GetErrorMessage());
		}
 
		return result;
	}

}