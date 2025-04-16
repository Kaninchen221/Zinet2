#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <filesystem>

namespace zt::core
{

	namespace Paths
	{
		ZINET_CORE_API std::filesystem::path RootPath();

		// It must be inline function because of ZINET_CURRENT_PROJECT_ROOT_PATH
		inline std::filesystem::path CurrentProjectRootPath() { return std::filesystem::absolute(ZINET_CURRENT_PROJECT_ROOT_PATH); }
	};

}