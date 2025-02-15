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

		const auto foundFilesPaths = FileFinder::FindFiles(getContentFolderPath(), findAssetsInput.recursive);

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

	bool AssetsFinder::isAssetFilePath(const std::filesystem::path& path) const
	{
		return path.extension().string().ends_with(assetFileExtension);
	}

	std::filesystem::path AssetsFinder::createAssetFilePath(const std::filesystem::path& filePath) const
	{
		const std::filesystem::path result = filePath.string() + "." + assetFileExtension;
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

	void AssetsFinder::createAssetFile(const std::filesystem::path& filePath, const std::filesystem::path& assetFilePath) const
	{
		auto assetFile = File::CreateFile(assetFilePath);
		const auto assetRelativePath = createRelativePath(contentFolderName, assetFilePath);
		const auto fileRelativePath = createRelativePath(contentFolderName, filePath);

		nlohmann::json data;
		data["hash"] = std::hash<std::filesystem::path>{}(assetRelativePath);
		data["assetRelativePath"] = assetRelativePath.string();
		data["fileRelativePath"] = fileRelativePath.string();
		data["fileName"] = fileRelativePath.filename().replace_extension("").string();

		const auto extension = fileRelativePath.filename().extension();
		std::string extension_string = "";
		if (!extension.empty())
		{
			extension_string = extension.string();
			if (extension_string[0] == '.')
				extension_string.erase(extension_string.begin());
		}

		data["fileExtension"] = extension_string;
		assetFile.write(data.dump(1, '\t'));
	}

	AssetsFinder::CreateAssetsResult AssetsFinder::createAssets(const FindAssetsResult& findAssetsResult) const
	{
		CreateAssetsResult result;

		result.assets.reserve(findAssetsResult.assetsFilesPaths.size());
		for (const auto& assetFilePath : findAssetsResult.assetsFilesPaths)
		{
			File file;
			file.open(assetFilePath, FileOpenMode::Read);
			if (!file.isOpen())
			{
				Logger->error("Found asset file but couldn't open it: {}", assetFilePath.string());
				continue;
			}

			auto& asset = result.assets.emplace_back();
			nlohmann::json content = nlohmann::json::parse(file.readAll());
			asset.setMetaData(content);
		}

		return result;
	}

}