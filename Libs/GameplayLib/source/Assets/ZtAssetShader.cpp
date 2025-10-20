#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"

#include "Zinet/Core/ZtImgui.hpp"

namespace zt::gameplay::asset
{
	using namespace vulkan_renderer;

	bool Shader::load(const core::Path& rootPath)
	{
		if (!Text::load(rootPath))
			return false;
		loaded = false;

		ShadersCompiler shadersCompiler;

		const auto shaderTypeStr = metaData.value("fileExtension", "fileExtension");
		const auto shaderType = FromStringToShaderType(shaderTypeStr);
		if (shaderType == ShaderType::Invalid)
		{
			Logger->error("Invalid shader type str: {}", shaderTypeStr);
			return false;
		}

		const auto fileName = metaData.value("fileName", "fileName");
		const auto compileResult = shadersCompiler.compileFromString(text, shaderType, fileName);
		if (compileResult.empty())
		{
			Logger->error("Couldn't compile shader: {}", metaData.value("fileNameExt", "fileNameExt"));
			return false;
		}

		loaded = true;
		return true;
	}

	void Shader::unload()
	{
		Text::unload();

		result.clear();
	}

	void Shader::show()
	{
		Asset::show();
		if (!isLoaded())
			return;

		ImGui::Separator();
		ImGui::TextCStr("Loaded shader code:");
		ImGui::Text(text);
	}

}