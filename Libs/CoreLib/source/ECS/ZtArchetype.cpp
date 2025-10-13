#include "Zinet/Core/ECS/ZtArchetype.hpp"

namespace zt::core::ecs
{

	bool Archetype::remove(const Entity& entity)
	{
		auto prediction = [&](const Entity& element) { return element.getID() == entity.getID(); };
		const auto it = std::ranges::find_if(entities, prediction);

		if (it != entities.end())
		{
			entities.erase(it);

			for (auto& components : componentsPack)
			{
				components.remove(entity.getComponentsIndex());
			}

			return true;
		}

		return false;
	}

	bool Archetype::hasEntity(const Entity& entity) const
	{
		for (const Entity& ourEntity : entities)
		{
			if (ourEntity.getID() == entity.getID())
				return true;
		}

		return false;
	}

	size_t Archetype::getComponentsCount() const noexcept
	{
		if (componentsPack.empty())
			return 0;

		return componentsPack.size() * componentsPack.front().getComponentsCount();
	}

}