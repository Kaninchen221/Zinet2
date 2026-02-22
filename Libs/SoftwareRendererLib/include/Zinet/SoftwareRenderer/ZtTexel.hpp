#pragma once

#include <Zinet/Math/ZtVecTypes.hpp>

namespace zt::software_renderer
{
	struct Texel
	{
		constexpr inline static int Channels = 4;

		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		bool operator == (const Texel& other) const noexcept;
	};

	inline bool Texel::operator==(const Texel& other) const noexcept
	{
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}
}