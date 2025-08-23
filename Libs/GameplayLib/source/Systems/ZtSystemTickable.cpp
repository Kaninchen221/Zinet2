#include "Zinet/Gameplay/Systems/ZtSystemTickable.hpp"

namespace zt::gameplay
{
	void SystemTickable::update()
	{
		System::update();

		for (auto& node : nodes)
		{
			if (node.isValid())
			{
				// TODO: Delta time
				node->update(1 /* Delta placeholder */);
			}
		}
	}

}