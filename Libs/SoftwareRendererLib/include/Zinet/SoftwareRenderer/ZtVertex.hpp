#pragma once

#include <Zinet/Math/ZtVecTypes.hpp>

#include "Zinet/SoftwareRenderer/ZtTexel.hpp"

namespace zt::software_renderer
{
	struct Vertex
	{
		Vector3f position;
		Vector2f texCoords;
		Texel color;
	};
}