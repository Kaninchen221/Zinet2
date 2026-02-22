#pragma once

#include <Zinet/Math/ZtVecTypes.hpp>

#include "Zinet/SoftwareRenderer/ZtTexel.hpp"

namespace zt::software_renderer
{
	constexpr Vector2ui TwoKDimension = Vector2ui{ 2560, 1440 };

	constexpr Texel BlackColor{ 0, 0, 0, 255 };
	constexpr Texel WhiteColor{ 255, 255, 255, 255 };
}