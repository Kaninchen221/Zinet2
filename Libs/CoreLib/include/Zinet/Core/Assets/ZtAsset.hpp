#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtPaths.hpp"

#include <filesystem>

#include <nlohmann/json.hpp>

namespace zt::core::assets
{
	class Asset;

	template<std::derived_from<Asset> AssetType = Asset>
	class AssetHandle
	{
	public:

		using AssetT = AssetType;

		AssetHandle() ZINET_API_POST = default;
		AssetHandle(AssetT* newAsset) : asset{ newAsset } {}
		AssetHandle(const AssetHandle& other) ZINET_API_POST = default;
		AssetHandle(AssetHandle&& other) ZINET_API_POST = default;
		~AssetHandle() ZINET_API_POST = default;

		AssetHandle& operator = (const AssetHandle& other) ZINET_API_POST = default;
		AssetHandle& operator = (AssetHandle&& other) ZINET_API_POST = default;

		bool isValid() const ZINET_API_POST { return asset; }

		operator bool() const ZINET_API_POST { return isValid(); }

		AssetT* operator->() const ZINET_API_POST { return asset; }

		AssetT* get() const ZINET_API_POST { return asset; }

	protected:

		AssetT* asset = nullptr;

	};

	ZT_REFLECT_CLASS()
	class ZINET_CORE_API Asset : public Object
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("Asset");

	public:

		using json = nlohmann::json;
		using Extensions = std::vector<std::string>;
		using AssetPtr = std::shared_ptr<Asset>;

		Asset() ZINET_API_POST = default;
		Asset(Extensions newExtensions) : extensions{newExtensions} {}
		Asset(const Asset& other) ZINET_API_POST = default;
		Asset(Asset&& other) ZINET_API_POST = default;
		~Asset() ZINET_API_POST = default;

		Asset& operator = (const Asset& other) ZINET_API_POST = default;
		Asset& operator = (Asset&& other) ZINET_API_POST = default;

		bool isLoaded() ZINET_API_POST { return loaded; }

		virtual AssetPtr createCopy() const ZINET_API_POST { return {}; }

		virtual bool load([[maybe_unused]] const Path& rootPath) ZINET_API_POST { return false; }

		virtual void unload() ZINET_API_POST {}

		const auto& getExtensions() const ZINET_API_POST { return extensions; }
		
		virtual void imGuiAssetInspect() ZINET_API_POST;

		json metaData;

	protected:

		bool loaded = false;

	private:

		// Config
		Extensions extensions = { "default_ext" };

	};

}