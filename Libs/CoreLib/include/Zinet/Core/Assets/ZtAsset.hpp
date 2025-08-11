#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtArchive.hpp"

#include <filesystem>

#include <nlohmann/json.hpp>

namespace zt::core
{
	class Asset;

	template<std::derived_from<Asset> AssetType = Asset>
	class AssetHandle
	{
	public:

		using AssetT = AssetType;

		AssetHandle() noexcept = default;
		AssetHandle(AssetT* newAsset) : asset{ newAsset } {}
		AssetHandle(const AssetHandle& other) noexcept = default;
		AssetHandle(AssetHandle&& other) noexcept { asset = other.asset; other.invalidate(); };
		~AssetHandle() noexcept = default;

		AssetHandle& operator = (const AssetHandle& other) = default;
		AssetHandle& operator = (AssetHandle&& other) noexcept { asset = other.asset; other.invalidate(); return *this; };

		bool isValid() const noexcept { return asset; }

		operator bool() const noexcept { return isValid(); }

		AssetT* operator->() const noexcept { return asset; }

		AssetT* get() const noexcept { return asset; }

		void invalidate() noexcept { asset = nullptr; }

	protected:

		AssetT* asset = nullptr;

	};

	class Asset : public Object
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("Asset");

	public:

		using Extensions = std::vector<std::string>;

		ZINET_CORE_API Asset() = default;
		ZINET_CORE_API Asset(const Extensions& newExtensions) : extensions{ newExtensions } {}
		ZINET_CORE_API Asset(const Asset& other) = default;
		ZINET_CORE_API Asset(Asset&& other) noexcept = default;
		ZINET_CORE_API ~Asset() noexcept = default;

		ZINET_CORE_API Asset& operator = (const Asset& other) = default;
		ZINET_CORE_API Asset& operator = (Asset&& other) noexcept = default;

		ZINET_CORE_API std::string getDisplayName() { return metaData.value("fileNameExt", "fileNameExt_DefaultName"); }

		ZINET_CORE_API bool isLoaded() noexcept { return loaded; }

		ZINET_CORE_API virtual bool load([[maybe_unused]] const Path& rootPath) { return false; }

		ZINET_CORE_API virtual void unload() {}

		ZINET_CORE_API const auto& getExtensions() const noexcept { return extensions; }
		
		ZINET_CORE_API virtual void imGui();

		ZINET_CORE_API bool getAutoLoad() const noexcept { return autoLoad; }

		ZINET_CORE_API auto& getMetaData() noexcept { return metaData; }
		void setMetaData(auto&& newMetaData) { metaData = newMetaData; }

	protected:

		bool loaded = false;
		bool autoLoad = false;

		Json metaData;

	private:

		Extensions extensions = { "default_ext" };

	};

}