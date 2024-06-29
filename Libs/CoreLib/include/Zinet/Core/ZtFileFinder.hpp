#pragma once

#include <filesystem>
#include <vector>

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"

namespace zt::core
{

	namespace FileFinder
	{
		using Path = std::filesystem::path;
		using FileInfo = std::filesystem::directory_entry;
		using FolderInfo = std::vector<FileInfo>;
		using Extension = std::filesystem::path;

		Path ZINET_CORE_API CurrentPath();

		inline Path CurrentProjectRootPath()
		{
			using MacroType = std::decay_t<decltype(ZINET_CURRENT_PROJECT_ROOT_PATH)>;
			using ValidType = const char*;
			static_assert(std::is_same_v<ValidType, MacroType>, "Macro must have valid type");
			return ZINET_CURRENT_PROJECT_ROOT_PATH;
		}

		FolderInfo ZINET_CORE_API FindFiles(Path pathToFolder);

		FolderInfo ZINET_CORE_API FindFiles(Path pathToFolder, Extension neededExtension);

	};

}