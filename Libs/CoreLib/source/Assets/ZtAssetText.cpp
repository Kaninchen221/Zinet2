#include "Zinet/Core/Assets/ZtAssetText.hpp"

#include <imgui.h>

namespace zt::core
{
	bool AssetText::load(const Path& rootPath) ZINET_API_POST
	{
		File file;
		const auto filePath = rootPath / metaData.value("fileRelativePath", "");
		file.open(filePath, FileOpenMode::Read);
		if (!file.isOpen())
		{
			Logger->error("Couldn't open file, path: {}", filePath.generic_string());
			return false;
		}

		text = file.readAll();
		file.close();

		loaded = true;
		return true;
	}

	void AssetText::unload() ZINET_API_POST
	{ 
		text.clear(); 
		loaded = false; 
	}

	void AssetText::imGui() ZINET_API_POST
	{
		Asset::imGui();
		if (!isLoaded())
			return;

		ImGui::Separator();
		ImGui::Text("Loaded text:");
		ImGui::Text(text.c_str());
	}

}