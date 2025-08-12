#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include <string_view>
#include <array>

namespace zt::gameplay
{
	struct ZINET_GAMEPLAY_API EditorSearchBar
	{
		std::string_view show();

		std::array<char, 1000> assetsListBuffer{ '\0' };
	};
}