#include "Zinet/Core/Assets/ZtAssetsFinder.hpp"

#include "Zinet/Core/ZtFileFinder.hpp"
#include "Zinet/Core/ZtFile.hpp"

#include <nlohmann/json.hpp>

#include <chrono>
#include <filesystem>

namespace zt::core::assets
{

	AssetsFinder::FindAssetsResult AssetsFinder::findAssets(const FindAssetsInput& findAssetsInput) const noexcept
	{
		if (!fs::exists(getContentFolderPath()))
		{
			Logger->error("Content folder doesn't exist");
			return {};
		}

		AssetsFinder::FindAssetsResult result;

		const auto foundFilesPaths = FileFinder::FindFiles(getContentFolderPath(), findAssetsInput.recursive);

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

	bool AssetsFinder::isAssetFile(const fs::path& path) const noexcept
	{
		return path.extension().string().ends_with(assetFileExtension);
	}

	fs::path AssetsFinder::createAssetFilePath(const fs::path& filePath) const noexcept
	{
		const fs::path result = filePath.string() + "." + assetFileExtension;
		return result;
	}

	fs::path AssetsFinder::createRelativePath(const std::string& folderAsRoot, const fs::path& path) const noexcept
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

	void AssetsFinder::createAssetFile(const fs::path& filePath, const fs::path& assetPath) const noexcept
	{
		auto assetFile = File::CreateFile(assetPath);
		const auto assetRelativePath = createRelativePath(contentFolderName, assetPath);
		const auto fileRelativePath = createRelativePath(contentFolderName, filePath);

		nlohmann::json data;
		data["hash"] = std::hash<fs::path>{}(assetRelativePath);
		data["assetRelativePath"] = assetRelativePath.string();
		data["fileRelativePath"] = fileRelativePath.string();
		data["fileName"] = fileRelativePath.filename().replace_extension("").string();

		auto extension = fileRelativePath.filename().extension().string();
		if (!extension.empty())
		{
			if (extension.front() == '.')
				extension.erase(extension.begin());
		}

		data["fileExtension"] = extension;
		assetFile.write(data.dump(1, '\t'));
	}

	Asset AssetsFinder::loadAsset(const fs::path& filePath, const fs::path& assetPath) const noexcept
	{
		Asset asset;

		File file;
		file.open(filePath, FileOpenMode::Read);
		if (!file.isOpen())
		{
			Logger->error("Couldn't open file, path: {}", filePath.string());
			return {};
		}

		asset.rawData = file.readData();
		file.close();

		file.open(assetPath, FileOpenMode::Read);
		if (!file.isOpen())
		{
			Logger->error("Couldn't open asset, path: {}", assetPath.string());
			return {};
		}

		using json = nlohmann::json;
		asset.metaData = json::parse(file.readAll());

		return asset;
	}

}