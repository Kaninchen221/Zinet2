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
	class AssetsStorage : public Object
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("AssetsStorage");

	public:

		using AssetsKey = std::string;
		using ObjectPtr = std::shared_ptr<Asset>;
		using Assets = std::map<AssetsKey, ObjectPtr>;
		using AssetHandlers = std::vector<AssetHandle<>>;
		using LoadMinimalAssetResult = std::optional<Asset>;

		ZINET_CORE_API AssetsStorage() = default;
		ZINET_CORE_API AssetsStorage(const AssetsStorage& other) = default;
		ZINET_CORE_API AssetsStorage(AssetsStorage&& other) noexcept = default;
		ZINET_CORE_API ~AssetsStorage() noexcept = default;

		ZINET_CORE_API AssetsStorage& operator = (const AssetsStorage& other) = default;
		ZINET_CORE_API AssetsStorage& operator = (AssetsStorage&& other) noexcept = default;

		ZINET_CORE_API bool storeAssets();

		ZINET_CORE_API void unloadAssets();

		template<std::derived_from<Asset> AssetT>
		void registerAssetClass();

		ZINET_CORE_API AssetHandle<Asset> get(const AssetsKey& key);

		template<std::derived_from<Asset> AssetT>
		AssetHandle<AssetT> getAs(const AssetsKey& key);

		ZINET_CORE_API AssetHandlers getAssets();

		ZINET_CORE_API LoadMinimalAssetResult loadAssetMetaData(const fs::path& assetPath) const;

		AssetsFinder assetsFinder;

	protected:

		Assets assets;
		
		ClassRegistry<Asset> classRegistry;

	};

	template<std::derived_from<Asset> AssetT>
	void AssetsStorage::registerAssetClass()
	{
		classRegistry.registerClass<AssetT>();
	}

	template<std::derived_from<Asset> AssetT>
	AssetHandle<AssetT> AssetsStorage::getAs(const AssetsKey& key)
	{
		auto assetHandle = get(key);
		if (!assetHandle)
			return nullptr;

		auto result = dynamic_cast<AssetT*>(assetHandle.get());
		return result;
	}
}