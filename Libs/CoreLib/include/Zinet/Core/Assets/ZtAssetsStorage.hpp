#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtObjectRefCounter.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtClassRegistry.hpp"
#include "Zinet/Core/ZtUtils.hpp"

#include "Zinet/Core/Assets/ZtAssetsFinder.hpp"
#include "Zinet/Core/Assets/ZtAsset.hpp"

#include <unordered_map>
#include <tuple>

namespace zt::core
{
	template<std::derived_from<Asset> AssetT>
	using AssetHandle = ObjectHandle<AssetT, true>;

	template<std::derived_from<Asset> AssetT>
	using ConstAssetHandle = ObjectHandle<AssetT, true, const ObjectRefCounter>;

	// TODO: Rename class name to AssetStorage
	class AssetsStorage : public Object
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("zt::core::AssetsStorage");

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

		auto get(this auto& self, const AssetsKey& key);

		template<std::derived_from<Asset> AssetT>
		auto getAs(this auto& self, const AssetsKey& key);

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

	auto AssetsStorage::get(this auto& self, const AssetsKey& key)
	{
		using ResultT = std::conditional_t<IsSelfConst<decltype(self)>(),
			::zt::ObjectHandle<Asset, const ObjectRefCounter>, ::zt::ObjectHandle<Asset, ObjectRefCounter>>;

		auto findResult = self.assets.find(key);
		if (findResult == self.assets.end())
		{
			Logger->info("Couldn't find asset with key: {}", key);
			return ResultT{ static_cast<ObjectRefCounter*>(nullptr) };
		}

		return ResultT( &findResult->second );
	}

	template<std::derived_from<Asset> AssetT>
	auto AssetsStorage::getAs(this auto& self, const AssetsKey& key)
	{
		using ObjectRefCounterT = std::conditional_t<IsSelfConst<decltype(self)>(),
			const ObjectRefCounter, ObjectRefCounter>;

		using ResultT = ::zt::ObjectHandle<AssetT, ObjectRefCounterT>;

		auto assetHandle = self.get(key);
		if (!assetHandle)
			return ResultT( static_cast<ObjectRefCounterT*>(nullptr) );

		auto result = ResultT(assetHandle);
		return result;
	}
}