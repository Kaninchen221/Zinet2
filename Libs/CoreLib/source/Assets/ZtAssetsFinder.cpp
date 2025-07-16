#include "Zinet/Core/Assets/ZtAssetsFinder.hpp"

#include "Zinet/Core/ZtFileFinder.hpp"
#include "Zinet/Core/ZtFile.hpp"

#include <nlohmann/json.hpp>

#include <chrono>
#include <filesystem>

namespace zt::core::assets
{

	AssetsFinder::FindAssetsResult AssetsFinder::findAssets(const FindAssetsInput& findAssetsInput) const ZINET_API_POST
	{
		const auto contentFolderPath = getContentFolderPath();
		Logger->info("Content folder path: {}", contentFolderPath.generic_string());

		if (!fs::exists(contentFolderPath))
		{
			Logger->error("Content folder doesn't exist");
			return {};
		}

		AssetsFinder::FindAssetsResult result;

		const auto foundFilesPaths = FileFinder::FindFiles(contentFolderPath, findAssetsInput.recursive);

		result.files.reserve(foundFilesPaths.size());
		result.assets.reserve(foundFilesPaths.size());
		for (const auto& filePath : foundFilesPaths)
		{
			if (isAssetFile(filePath))
				continue;

			result.files.push_back(filePath);

			const auto assetFilePath = createAssetFilePath(filePath);
			result.assets.push_back(assetFilePath);

			if (fs::exists(assetFilePath))
			{
				if (findAssetsInput.reimport)
				{
					File::RemoveFile(assetFilePath);
					createAssetFile(filePath, assetFilePath);
				}
			}
			else
			{
				createAssetFile(filePath, assetFilePath);
			}
		}

		return result;
	}

	bool AssetsFinder::isAssetFile(const fs::path& path) const ZINET_API_POST
	{
		return path.extension().generic_string().ends_with(assetFileExtension);
	}

	fs::path AssetsFinder::createAssetFilePath(const fs::path& filePath) const ZINET_API_POST
	{
		const fs::path result = filePath.generic_string() + "." + assetFileExtension;
		return result;
	}

	fs::path AssetsFinder::createRelativePath(const std::string& folderAsRoot, const fs::path& path) const ZINET_API_POST
	{
		std::string result = path.relative_path().generic_string();
		while (!result.starts_with(folderAsRoot))
		{
			if (result.empty())
			{
				Logger->error("Can't create relative path with {} as a root folder, path: {}", folderAsRoot, path.generic_string());
				return {};
			}

			result.erase(0, 1);
		}

		return result;
	}

	void AssetsFinder::createAssetFile(const fs::path& filePath, const fs::path& assetPath) const ZINET_API_POST
	{
		auto assetFile = File::CreateFile(assetPath);
		const auto assetRelativePath = createRelativePath(contentFolderName, assetPath);
		const auto fileRelativePath = createRelativePath(contentFolderName, filePath);

		nlohmann::json data;
		data["hash"] = std::hash<fs::path>{}(assetRelativePath);
		data["assetRelativePath"] = assetRelativePath.generic_string();
		data["fileRelativePath"] = fileRelativePath.generic_string();
		data["fileName"] = fileRelativePath.filename().replace_extension("").generic_string();

		auto extension = fileRelativePath.filename().extension().generic_string();
		if (!extension.empty())
		{
			if (extension.front() == '.')
				extension.erase(extension.begin());
		}

		data["fileExtension"] = extension;
		assetFile.write(data.dump(1, '\t'));
	}

}