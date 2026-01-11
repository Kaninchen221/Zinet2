#include "Zinet/Core/ECS/ZtWorld.hpp"

namespace zt::core::ecs
{

	bool World::hasEntity(const Entity& entity) noexcept
	{
		for (auto& archetype : archetypes)
		{
			if (archetype.hasEntity(entity))
				return true;
		}

		return false;
	}

	bool World::remove(const Entity& entity)
	{
		for (auto& archetype : archetypes)
		{
			if (archetype.remove(entity))
				return true;
		}

		return false;
	}

	size_t World::getComponentCount() const noexcept
	{
		size_t count = 0;
		for (const auto& archetype : archetypes)
		{
			count += archetype.getComponentCount();
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

	bool World::hasResource(TypeID typeID) const
	{
		for (const auto& resource : resources)
		{
			if (resource.getTypeID() == typeID)
				return true;
		}

		return false;
	}

}