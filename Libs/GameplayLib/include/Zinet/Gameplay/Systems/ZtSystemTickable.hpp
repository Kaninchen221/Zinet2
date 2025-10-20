#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::gameplay::system
{
	class ZINET_GAMEPLAY_API Tickable
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::system::Tickable");


	};

}