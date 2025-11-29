#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <string>

namespace zt::core
{
	struct ZINET_CORE_API ExitReason
	{
		enum Level
		{
			Info,
			Error,
			Critical
		};

		std::string reason;
		Level level = Error;
	};
}