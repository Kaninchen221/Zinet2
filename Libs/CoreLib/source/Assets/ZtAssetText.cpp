#include "Zinet/Core/Assets/ZtAssetText.hpp"

#include "Zinet/Core/ZtImgui.hpp"

namespace zt::core
{
	bool AssetText::load(const Path& rootPath)
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

	void AssetText::unload()
	{ 
		text.clear(); 
		loaded = false; 
	}

	void AssetText::show()
	{
		Asset::show();
		if (!isLoaded())
			return;

		ImGui::Separator();
		ImGui::TextCStr("Loaded text:");
		ImGui::Text(text);
	}

}