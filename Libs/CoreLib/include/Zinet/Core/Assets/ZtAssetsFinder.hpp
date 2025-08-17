#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include "Zinet/Core/Assets/ZtAsset.hpp"

#include <filesystem>

namespace zt::core
{
	namespace fs = std::filesystem;

	class AssetsFinder : public Object
	{
	protected:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("AssetsFinder");

	public:		
		
		AssetsFinder() = default;
		AssetsFinder(const AssetsFinder& other) = default;
		AssetsFinder(AssetsFinder&& other) noexcept = default;
		~AssetsFinder() noexcept = default;

		AssetsFinder& operator = (const AssetsFinder& other) = default;
		AssetsFinder& operator = (AssetsFinder&& other) noexcept = default;

		std::filesystem::path getContentFolderPath() const { return rootFolder / contentFolderName; }

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

		FindAssetsResult findAssets(const FindAssetsInput& findAssetsInput) const;

		bool isAssetFile(const std::filesystem::path& path) const;

		std::filesystem::path createAssetFilePath(const std::filesystem::path& filePath) const;

		std::filesystem::path createRelativePath(const std::string& folderAsRoot, const std::filesystem::path& path) const;

		void createAssetFile(const std::filesystem::path& filePath, const std::filesystem::path& assetPath) const;

		void setRootPath(const Path& path) { rootFolder = path; }

		const auto& getRootPath() const noexcept { return rootFolder; }
		const auto& getContentFolderName() const noexcept { return contentFolderName; }
		const auto& getAssetFileExtension() const noexcept { return assetFileExtension; }

	protected:

		fs::path rootFolder = Paths::RootPath();
		std::string contentFolderName = "Content";
		std::string assetFileExtension = "asset";

	};

}