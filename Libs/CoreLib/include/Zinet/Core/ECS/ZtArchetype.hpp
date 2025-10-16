#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include "Zinet/Core/ECS/ZtEntity.hpp"
#include "Zinet/Core/ECS/ZtTypes.hpp"
#include "Zinet/Core/ECS/ZtTypeLessVector.hpp"

#include <algorithm>
#include <ranges>

namespace zt::core::ecs
{
	class ZINET_CORE_API Archetype
	{
	public:

		Archetype(Archetype&& other) noexcept = default;
		Archetype& operator = (Archetype&& other) noexcept = default;

		~Archetype() noexcept = default;

		template<class... Components>
		static Archetype Create();

		template<class... Components>
		size_t add(const Entity& entity, Components&&... components);

		bool remove(const Entity& entity);

		bool hasEntity(const Entity& entity) const;

		template<class Component>
		Component* getComponentOfType(size_t index) noexcept;

		template<class Component>
		TypeLessVector* getComponentsOfType() noexcept;

		template<class... Components>
		constexpr bool hasTypes() const noexcept;

		template<class... Components>
		constexpr bool typesEqual() const noexcept;

		size_t getEntitiesCount() const noexcept { return entities.size(); }

		size_t getComponentsCount() const noexcept;

	private:

		Archetype() noexcept = default;
		Archetype(const Archetype& other) noexcept = default;

		Archetype& operator = (const Archetype& other) noexcept = default;

		std::vector<TypeLessVector> componentsPack;

		std::vector<ID> types;

		std::vector<Entity> entities;

		template<class Component>
		size_t addSingleComponent(Component&& component)
		{
			auto components = getComponentsOfType<Component>();
			return components->add(component);
		}
	};

	template<class... Components>
	Archetype Archetype::Create()
	{
		Archetype archetype;
		(archetype.componentsPack.push_back(TypeLessVector::Create<Components>()), ...);

		(archetype.types.push_back(GetTypeID<Components>()), ...);

		return archetype;
	}

	template<class... Components>
	size_t Archetype::add(const Entity& entity, Components&&... components)
	{
		if (sizeof...(Components) != componentsPack.size())
			return InvalidIndex;

		size_t index = InvalidIndex;
		((index = addSingleComponent(components)), ...);

		if (index != InvalidIndex)
			entities.emplace_back(entity);

		return index;
	}

	template<class Component>
	Component* Archetype::getComponentOfType(size_t index) noexcept
	{
		auto components = getComponentsOfType<Component>();
		if (!components)
			return nullptr;

		if (index >= components->getObjectsCount())
			return nullptr;

		return components->get<Component>(index);
	}

	template<class Component>
	TypeLessVector* Archetype::getComponentsOfType() noexcept
	{
		for (auto& components : componentsPack)
		{
			if (components.hasType<Component>())
				return &components;
		}

		return nullptr;
	}

	template<class... Components>
	constexpr bool Archetype::hasTypes() const noexcept
	{
		const std::vector<ID> wantedTypes
		{
			GetTypeID<Components>()...
		};

		for (auto wantedType : wantedTypes)
		{
			if (!std::ranges::contains(types, wantedType))
				return false;
		}

		return true;
	}

	template<class... Components>
	constexpr bool Archetype::typesEqual() const noexcept
	{
		std::vector<ID> wantedTypes
		{
			GetTypeID<Components>()...
		};

		for (const auto& type : types)
		{
			auto it = std::ranges::find(wantedTypes, type);
			if (it == wantedTypes.end())
				return false;

			wantedTypes.erase(it);
		}

		return wantedTypes.empty();
	}
}