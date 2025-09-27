#pragma once

#include "Zinet/Math/ZtMath.hpp"

namespace zt
{
	struct MVP
	{
		Matrix model;
		Matrix view;
		Matrix projection;
	};
}