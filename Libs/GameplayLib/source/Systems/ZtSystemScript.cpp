#include "Zinet/Gameplay/Systems/ZtSystemScript.hpp"

namespace zt::gameplay
{
	void SystemScript::update()
	{
		System::update();

		for (auto& node : nodes)
		{
			if (node.isValid())
			{
				// Pass
			}
		}
	}

}