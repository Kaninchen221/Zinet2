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

		bool isValid() const noexcept { return static_cast<bool>(asset); }

		operator bool() const noexcept { return isValid(); }

		AssetT* operator->() const noexcept { return asset; }

		AssetT* get() const noexcept { return asset; }

		AssetT& operator*() const noexcept { Ensure(asset); return *asset; }

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

		 Asset() = default;
		Asset(const Extensions& newExtensions) : extensions{ newExtensions } {}
		 Asset(const Asset& other) = default;
		 Asset(Asset&& other) noexcept = default;
		 ~Asset() noexcept = default;

		 Asset& operator = (const Asset& other) = default;
		 Asset& operator = (Asset&& other) noexcept = default;

		std::string getDisplayName() { return metaData.value("fileNameExt", "fileNameExt_DefaultName"); }

		bool isLoaded() noexcept { return loaded; }

		virtual bool load([[maybe_unused]] const Path& rootPath) { return false; }

		virtual void unload() {}

		const auto& getExtensions() const noexcept { return extensions; }
		
		 virtual void imGui();

		bool getAutoLoad() const noexcept { return autoLoad; }

		auto& getMetaData() noexcept { return metaData; }
		void setMetaData(auto&& newMetaData) { metaData = newMetaData; }

	protected:

		bool loaded = false;
		bool autoLoad = false;

		Json metaData;

	private:

		Extensions extensions = { "default_ext" };

	};

}