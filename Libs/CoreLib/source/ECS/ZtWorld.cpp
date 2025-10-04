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

}