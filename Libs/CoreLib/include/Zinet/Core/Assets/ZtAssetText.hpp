#pragma once

#include "Zinet/Core/Assets/ZtAsset.hpp"
#include "Zinet/Core/ZtFile.hpp"

namespace zt::core::assets
{
	struct AssetText : public Asset
	{
		AssetText() : Asset{ "txt" } {}

		AssetHandle<Asset> createCopy() const ZINET_API_POST override { return std::make_shared<AssetText>(*this); }

		bool load(const Path& rootPath) ZINET_API_POST override
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

		void unload() ZINET_API_POST override { text.clear(); loaded = false; }

		// Content
		std::string text;

	};

}