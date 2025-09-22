#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"
#include "Zinet/Core/ZtFile.hpp"
#include "Zinet/Core/ZtObjectRefCounter.hpp"

#include <ranges>
#include <algorithm>

namespace zt::core
{
	bool AssetsStorage::storeAssets()
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

			const auto extensionValue = minimalAsset.getMetaData().value("fileExtension", "");
			for (const auto& [name, cdo] : classRegistry.getCDOs())
			{
				if (!cdo)
					continue;

				if (!std::ranges::contains(cdo->getExtensions(), extensionValue))
					continue;

				auto assetClassCopy = dynamic_cast<Asset*>(cdo->createCopy().release());
				if (!assetClassCopy)
				{
					result = false;
					Logger->warn("createCopy from asset returned invalid asset but continue");
					continue;
				}
				assetClassCopy->setMetaData(minimalAsset.getMetaData());

				const auto keyValue = assetClassCopy->getMetaData().value("fileRelativePath", "");
				auto [it, success] = assets.insert_or_assign(keyValue, ObjectRefCounter{ assetClassCopy });
				if (success)
				{
					Logger->info("Loaded asset meta data: {}", keyValue);

					auto& objRefCounter = it->second;
					if (!objRefCounter.isValid())
					{
						Logger->error("ObjectRefCounter is invalid for asset with key: {}, but continue", keyValue);
						result = false;
						continue;
					}
					assetHandlers.emplace_back(&objRefCounter);

					auto& asset = *dynamic_cast<Asset*>(objRefCounter.get());

					core::JsonArchive metaDataArchive{ &asset.getMetaData() };
					asset.deserialize(metaDataArchive);

					asset.setDisplayName(keyValue);
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

	void AssetsStorage::unloadAssets()
	{
		for (auto& assetHandle : assetHandlers)
		{
			if (!assetHandle)
				continue;

			auto& metaData = assetHandle->getMetaData();

			core::JsonArchive metaDataArchive{ &metaData };
			assetHandle->serialize(metaDataArchive);

			const auto rootPath = assetsFinder.getRootPath();
			const auto metaDataFilePath = rootPath / metaData["assetRelativePath"];
			File assetFile;
			assetFile.open(metaDataFilePath, FileOpenMode::Write);
			assetFile.write(metaData.dump(1, '\t'));
			if (!assetFile.isOkay())
			{
				Logger->error("Something goes wrong with asset file but continue");
			}
			assetFile.close();

			assetHandle->unload();
		}

		for (auto& [key, asset] : assets)
		{
			if (asset)
			{
				asset.reset();
			}
		}
	}

	ObjectHandle<Asset> AssetsStorage::get(const AssetsKey& key)
	{
		auto findResult = assets.find(key);
		if (findResult == assets.end())
		{
			Logger->info("Couldn't find asset with key: {}", key);
			return nullptr;
		}

		return &findResult->second;
	}

	AssetsStorage::LoadMinimalAssetResult AssetsStorage::loadAssetMetaData(const fs::path& assetPath) const
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
		asset.setMetaData(json::parse(file.readAll()));

		return asset;
	}

}