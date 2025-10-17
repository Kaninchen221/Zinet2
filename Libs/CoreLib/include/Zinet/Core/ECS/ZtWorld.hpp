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

		/// Entities & Components
		template<class... Components>
		Entity spawn(Components&&... components);

		bool remove(const Entity& entity);

		template<class Component>
		Component* getComponent(const Entity& entity);

		template<class Component>
		QueryTypes::ComponentsPack getComponentsOfType() noexcept;

		size_t getComponentsCount() const noexcept;

		size_t getArchetypesCount() const noexcept { return archetypes.size(); }

		size_t getEntitiesCount() const noexcept;

		/// Resources
		template<class ResourceT>
		std::decay_t<ResourceT>* addResource(ResourceT&& newResource);

		template<class Resource>
		Resource* getResource();

	private:

		/// Entities & Components
		template<class... Components>
		size_t addComponents(const Entity& entity, Components&&... components);

		ID lastID = InvalidID;

		std::vector<Archetype> archetypes;

		// Resources
		std::vector<TypeLessVector> resources;

	};

#pragma warning(push)
#pragma warning(disable: 4702) // unreachable code when we try to spawn an entity without components
	template<class... Components>
	Entity World::spawn(Components&&... components)
	{
		if constexpr (sizeof...(Components) == 0)
			return Entity{ InvalidID, InvalidIndex };
		++lastID;

		Entity entity{ lastID, 0 };
		
		auto componentsIndex = addComponents(entity, std::forward<Components>(components)...);
		entity = Entity{ lastID, componentsIndex };

		return entity;
	}
#pragma warning(pop)

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

	template<class Component>
	QueryTypes::ComponentsPack World::getComponentsOfType() noexcept
	{
		typename QueryTypes::ComponentsPack componentsPack;

		for (auto& archetype : archetypes)
		{
			auto components = archetype.getComponentsOfType<Component>();
			if (components)
				componentsPack.emplace_back(components);
		}

		return componentsPack;
	}

	template<class... Components>
	size_t World::addComponents(const Entity& entity, Components&&... components)
	{
		for (auto& archetype : archetypes)
		{
			if (archetype.typesEqual<Components...>())
				return archetype.add(entity, components...);
		}

		auto& archetype = archetypes.emplace_back(Archetype::Create<Components...>());
		return archetype.add(entity, components...);
	}

	template<class ResourceT>
	std::decay_t<ResourceT>* World::addResource(ResourceT&& newResource)
	{
		using Resource = std::decay_t<ResourceT>;

		for (auto& resource : resources)
		{
			if (resource.hasType<Resource>())
				return {};
		}

		auto& typeLessVector = resources.emplace_back(TypeLessVector::Create<Resource>());
		typeLessVector.add(newResource);

		return typeLessVector.get<Resource>(0);
	}

	template<class Resource>
	Resource* World::getResource()
	{
		for (auto& resource : resources)
		{
			if (resource.hasType<Resource>())
				return resource.get<Resource>(0);
		}

		return {};
	}
}