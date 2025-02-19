#include "Zinet/Core/Assets/ZtTextAsset.hpp"
#include "Zinet/Core/ZtFile.hpp"

namespace zt::core::assets
{

	bool TextAsset::loadContentUsingMetaData(const std::filesystem::path& contentRootFolder)
	{
		auto findIt = metaData.find("fileRelativePath");
		if (findIt == metaData.end())
		{
			Logger->error("Meta data doesn't have fileRelativePath");
			return false;
		}

		if (!findIt->is_string())
		{
			Logger->error("fileRelativePath isn't string");
			return false;
		}

		const auto path = contentRootFolder / *findIt;
		std::error_code error_code;
		if (!std::filesystem::exists(path, error_code))
		{
			Logger->error("Path to content file doesn't exist: {}", path.string());
			Logger->error("Error code: {} Error Message: {}", error_code.value(), error_code.message());

			return false;
		}

		File file;
		file.open(path, FileOpenMode::Read);
		if (!file.isOpen())
		{
			Logger->error("Can't open file: {}", path.string());
			return false;
		}

		text = file.readAll();

		return true;
	}

}