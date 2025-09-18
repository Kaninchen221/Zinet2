#pragma once

#include "Zinet/Math/ZtMath.hpp"

namespace zt
{
	struct MVP
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 projection;
	};
}