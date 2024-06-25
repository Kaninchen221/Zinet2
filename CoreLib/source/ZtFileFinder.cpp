#include "Zinet/Core/ZtFileFinder.hpp"

namespace zt::core
{
	FileFinder::Path FileFinder::CurrentPath()
	{
		return std::filesystem::current_path();
	}

	std::vector<FileFinder::FileInfo> FileFinder::FindFiles(Path pathToFolder)
	{
		std::vector<FileInfo> files;

		std::filesystem::directory_iterator directoryIterator = std::filesystem::directory_iterator(pathToFolder);
		for (std::filesystem::directory_entry directoryEntry : directoryIterator)
		{
			if (directoryEntry.is_regular_file())
			{
				files.push_back(directoryEntry);
			}
		}

		return files;
	}

	std::vector<FileFinder::FileInfo> FileFinder::FindFiles(Path pathToFolder, Extension neededExtension)
	{
		std::vector<FileInfo> files;

		std::filesystem::directory_iterator directoryIterator = std::filesystem::directory_iterator(pathToFolder);
		for (std::filesystem::directory_entry directoryEntry : directoryIterator)
		{
			if (directoryEntry.is_regular_file())
			{
				Path filePath = directoryEntry.path();
				Extension fileExtension = filePath.extension();

				if (neededExtension == fileExtension)
				{
					files.push_back(directoryEntry);
				}
			}
		}

		return files;
	}

}