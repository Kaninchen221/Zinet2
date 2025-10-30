#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtFunction.hpp"
#include "Zinet/Core/ZtUtils.hpp"

#include "Zinet/Core/ECS/ZtSystemReturnState.hpp"

#include <cstdint>
#include <limits>
#include <vector>

namespace zt::core::ecs
{
	constexpr inline static size_t InvalidIndex = std::numeric_limits<size_t>::max();

	using ID = size_t;
	constexpr inline static ID InvalidID = InvalidIndex;

	using ThreadID = uint8_t;

	class World;
	// System can't access other systems because it will broke decoupling
	// But can access components and resources from the world
	using System = Function<SystemReturnState, World&>;
}