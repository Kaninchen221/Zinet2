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
		
		AssetsFinder() noexcept = default;
		AssetsFinder(const AssetsFinder& other) noexcept = default;
		AssetsFinder(AssetsFinder&& other) noexcept = default;
		~AssetsFinder() noexcept = default;

		AssetsFinder& operator = (const AssetsFinder& other) noexcept = default;
		AssetsFinder& operator = (AssetsFinder&& other) noexcept = default;

		std::filesystem::path getContentFolderPath() const noexcept { return rootFolder / contentFolderName; }

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

		FindAssetsResult findAssets(const FindAssetsInput& findAssetsInput) const noexcept;

		bool isAssetFile(const std::filesystem::path& path) const noexcept;

		std::filesystem::path createAssetFilePath(const std::filesystem::path& filePath) const noexcept;

		std::filesystem::path createRelativePath(const std::string& folderAsRoot, const std::filesystem::path& path) const noexcept;

		void createAssetFile(const std::filesystem::path& filePath, const std::filesystem::path& assetPath) const noexcept;

		Asset loadAsset(const fs::path& filePath, const fs::path& assetPath) const noexcept;

		fs::path rootFolder = Paths::RootPath();
		std::string contentFolderName = "Content";
		std::string assetFileExtension = "asset";

	protected:

	};

}