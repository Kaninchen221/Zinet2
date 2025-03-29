#include "Zinet/GameplayLib/Systems/ZtTickableSystem.hpp"

#include <ranges>

namespace zt::gameplay_lib
{
	void TickableSystem::tick(float deltaTime)
	{
		for (const auto& node : nodes | std::views::reverse)
		{
			if (node.expired())
				continue;

			auto asShared = node.lock();
			asShared->tick(deltaTime);
		}
	}
}