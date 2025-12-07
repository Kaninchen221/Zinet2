#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtTypes.hpp"
#include "Zinet/Core/ZtDebug.hpp"

#include "Zinet/Core/ECS/ZtEntity.hpp"
#include "Zinet/Core/ECS/ZtArchetype.hpp"

#include <utility>

namespace zt::core::ecs
{
	class ZINET_CORE_API World
	{
		inline static auto Logger = ConsoleLogger::Create("zt::core::ecs::World");

	public:

		using Command = std::function<void(World&)>;
		using Commands = std::vector<Command>;

		World() noexcept = default;
		World(const World& other) noexcept = default;
		World(World&& other) noexcept = default;
		~World() noexcept = default;

		World& operator = (const World& other) noexcept = default;
		World& operator = (World&& other) noexcept = default;

		/// Entities & Components
		template<class... Components>
		Entity spawn(Components&&... components);

		bool hasEntity(const Entity& entity) noexcept;

		bool remove(const Entity& entity);

		template<class Component>
		Component* getComponent(const Entity& entity);

		template<class Component>
		std::vector<TypeLessVector*> getComponentsOfType() noexcept;

		size_t getComponentsCount() const noexcept;

		size_t getArchetypesCount() const noexcept { return archetypes.size(); }

		size_t getEntitiesCount() const noexcept;

		template<class... Components>
		auto getArchetypesWith(this auto& self);

		/// Resources
		// Resources are unique by type
		// User can't remove resources

		template<class ResourceT>
		auto* addResource(ResourceT&& newResource);

		template<class ResourceT>
		auto* getResource(this auto& self);

		/// Commands
		// Would be nice to completly omit mutexes
		void addCommands(const Commands& newCommands) 
		{ 
			std::lock_guard guard{ addCommandsMutex };

			commands.insert(
				commands.end(),
				std::make_move_iterator(newCommands.begin()),
				std::make_move_iterator(newCommands.end())
			);
		}

		void executeCommands()
		{
			for (auto& command : commands)
			{
				command(*this);
			}
		}

		void clearCommands() { commands.clear(); }

	private:

		/// Entities & Components
		template<class... Components>
		size_t addComponents(const Entity& entity, Components&&... components);

		ID lastID = InvalidID;

		std::vector<Archetype> archetypes;

		/// Resources
		std::vector<TypeLessVector> resources;

		/// Commands
		Commands commands;
		std::mutex addCommandsMutex;
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
	std::vector<TypeLessVector*> World::getComponentsOfType() noexcept
	{
		std::vector<TypeLessVector*> componentsPack;

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
				return archetype.add(entity, std::forward<Components>(components)...);
		}

		auto& archetype = archetypes.emplace_back(Archetype::Create<Components...>());
		return archetype.add(entity, std::forward<Components>(components)...);
	}

	template<class ResourceT>
	auto* World::addResource(ResourceT&& newResource)
	{
		using Resource = std::remove_cvref_t<ResourceT>;

		for (auto& resource : resources)
		{
			// Don't return a valid pointer if the resource already exists
			// Because functions should be explicit about what they are doing
			if (resource.hasType<Resource>())
				return static_cast<Resource*>(nullptr);
		}

		auto& typeLessVector = resources.emplace_back(TypeLessVector::Create<Resource>());
		typeLessVector.add(newResource);

		return typeLessVector.get<Resource>(0);
	}

	template<class ResourceT>
	auto* World::getResource(this auto& self)
	{
		using Resource = std::remove_cvref_t<ResourceT>;

		using ResultT = std::conditional_t<IsSelfConst<decltype(self)>(),
			const Resource*, Resource*>;

		for (auto& resource : self.resources)
		{
			if (resource.hasType<Resource>())
				return static_cast<ResultT>(resource.get<Resource>(0));
		}

		return static_cast<ResultT>(nullptr);
	}

	template<class... Components>
	auto World::getArchetypesWith(this auto& self)
	{
		using ResultT = std::conditional_t<IsSelfConst<decltype(self)>(),
			std::vector<const Archetype*>, std::vector<Archetype*>>;

		ResultT result;
		for (auto& archetype : self.archetypes)
		{
			if (archetype.hasTypes<Components...>())
				result.push_back(&archetype);
		}

		return result;
	}
}