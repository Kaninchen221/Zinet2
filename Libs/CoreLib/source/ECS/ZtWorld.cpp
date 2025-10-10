#include "Zinet/Core/ECS/ZtWorld.hpp"

namespace zt::core::ecs
{

	bool World::remove(const Entity& entity)
	{
		for (auto& archetype : archetypes)
		{
			if (archetype.remove(entity))
				return true;
		}

		return false;
	}

	size_t World::getComponentsCount() const noexcept
	{
		size_t count = 0;
		for (const auto& archetype : archetypes)
		{
			count += archetype.getComponentsCount();
		}
		return count;
	}

	size_t World::getEntitiesCount() const noexcept
	{
		size_t count = 0;
		for (const auto& archetype : archetypes)
		{
			count += archetype.getEntitiesCount();
		}
		return count;
	}

}