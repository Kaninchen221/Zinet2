#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtClassRegistry.hpp"

#include "Zinet/Core/Assets/ZtAssetsFinder.hpp"
#include "Zinet/Core/Assets/ZtAsset.hpp"

#include <unordered_map>
#include <tuple>

namespace zt::core
{
	class ZINET_CORE_API AssetsStorage : public Object
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("AssetsStorage");

	public:

		AssetsStorage() ZINET_API_POST = default;
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
		void registerAssetClass(const std::string& name) ZINET_API_POST;

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
		
		ClassRegistry<Asset> classRegistry;

	};

	template<std::derived_from<Asset> AssetT>
	void AssetsStorage::registerAssetClass(const std::string& name) ZINET_API_POST
	{
		classRegistry.registerClass<AssetT>({ name });
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