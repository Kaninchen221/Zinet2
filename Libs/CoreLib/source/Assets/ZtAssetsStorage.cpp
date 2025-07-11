#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

#include <ranges>

namespace zt::core::assets
{

	bool AssetsStorage::storeAssets() ZINET_API_POST
	{
		const AssetsFinder::FindAssetsInput input
		{
			.recursive = true,
			.reimport = false
		};

		bool result = true;
		const auto findAssetsResult = assetsFinder.findAssets(input);
		for (const auto& [filePath, assetPath] : std::views::zip(findAssetsResult.files, findAssetsResult.assets))
		{
			auto optionalAsset = assetsFinder.loadAsset(filePath, assetPath);
			if (!optionalAsset)
			{
				Logger->error("Couldn't load asset, file path: {}, asset path: {}", filePath.generic_string(), assetPath.generic_string());
			}
			auto asset = std::move(optionalAsset.value());

			auto keyValue = asset.metaData["fileRelativePath"].get<std::string>();
			auto [it, success] = assets.insert_or_assign(keyValue, std::move(asset));
			if (success)
			{
				Logger->info("Loaded asset: {}", keyValue);
			}
			else
			{
				Logger->error("Failed to load asset: {}, but continue", keyValue);
				result = false;
			}
		}

		return result;
	}

	AssetsStorage::GetResult AssetsStorage::get(const AssetsKey& key) ZINET_API_POST
	{
		auto findResult = assets.find(key);
		if (findResult == assets.end())
		{
			Logger->info("Couldn't find asset with key: {}", key);
			return nullptr;
		}

		return &findResult->second;
	}

}