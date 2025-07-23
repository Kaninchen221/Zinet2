#include "Zinet/VulkanRenderer/Assets/ZtAssetShader.hpp"

#include <imgui.h>

namespace zt::vulkan_renderer::assets
{
	// TODO: Derive from AssetText?
	bool AssetShader::load(const core::Path& rootPath) ZINET_API_POST
	{
		core::File file;
		const auto filePath = rootPath / metaData.value("fileRelativePath", "");
		file.open(filePath, core::FileOpenMode::Read);
		if (!file.isOpen())
		{
			Logger->error("Couldn't open file, path: {}", filePath.generic_string());
			return false;
		}

		shaderCode = file.readAll();
		file.close();

		loaded = true;
		return true;
	}

	void AssetShader::unload() ZINET_API_POST
	{ 
		shaderCode.clear();
		loaded = false; 
	}

	void AssetShader::imGuiAssetInspect() ZINET_API_POST
	{
		Asset::imGuiAssetInspect();
		if (!isLoaded())
			return;

		ImGui::Separator();
		ImGui::Text("Loaded text:");
		ImGui::Text(shaderCode.c_str());
	}

}