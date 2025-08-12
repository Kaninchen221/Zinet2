#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include <Zinet/VulkanRenderer/ZtShadersCompiler.hpp>
#include <Zinet/VulkanRenderer/ZtShaderModule.hpp>

#include <imgui.h>

namespace zt::gameplay
{
	using namespace vulkan_renderer;

	bool AssetShader::load(const core::Path& rootPath)
	{
		if (!AssetText::load(rootPath))
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
		if (compileResult.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			Logger->error("Couldn't compile shader: {}", metaData.value("fileNameExt", "fileNameExt"));
			return false;
		}

		auto& engineContext = EngineContext::Get();
		auto systemRenderer = engineContext.getSystem<SystemRenderer>();
		if (!systemRenderer)
		{
			Logger->error("Invalid system renderer");
			return false;
		}

		const auto& device = systemRenderer->getRenderer().getRendererContext().device;
		shaderModule.create(device, compileResult);

		loaded = true;
		return true;
	}

	void AssetShader::unload()
	{
		AssetText::unload();

		auto& engineContext = EngineContext::Get();
		auto systemRenderer = engineContext.getSystem<SystemRenderer>();
		if (!systemRenderer)
		{
			Logger->error("Invalid system renderer");
			return;
		}

		const auto& device = systemRenderer->getRenderer().getRendererContext().device;
		shaderModule.destroy(device);
	}

	void AssetShader::imGui()
	{
		Asset::imGui();
		if (!isLoaded())
			return;

		ImGui::Separator();
		ImGui::Text("Loaded shader code:");
		ImGui::Text(text.c_str());
	}

}