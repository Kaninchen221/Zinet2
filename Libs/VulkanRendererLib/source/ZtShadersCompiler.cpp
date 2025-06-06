#include "Zinet/VulkanRenderer/ZtShadersCompiler.hpp"

#include "Zinet/Core/ZtFile.hpp"

namespace zt::vulkan_renderer
{
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