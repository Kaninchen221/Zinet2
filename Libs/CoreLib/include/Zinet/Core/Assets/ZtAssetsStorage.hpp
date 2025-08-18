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
	template<std::derived_from<Asset> AssetT>
	using AssetHandle = ObjectHandle<AssetT, true>;

	class AssetsStorage : public Object
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("AssetsStorage");

	public:

		using AssetsKey = std::string;
		using Assets = std::map<AssetsKey, ObjectRefCounter>;
		using AssetHandlers = std::vector<ObjectHandle<Asset>>;
		using LoadMinimalAssetResult = std::optional<Asset>;

		AssetsStorage() = default;
		AssetsStorage(const AssetsStorage& other) = default;
		AssetsStorage(AssetsStorage&& other) noexcept = default;
		~AssetsStorage() noexcept = default;

		AssetsStorage& operator = (const AssetsStorage& other) = default;
		AssetsStorage& operator = (AssetsStorage&& other) noexcept = default;

		bool storeAssets();

		void unloadAssets();

		template<std::derived_from<Asset> AssetT>
		void registerAssetClass();

		ObjectHandle<Asset> get(const AssetsKey& key);

		template<std::derived_from<Asset> AssetT>
		ObjectHandle<AssetT> getAs(const AssetsKey& key);

		AssetHandlers getAssets() { return assetHandlers; }

		LoadMinimalAssetResult loadAssetMetaData(const fs::path& assetPath) const;

		auto& getAssetsFinder() const noexcept { return assetsFinder; }

	protected:

		Assets assets;
		AssetHandlers assetHandlers;
		AssetsFinder assetsFinder;
		ClassRegistry<Asset> classRegistry;

	};

	template<std::derived_from<Asset> AssetT>
	void AssetsStorage::registerAssetClass()
	{
		classRegistry.registerClass<AssetT>();
	}

	template<std::derived_from<Asset> AssetT>
	ObjectHandle<AssetT> AssetsStorage::getAs(const AssetsKey& key)
	{
		auto assetHandle = get(key);
		if (!assetHandle)
			ObjectHandle<AssetT>{ nullptr };

		auto result = ObjectHandle<AssetT>(assetHandle);
		return result;
	}
}