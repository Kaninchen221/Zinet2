#pragma once

#include <Zinet/Math/ZtVecTypes.hpp>

#include "Zinet/SoftwareRenderer/ZtTexel.hpp"

namespace zt::software_renderer
{
	constexpr auto TwoKDimension = Vector2ui{ 2560, 1440 };
	constexpr auto BigDimension = Vector2ui(10, 1'000'000);
	constexpr auto SmallDimension = Vector2ui(100, 100);

	constexpr Texel BlackColor{ 0, 0, 0, 255 };
	constexpr Texel WhiteColor{ 255, 255, 255, 255 };
}