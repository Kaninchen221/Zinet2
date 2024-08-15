#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <filesystem>

namespace zt::core
{

	namespace Paths
	{
		ZINET_CORE_API std::filesystem::path RootPath();

		// It must be inline function
		inline std::filesystem::path CurrentProjectRootPath() { return ZINET_CURRENT_PROJECT_ROOT_PATH; }
	};

}