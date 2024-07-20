#include "Zinet/Core/Assets/ZtAssetsFinder.hpp"

#include "Zinet/Core/ZtFileFinder.hpp"
#include "Zinet/Core/ZtFile.hpp"

#include <nlohmann/json.hpp>

#include <chrono>

namespace zt::core::assets
{

	AssetsFinder::FindAssetsResult AssetsFinder::findAssets(const FindAssetsInput& findAssetsInput) const
	{
		if (!contentFolderExists())
		{
			Logger->error("Content folder doesn't exist");
			return {};
		}

		AssetsFinder::FindAssetsResult result;

		const bool recursive = true;
		auto foundFilesPaths = FileFinder::FindFiles(getContentFolderPath(), recursive);

		result.filesPaths.reserve(foundFilesPaths.size());
		result.assetsFilesPaths.reserve(foundFilesPaths.size());
		for (const auto& filePath : foundFilesPaths)
		{
			if (isAssetFilePath(filePath))
				continue;

			result.filesPaths.push_back(filePath);

			const auto assetFilePath = createAssetFilePath(filePath);
			result.assetsFilesPaths.push_back(assetFilePath);

			if (std::filesystem::exists(assetFilePath))
			{
				if (findAssetsInput.reimport)
				{
					File::RemoveFile(assetFilePath);
					createAssetFile(assetFilePath);
				}
			}
			else
			{
				createAssetFile(assetFilePath);
			}
		}

		return result;
	}

	bool AssetsFinder::isAssetFilePath(const std::filesystem::path& path) const
	{
		return path.extension().string().ends_with(assetExtension);
	}

	std::filesystem::path AssetsFinder::createAssetFilePath(const std::filesystem::path& filePath) const
	{
		const std::filesystem::path result = filePath.string() + "." + assetExtension;
		return result;
	}

	std::filesystem::path AssetsFinder::createRelativePath(const std::string& folderAsRoot, const std::filesystem::path& path) const
	{
		std::string result = path.relative_path().string();
		while (!result.starts_with(folderAsRoot))
		{
			if (result.empty())
			{
				Logger->error("Can't create relative path with {} as a root folder, path: {}", folderAsRoot, path.string());
				return {};
			}

			result.erase(0, 1);
		}

		return result;
	}

	void AssetsFinder::createAssetFile(const std::filesystem::path& assetFilePath) const
	{
		auto assetFile = File::CreateFile(assetFilePath);
		const auto relativePath = createRelativePath(contentFolderName, assetFilePath);

		nlohmann::json data;
		data["hash"] = std::hash<std::filesystem::path>{}(relativePath);
		data["relativePath"] = relativePath.string();
		data["fileName"] = assetFilePath.filename().replace_extension("").string();
		assetFile.write(data.dump(1, '\t'));
	}

	AssetsFinder::CreateAssetsResult AssetsFinder::createAssets(const FindAssetsResult& findAssetsResult) const
	{
		CreateAssetsResult result;

		result.assets.reserve(findAssetsResult.assetsFilesPaths.size());
		for ([[maybe_unused]] const auto& assetFilePath : findAssetsResult.assetsFilesPaths)
		{
			result.assets.push_back({});
		}

		return result;
	}

}