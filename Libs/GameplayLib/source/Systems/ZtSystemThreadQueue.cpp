#include "Zinet/Gameplay/Systems/ZtSystemThreadQueue.hpp"

namespace zt::gameplay
{
	void SystemThreadQueue::update()
	{
		std::lock_guard<std::mutex> guard(mutex);

		System::update();

		for (auto& [objectHandle, logic] : events)
		{
			if (objectHandle)
				std::invoke(logic, objectHandle);
		}

		events.clear();
	}
}