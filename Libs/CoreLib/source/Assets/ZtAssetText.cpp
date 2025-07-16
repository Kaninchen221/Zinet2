#include "Zinet/Core/Assets/ZtAssetText.hpp"

namespace zt::core::assets
{
	bool AssetText::load(const Path& rootPath) ZINET_API_POST
	{
		File file;
		const auto filePath = rootPath / metaData.value("fileRelativePath", "");
		file.open(filePath, FileOpenMode::Read);
		if (!file.isOpen())
		{
			Logger->error("Couldn't open file, path: {}", filePath.generic_string());
			return {};
		}

		auto rawData = file.readData();
		file.close();

		text = std::string{ rawData.begin(), rawData.end() };
		loaded = true;
		return true;
	}

	void AssetText::unload() ZINET_API_POST
	{ 
		text.clear(); loaded = false; 
	}
}