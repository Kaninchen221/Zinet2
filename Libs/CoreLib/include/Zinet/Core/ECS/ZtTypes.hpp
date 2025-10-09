#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtFunction.hpp"

#include <cstdint>
#include <limits>
#include <vector>

namespace zt::core::ecs
{
	constexpr inline static size_t InvalidIndex = std::numeric_limits<size_t>::max();

	using ID = size_t;
	constexpr inline static ID InvalidID = InvalidIndex;

	class TypeLessVector;

	namespace QueryTypes
	{
		using Segment = TypeLessVector;
		using ComponentsPack = std::vector<Segment*>;
	}

	using ThreadID = uint8_t;

	class World;
	using System = Function<void, World&>;
}