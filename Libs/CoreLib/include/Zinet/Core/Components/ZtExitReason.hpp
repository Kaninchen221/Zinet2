#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <string>

namespace zt::core::components
{
	struct ZINET_CORE_API ExitReason
	{
		bool exit = false;
		std::string reason;
	};
}