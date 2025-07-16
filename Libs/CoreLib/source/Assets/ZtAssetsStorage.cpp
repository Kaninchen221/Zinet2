#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"
#include "Zinet/Core/ZtFile.hpp"

#include <ranges>
#include <algorithm>

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
			auto optionalAsset = loadAssetMetaData(assetPath);
			if (!optionalAsset)
			{
				Logger->error("Couldn't load asset, file path: {}, asset path: {}", filePath.generic_string(), assetPath.generic_string());
			}
			auto minimalAsset = std::move(optionalAsset.value());

			const auto extensionValue = minimalAsset.metaData.value("fileExtension", "");
			for (const auto& assetClass : assetClasses)
			{
				if (!assetClass)
					continue;

				if (assetClass->getExtension() != extensionValue)
					continue;

				auto asset = assetClass->createCopy();
				if (!asset)
				{
					result = false;
					Logger->warn("createCopy from asset returned invalid asset but continue");
					continue;
				}
				asset->metaData = std::move(minimalAsset.metaData);

				const auto keyValue = asset->metaData.value("fileRelativePath", "");
				auto [it, success] = assets.insert_or_assign(keyValue, std::move(asset));
				if (success)
				{
					Logger->info("Loaded minimal asset: {}", keyValue);
				}
				else
				{
					Logger->error("Failed to load mnimal asset: {}, but continue", keyValue);
					result = false;
					continue;
				}
			}
		}

		return result;
	}

	AssetHandle<Asset> AssetsStorage::get(const AssetsKey& key) ZINET_API_POST
	{
		auto findResult = assets.find(key);
		if (findResult == assets.end())
		{
			Logger->info("Couldn't find asset with key: {}", key);
			return nullptr;
		}

		return findResult->second;
	}

	AssetsStorage::LoadMinimalAssetResult AssetsStorage::loadAssetMetaData(const fs::path& assetPath) const ZINET_API_POST
	{
		File file;
		file.open(assetPath, FileOpenMode::Read);
		if (!file.isOpen())
		{
			Logger->error("Couldn't open asset, path: {}", assetPath.generic_string());
			return {};
		}

		Asset asset;
		using json = nlohmann::json;
		asset.metaData = json::parse(file.readAll());

		return asset;
	}
}