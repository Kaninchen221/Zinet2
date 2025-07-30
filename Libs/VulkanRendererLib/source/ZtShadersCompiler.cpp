#include "Zinet/VulkanRenderer/ZtShadersCompiler.hpp"

#include "Zinet/Core/ZtFile.hpp"

namespace zt::vulkan_renderer
{
	shaderc::SpvCompilationResult ShadersCompiler::compileFromFile(const std::filesystem::path& filePath, ShaderType shaderType) const ZINET_API_POST
	{
		core::File file;
		file.open(filePath, core::FileOpenMode::Read);
		if (!file.isOpen())
		{
			Logger->error("Couldn't open file: {}", filePath.string());
			return {};
		}

		const auto sourceCode = file.readAll();
		const auto sourceFileName = filePath.filename().generic_string();

		return compileFromString(sourceCode, shaderType, sourceFileName);
	}
	shaderc::SpvCompilationResult ShadersCompiler::compileFromString(const std::string& source, ShaderType shaderType, const std::string& fileName) const ZINET_API_POST
	{
		shaderc::Compiler compiler;
		auto result = compiler.CompileGlslToSpv(source.c_str(), ShaderTypeToShaderCShaderKind(shaderType), fileName.c_str());

		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			Logger->error("Error message: {}", result.GetErrorMessage());
		}

		return result;
	}
}