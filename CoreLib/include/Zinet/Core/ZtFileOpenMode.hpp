#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

namespace zt::core
{

	enum class ZINET_CORE_API FileOpenMode
	{
		App,
		Binary,
		In,
		Out,
		Truncate,
		Ate
	};

}