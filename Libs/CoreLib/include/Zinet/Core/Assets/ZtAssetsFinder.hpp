#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include "Zinet/Core/Assets/ZtAsset.hpp"

#include <filesystem>

namespace zt::core::assets
{
	namespace fs = std::filesystem;

	class ZINET_CORE_API AssetsFinder : public Object
	{
	protected:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("AssetsFinder");

	public:		
		
		AssetsFinder() ZINET_API_POST : Object("AssetsFinder") {}
		AssetsFinder(const AssetsFinder& other) ZINET_API_POST = default;
		AssetsFinder(AssetsFinder&& other) ZINET_API_POST = default;
		~AssetsFinder() ZINET_API_POST = default;

		AssetsFinder& operator = (const AssetsFinder& other) ZINET_API_POST = default;
		AssetsFinder& operator = (AssetsFinder&& other) ZINET_API_POST = default;

		std::filesystem::path getContentFolderPath() const ZINET_API_POST { return rootFolder / contentFolderName; }

		struct FindAssetsInput
		{
			bool recursive = true;
			bool reimport = false;
		};

		struct FindAssetsResult 
		{
			std::vector<std::filesystem::path> files;
			std::vector<std::filesystem::path> assets;
		};

		FindAssetsResult findAssets(const FindAssetsInput& findAssetsInput) const ZINET_API_POST;

		bool isAssetFile(const std::filesystem::path& path) const ZINET_API_POST;

		std::filesystem::path createAssetFilePath(const std::filesystem::path& filePath) const ZINET_API_POST;

		std::filesystem::path createRelativePath(const std::string& folderAsRoot, const std::filesystem::path& path) const ZINET_API_POST;

		void createAssetFile(const std::filesystem::path& filePath, const std::filesystem::path& assetPath) const ZINET_API_POST;

		void setRootPath(const Path& path) ZINET_API_POST { rootFolder = path; }

		const auto& getRootPath() const ZINET_API_POST { return rootFolder; }
		const auto& getContentFolderName() const ZINET_API_POST { return contentFolderName; }
		const auto& getAssetFileExtension() const ZINET_API_POST { return assetFileExtension; }

	protected:

		fs::path rootFolder = Paths::RootPath();
		std::string contentFolderName = "Content";
		std::string assetFileExtension = "asset";

	};

}