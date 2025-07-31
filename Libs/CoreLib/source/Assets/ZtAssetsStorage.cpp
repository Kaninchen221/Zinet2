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
				Logger->error("Couldn't load asset meta data, file path: {}, asset path: {}, but continue", filePath.generic_string(), assetPath.generic_string());
				continue;
			}
			auto minimalAsset = std::move(optionalAsset.value());

			const auto extensionValue = minimalAsset.metaData.value("fileExtension", "");
			for (const auto& assetClass : assetClasses)
			{
				if (!assetClass)
					continue;

				if (!std::ranges::contains(assetClass->getExtensions(), extensionValue))
					continue;

				auto assetClassCopy = assetClass->createCopy();
				if (!assetClassCopy)
				{
					result = false;
					Logger->warn("createCopy from asset returned invalid asset but continue");
					continue;
				}
				assetClassCopy->metaData = std::move(minimalAsset.metaData);

				const auto keyValue = assetClassCopy->metaData.value("fileRelativePath", "");
				auto [it, success] = assets.insert_or_assign(keyValue, std::move(assetClassCopy));
				if (success)
				{
					Logger->info("Loaded asset meta data: {}", keyValue);

					auto& asset = *it->second;
					if (asset.getAutoLoad())
					{
						if (asset.load(assetsFinder.getRootPath()))
						{
							Logger->info("Succesfull auto load asset with key: {}", keyValue);
						}
						else
						{
							Logger->warn("Failed auto load asset with key: {}", keyValue);
							result = false;
							continue;
						}
					}
				}
				else
				{
					Logger->warn("Failed to load minimal asset: {}, but continue", keyValue);
					result = false;
					continue;
				}
			}
		}

		return result;
	}

	void AssetsStorage::clear() ZINET_API_POST
	{
		for (auto& asset : assets)
		{
			asset.second->unload();
		}
		assets.clear();
	}

	AssetHandle<Asset> AssetsStorage::get(const AssetsKey& key) ZINET_API_POST
	{
		auto findResult = assets.find(key);
		if (findResult == assets.end())
		{
			Logger->info("Couldn't find asset with key: {}", key);
			return nullptr;
		}

		return findResult->second.get();
	}

	AssetsStorage::AssetHandlers AssetsStorage::getAssets() ZINET_API_POST
	{
		AssetsStorage::AssetHandlers result;
		for (auto& asset : assets)
		{
			result.emplace_back(asset.second.get());
		}
		return result;
	}

	AssetsStorage::LoadMinimalAssetResult AssetsStorage::loadAssetMetaData(const fs::path& assetPath) const ZINET_API_POST
	{
		File file;
		file.open(assetPath, FileOpenMode::Read);
		if (!file.isOpen())
		{
			Logger->error("Couldn't open asset, path: {}", assetPath.generic_string());
			return std::nullopt;
		}

		Asset asset;
		using json = nlohmann::json;
		asset.metaData = json::parse(file.readAll());

		return asset;
	}

}