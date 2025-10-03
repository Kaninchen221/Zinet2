#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include "Zinet/Core/ECS/ZtTypes.hpp"
#include "Zinet/Core/ECS/ZtTypeLessVector.hpp"

#include <algorithm>

namespace zt::core::ecs
{
	class ZINET_CORE_API Archetype
	{
		Archetype() noexcept = default;
		Archetype(const Archetype& other) noexcept = default;

		Archetype& operator = (const Archetype& other) noexcept = default;

		std::vector<TypeLessVector> componentsPack;

		std::vector<const std::type_info*> types;

		template<class Component>
		void addSingleComponent(const Component& component)
		{
			auto components = getComponentsOfType<Component>();
			components->add(component);
		}

	public:

		Archetype(Archetype&& other) noexcept = default;
		Archetype& operator = (Archetype&& other) noexcept = default;

		~Archetype() noexcept = default;

		template<class... Components>
		static Archetype Create();

		template<class... Components>
		void add(const Components&... components);

		template<class Component>
		TypeLessVector* getComponentsOfType() noexcept;

		template<class... Components>
		constexpr bool hasTypes() const noexcept;

	};

	template<class... Components>
	Archetype Archetype::Create()
	{
		Archetype archetype;
		(archetype.componentsPack.push_back(TypeLessVector::Create<Components>()), ...);

		(archetype.types.push_back(&typeid(Components)), ...);

		return archetype;
	}

	template<class... Components>
	void Archetype::add([[maybe_unused]] const Components&... components)
	{
		(addSingleComponent(components), ...);
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
		constexpr size_t ComponentsCount = sizeof...(Components);
		std::array<const std::type_info*, ComponentsCount> wantedTypes
		{
			&typeid(Components)...
		};

		for (auto wantedType : wantedTypes)
		{
			if (!std::ranges::contains(types, wantedType))
				return false;
		}

		return true;
	}
}