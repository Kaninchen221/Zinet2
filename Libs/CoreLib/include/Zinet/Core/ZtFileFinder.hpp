#pragma once

#include <filesystem>
#include <vector>

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"

namespace zt::core
{

	namespace FileFinder
	{
		ZINET_CORE_API std::vector<std::filesystem::directory_entry> FindFiles(const std::filesystem::path& pathToFolder, const bool recursive = false);

		ZINET_CORE_API std::vector<std::filesystem::directory_entry> FindFiles(const std::filesystem::path& pathToFolder, const std::filesystem::path& neededExtension, const bool recursive = false);
	};

}