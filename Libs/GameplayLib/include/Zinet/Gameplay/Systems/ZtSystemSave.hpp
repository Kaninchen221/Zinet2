#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtArchive.hpp"
#include "Zinet/Core/ZtFile.hpp"

namespace zt::gameplay::system
{
	class ZINET_GAMEPLAY_API Save
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::system::Save");

	};

}