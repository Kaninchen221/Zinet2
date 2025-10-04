#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <cstdint>
#include <limits>

namespace zt::core::ecs
{
	constexpr inline static size_t InvalidIndex = std::numeric_limits<size_t>::max();

	using ID = size_t;
	constexpr inline static ID InvalidID = InvalidIndex;
}