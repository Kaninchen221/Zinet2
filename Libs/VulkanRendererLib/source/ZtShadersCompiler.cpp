#include "Zinet/VulkanRenderer/ZtShadersCompiler.hpp"

#include "Zinet/Core/ZtFile.hpp"

namespace zt::vulkan_renderer
{
	shaderc_shader_kind ShaderTypeToShaderCShaderKind(const ShaderType shaderType)
	{
		switch (shaderType)
		{
		case ShaderType::Compute: return shaderc_shader_kind::shaderc_compute_shader;
		case ShaderType::Fragment: return shaderc_shader_kind::shaderc_glsl_fragment_shader;
		case ShaderType::Vertex: return shaderc_shader_kind::shaderc_glsl_vertex_shader;
		}

		return shaderc_shader_kind::shaderc_glsl_infer_from_source;
	}

	shaderc::SpvCompilationResult ShadersCompiler::compileFromFile(const std::filesystem::path& filePath, const ShaderType shaderType)
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