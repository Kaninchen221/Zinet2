#pragma once

#include "Zinet/GameplayLib/ZtGameplayLibConfig.hpp"

#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"

namespace zt::gameplay_lib
{
	namespace sf = software_renderer;

	class ZINET_GAMEPLAY_LIB_API Tickable
	{
	public:

		virtual void tick(float deltaTime) = 0;
	};

}