#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtEntity.hpp"
#include "Zinet/Core/ECS/ZtTypes.hpp"
#include "Zinet/Core/ECS/ZtArchetype.hpp"
#include "Zinet/Core/ZtDebug.hpp"

#include <utility>

namespace zt::core::ecs
{
	class ZINET_CORE_API World
	{
		inline static auto Logger = ConsoleLogger::Create("zt::core::ecs::World");

	public:

		World() noexcept = default;
		World(const World& other) noexcept = default;
		World(World&& other) noexcept = default;
		~World() noexcept = default;

		World& operator = (const World& other) noexcept = default;
		World& operator = (World&& other) noexcept = default;

		template<class... Components>
		Entity spawn(Components&&... components);

		template<class Component>
		Component* getComponent(const Entity& entity);

		size_t getArchetypesCount() const noexcept { return archetypes.size(); }

	private:

		template<class... Components>
		size_t addComponents(const Entity& entity, Components&&... components);

		ID lastID = InvalidID;

		std::vector<Archetype> archetypes;

	};

	template<class... Components>
	Entity World::spawn(Components&&... components)
	{
		++lastID;

		Entity entity{ lastID, 0 };
		
		auto componentsIndex = addComponents(entity, std::forward<Components>(components)...);
		entity = Entity{ lastID, componentsIndex };

		return entity;
	}

	template<class Component>
	Component* World::getComponent(const Entity& entity)
	{
		for (auto& archetype : archetypes)
		{
			if (!archetype.hasTypes<Component>())
				continue;

			if (!archetype.hasEntity(entity))
				continue;

			return archetype.getComponentOfType<Component>(entity.getComponentsIndex());
		}

		return {};
	}

	template<class... Components>
	size_t World::addComponents(const Entity& entity, Components&&... components)
	{
		for (auto& archetype : archetypes)
		{
			if (archetype.hasTypes<Components...>())
				return archetype.add(entity, components...);
		}

		auto& archetype = archetypes.emplace_back(Archetype::Create<Components...>());
		return archetype.add(entity, components...);
	}
}