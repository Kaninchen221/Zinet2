#pragma once

#include <type_traits>

namespace zt::core
{
	// TODO: Remove this class
	constexpr int InvalidIndex = -1;

	struct Static
	{
		using Yes = std::true_type;
		using No = std::false_type;
	};

}