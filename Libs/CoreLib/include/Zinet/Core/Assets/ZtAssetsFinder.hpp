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

		void createAssetFile(const std::filesystem::path& filePath, const std::filesystem::path& path) const;

		ZT_REFLECT_MEMBER(ReadWrite)
		std::filesystem::path rootFolder = Paths::RootPath();

		ZT_REFLECT_MEMBER(ReadOnly)
		std::string contentFolderName = "Content";

		ZT_REFLECT_MEMBER(ReadOnly)
		std::string assetFileExtension = "asset";

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<AssetsFinder>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<AssetsFinder>(); *result = *this; return result; }
		
		AssetsFinder() = default;
		AssetsFinder(const AssetsFinder& other) = default;
		AssetsFinder(AssetsFinder&& other) = default;
		~AssetsFinder() noexcept = default;
		
		AssetsFinder& operator = (const AssetsFinder& other) = default;
		AssetsFinder& operator = (AssetsFinder&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "AssetsFinder"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
		const decltype(rootFolder)& getRootFolder() const { return rootFolder; }
		void setRootFolder(const decltype(rootFolder)& newValue) { rootFolder = newValue; }
		
		const decltype(contentFolderName)& getContentFolderName() const { return contentFolderName; }
		
		const decltype(assetFileExtension)& getAssetFileExtension() const { return assetFileExtension; }
		
/*GENERATED_CODE_END*/
	};

}