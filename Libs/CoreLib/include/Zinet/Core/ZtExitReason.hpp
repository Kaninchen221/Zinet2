#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <string>

namespace zt::core
{
	// TODO: More info 
	// - Add level of exit reason? Like: Info, warn, error, fatal?
	// - Add file, line of code?

	struct ZINET_CORE_API ExitReason
	{
		bool exit = false;
		std::string reason;
	};
}