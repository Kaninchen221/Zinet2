#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <filesystem>

namespace zt::core
{
	using Path = std::filesystem::path;

	namespace Paths
	{
		Path RootPath();

		// It must be inline function because of ZINET_CURRENT_PROJECT_ROOT_PATH
		inline Path CurrentProjectRootPath() { return std::filesystem::absolute(ZINET_CURRENT_PROJECT_ROOT_PATH); }
	};

}