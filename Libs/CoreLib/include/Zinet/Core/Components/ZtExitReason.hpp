#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <string>

// Namespace is resource but folder is component eh...
// The problem is that components can be used as resources and resources can be used as components so putting them in named namespaces and folders as "components" or "resources" doesn't have sens
namespace zt::core
{
	// TODO: Add level of exit reason? Like: Info, warn, error, fatal?
	struct ZINET_CORE_API ExitReason
	{
		bool exit = false;
		std::string reason;
	};
}