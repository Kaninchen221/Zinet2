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
		AssetHandle(AssetHandle&& other) ZINET_API_POST { asset = other.asset; other.invalidate(); };
		~AssetHandle() ZINET_API_POST = default;

		AssetHandle& operator = (const AssetHandle& other) ZINET_API_POST = default;
		AssetHandle& operator = (AssetHandle&& other) ZINET_API_POST { asset = other.asset; other.invalidate(); return *this; };

		bool isValid() const ZINET_API_POST { return asset; }

		operator bool() const ZINET_API_POST { return isValid(); }

		AssetT* operator->() const ZINET_API_POST { return asset; }

		AssetT* get() const ZINET_API_POST { return asset; }

		AssetT& operator*() ZINET_API_POST { return *asset; }

		void invalidate() ZINET_API_POST { asset = nullptr; }

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

		Asset() ZINET_API_POST : Object("Asset") {}
		Asset(const Extensions& newExtensions) : Object("Image"), extensions{newExtensions} {}
		Asset(const Asset& other) ZINET_API_POST = default;
		Asset(Asset&& other) ZINET_API_POST = default;
		~Asset() ZINET_API_POST = default;

		Asset& operator = (const Asset& other) ZINET_API_POST = default;
		Asset& operator = (Asset&& other) ZINET_API_POST = default;

		std::string getDisplayName() ZINET_API_POST { return metaData.value("fileNameExt", "fileNameExt_DefaultName"); }

		bool isLoaded() ZINET_API_POST { return loaded; }

		virtual AssetPtr createCopy() const ZINET_API_POST { return {}; }

		virtual bool load([[maybe_unused]] const Path& rootPath) ZINET_API_POST { return false; }

		virtual void unload() ZINET_API_POST {}

		const auto& getExtensions() const ZINET_API_POST { return extensions; }
		
		virtual void imGui() ZINET_API_POST;

		bool getAutoLoad() const ZINET_API_POST { return autoLoad; }

		json metaData;

	protected:

		bool loaded = false;
		bool autoLoad = false;

	private:

		// Config
		Extensions extensions = { "default_ext" };

	};

}