#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtTypes.hpp"
#include "Zinet/Core/ZtTypeLessVector.hpp"

#include "Zinet/Core/ECS/ZtEntity.hpp"

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
		auto* getComponentOfType(this auto& self, size_t index) noexcept;

		template<class Component>
		auto* getComponentsOfType(this auto& self) noexcept;

		template<class... Components>
		constexpr bool hasTypes() const noexcept;

		template<class... Components>
		constexpr bool typesEqual() const noexcept;

		size_t getEntitiesCount() const noexcept { return entities.size(); }

		size_t getComponentsCount() const noexcept;

		auto& getEntities() const noexcept { return entities; }

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
			return components->add(std::forward<Component>(component));
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
		((index = addSingleComponent(std::forward<Components>(components))), ...);

		if (index != InvalidIndex)
			entities.emplace_back(entity.getID(), index);

		return index;
	}

	template<class Component>
	auto* Archetype::getComponentOfType(this auto& self, size_t index) noexcept
	{
		using ReturnT = std::conditional_t<IsSelfConst<decltype(self)>(), const Component, Component>;

		auto components = self.getComponentsOfType<Component>();
		if (!components)
			return static_cast<ReturnT*>(nullptr);

		if (index >= components->getObjectsCount())
			return static_cast<ReturnT*>(nullptr);

		return components->get<ReturnT>(index);
	}

	template<class Component>
	auto* Archetype::getComponentsOfType(this auto& self) noexcept
	{
		using ReturnT = std::conditional_t<IsSelfConst<decltype(self)>(), const TypeLessVector, TypeLessVector>;

		for (auto& components : self.componentsPack)
		{
			if (components.hasType<Component>())
				return &components;
		}

		return static_cast<ReturnT*>(nullptr);
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