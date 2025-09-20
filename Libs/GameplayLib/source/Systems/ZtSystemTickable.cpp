#include "Zinet/Gameplay/Systems/ZtSystemTickable.hpp"

namespace zt::gameplay
{
	void SystemTickable::onCreate(ObjectWeakHandle<Object> newSelf)
	{
		System::onCreate(newSelf);

		clock.start();
	}

	void SystemTickable::update()
	{
		System::update();

		const auto elapsedTimeAsSeconds = clock.restart().getAsSeconds();
		for (auto& node : nodes)
		{
			if (node.isValid())
			{
				node->update(elapsedTimeAsSeconds);
			}
		}
	}

}