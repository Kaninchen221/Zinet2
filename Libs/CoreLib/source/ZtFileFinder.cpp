#include "Zinet/Core/ZtFileFinder.hpp"

namespace zt::core
{

	std::vector<std::filesystem::directory_entry> FileFinder::FindFiles(const std::filesystem::path& pathToFolder, const bool recursive)
	{
		std::vector<std::filesystem::directory_entry> result;

		auto internal = [&result](auto directoryIterator)
		{
			for (std::filesystem::directory_entry directoryEntry : directoryIterator)
			{
				if (directoryEntry.is_regular_file())
				{
					result.push_back(directoryEntry);
				}
			}
		};

		if (!recursive)
			internal(std::filesystem::directory_iterator(pathToFolder));
		else
			internal(std::filesystem::recursive_directory_iterator(pathToFolder));
		
		return result;
	}

	std::vector<std::filesystem::directory_entry> FileFinder::FindFiles(const std::filesystem::path& pathToFolder, const std::filesystem::path& neededExtension, const bool recursive)
	{
		std::vector<std::filesystem::directory_entry> result;

		auto internal = [&result, &neededExtension](auto directoryIterator)
		{
			for (std::filesystem::directory_entry directoryEntry : directoryIterator)
			{
				if (directoryEntry.is_regular_file())
				{
					std::filesystem::path filePath = directoryEntry.path();
					std::filesystem::path fileExtension = filePath.extension();

					if (neededExtension == fileExtension)
					{
						result.push_back(directoryEntry);
					}
				}
			}
		};

		if (!recursive)
			internal(std::filesystem::directory_iterator(pathToFolder));
		else
			internal(std::filesystem::recursive_directory_iterator(pathToFolder));

		return result;
	}

}