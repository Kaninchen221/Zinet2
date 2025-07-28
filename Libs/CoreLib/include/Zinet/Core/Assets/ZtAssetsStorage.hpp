#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/Assets/ZtAssetsFinder.hpp"
#include "Zinet/Core/Assets/ZtAsset.hpp"

#include <unordered_map>
#include <tuple>

namespace zt::core::assets
{
	class ZINET_CORE_API AssetsStorage : public Object
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("AssetsStorage");

	public:

		AssetsStorage() ZINET_API_POST : Object("AssetsStorage") {}
		AssetsStorage(const AssetsStorage& other) ZINET_API_POST = default;
		AssetsStorage(AssetsStorage&& other) ZINET_API_POST = default;
		~AssetsStorage() ZINET_API_POST = default;

		AssetsStorage& operator = (const AssetsStorage& other) ZINET_API_POST = default;
		AssetsStorage& operator = (AssetsStorage&& other) ZINET_API_POST = default;

		bool storeAssets() ZINET_API_POST;

		void clear() ZINET_API_POST;

		using AssetsKey = std::string;
		using AssetPtr = std::shared_ptr<Asset>;
		using Assets = std::map<AssetsKey, AssetPtr>;

		template<std::derived_from<Asset> AssetT>
		void registerAssetClass() ZINET_API_POST;

		AssetHandle<Asset> get(const AssetsKey& key) ZINET_API_POST;

		template<std::derived_from<Asset> AssetT>
		AssetHandle<AssetT> getAs(const AssetsKey& key) ZINET_API_POST;

		using AssetHandlers = std::vector<AssetHandle<>>;
		AssetHandlers getAssets() ZINET_API_POST;

		using LoadMinimalAssetResult = std::optional<Asset>;
		LoadMinimalAssetResult loadAssetMetaData(const fs::path& assetPath) const ZINET_API_POST;

		AssetsFinder assetsFinder;

	protected:

		Assets assets;

		using AssetClasses = std::vector<std::shared_ptr<Asset>>;
		AssetClasses assetClasses;

	};

	template<std::derived_from<Asset> AssetT>
	void AssetsStorage::registerAssetClass() ZINET_API_POST
	{
		auto asset = std::make_unique<AssetT>();
		assetClasses.push_back(std::move(asset));
	}

	template<std::derived_from<Asset> AssetT>
	AssetHandle<AssetT> AssetsStorage::getAs(const AssetsKey& key) ZINET_API_POST
	{
		auto assetHandle = get(key);
		if (!assetHandle)
			return nullptr;

		auto result = dynamic_cast<AssetT*>(assetHandle.get());
		return result;
	}
}