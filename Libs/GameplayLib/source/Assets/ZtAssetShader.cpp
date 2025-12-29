#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"

#include "Zinet/Core/ZtImgui.hpp"
#include "Zinet/Core/ZtPaths.hpp"

namespace zt::gameplay::asset
{
	using namespace vulkan_renderer;

	bool Shader::load()
	{
		if (!Text::load())
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
		result = shadersCompiler.compileFromString(text, shaderType, fileName);
		if (result.empty())
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

	Shader::ResourceOptT Shader::createResource(vulkan_renderer::RendererContext& rendererContext)
	{
		auto& device = rendererContext.getDevice();

		vulkan_renderer::ShaderModule shaderModule{ nullptr };
		if (!shaderModule.create(device, result))
			return {};

		return shaderModule;
	}

}