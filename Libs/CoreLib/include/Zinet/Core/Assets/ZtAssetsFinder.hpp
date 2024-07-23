#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include "Zinet/Core/Assets/ZtAsset.hpp"

#include <filesystem>

namespace zt::core::assets
{

	ZT_REFLECT_CLASS()
	class ZINET_CORE_API AssetsFinder : public Object
	{
	protected:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("AssetsFinder");

	public:

		std::filesystem::path getContentFolderPath() const { return rootFolder / contentFolderName; }

		bool contentFolderExists() const { return std::filesystem::exists(getContentFolderPath()); }

		struct FindAssetsInput
		{
			bool recursive = true;
			bool reimport = false;
		};

		struct FindAssetsResult 
		{
			std::vector<std::filesystem::path> filesPaths;
			std::vector<std::filesystem::path> assetsFilesPaths;
		};

		FindAssetsResult findAssets(const FindAssetsInput& findAssetsInput) const;

		bool isAssetFilePath(const std::filesystem::path& path) const;

		std::filesystem::path createAssetFilePath(const std::filesystem::path& filePath) const;

		std::filesystem::path createRelativePath(const std::string& folderAsRoot, const std::filesystem::path& path) const;

		struct CreateAssetsResult
		{
			std::vector<Asset> assets;
		};

		AssetsFinder::CreateAssetsResult createAssets(const FindAssetsResult& findAssetsResult) const;

	protected:

		void createAssetFile(const std::filesystem::path& path) const;

		ZT_REFLECT_MEMBER(ReadWrite)
		std::filesystem::path rootFolder = Paths::RootPath();

		ZT_REFLECT_MEMBER(ReadOnly)
		std::string contentFolderName = "Content";

		ZT_REFLECT_MEMBER(ReadOnly)
		std::string assetExtension = "asset";

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		
		AssetsFinder() = default;
		AssetsFinder(const AssetsFinder& other) = default;
		AssetsFinder(AssetsFinder&& other) = default;
		
		~AssetsFinder() noexcept = default;
		
		AssetsFinder& operator = (const AssetsFinder& other) = default;
		AssetsFinder& operator = (AssetsFinder&& other) = default;
		
		class ClassInfo : zt::core::ClassInfoBase
		{
		public:
		
			static std::string_view GetClassName() { return "AssetsFinder"; }
			constexpr static auto GetParentsClassInfo() { return std::vector{Object::ClassInfo{}}; }
		};
		
		const decltype(rootFolder)& getRootFolder() const { return rootFolder; }
		void setRootFolder(const decltype(rootFolder)& newValue) { rootFolder = newValue; }
		
		const decltype(contentFolderName)& getContentFolderName() const { return contentFolderName; }
		
		const decltype(assetExtension)& getAssetExtension() const { return assetExtension; }
		
/*GENERATED_CODE_END*/
	};

}