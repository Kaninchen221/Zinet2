#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include "Zinet/Core/ECS/ZtEntity.hpp"
#include "Zinet/Core/ECS/ZtTypes.hpp"
#include "Zinet/Core/ZtDebug.hpp"

#include <algorithm>

namespace zt::core::ecs
{
	class TypeLessVector
	{
	public:

		TypeLessVector(TypeLessVector&& other) noexcept = default;
		TypeLessVector& operator = (TypeLessVector&& other) noexcept = default;

		// TODO: Invoke destructor of the removed components 
		// Reason: To handle situations when they have complex types like vector
		~TypeLessVector() noexcept = default;

		template<class Component>
		static TypeLessVector Create();

		template<class Component>
		size_t add(Component&& component);

		void remove(size_t index);

		template<class Component>
		Component* get(size_t index);

		template<class Component>
		bool hasType() const noexcept { return GetTypeID<Component>() == typeID; }

		size_t getSize() const noexcept { return size; }

		auto getTypeID() const noexcept { return typeID; }

	private:

		using Components = std::vector<std::byte>;

		TypeLessVector(const ID& newTypeID)
			: typeID(newTypeID)
		{
		}

		TypeLessVector() noexcept = default;
		TypeLessVector(const TypeLessVector& other) noexcept = default;

		TypeLessVector& operator = (const TypeLessVector& other) noexcept = default;

		const ID typeID;
		Components components; // Of the same type

		std::vector<size_t> removedComponents;

		size_t size = 0;
	};

	template<class Component>
	TypeLessVector TypeLessVector::Create()
	{
		// There is a prototype of lambda that will invoke destructor of our type less types
		[[maybe_unused]] auto destructor = [](void* component) 
		{
			typedef std::decay_t<Component> T;
			((Component&)component).~T(); 
		};

		return TypeLessVector
		(
			GetTypeID<Component>()
		);
	}

	template<class Component>
	size_t TypeLessVector::add([[maybe_unused]] Component&& component)
	{
#	if ZINET_SANITY_CHECK
		if (typeID != GetTypeID<Component>())
		{
			Ensure(false); // Tried to add component of different type
			return InvalidID;
		}
#	endif

		// Try to place a new component at a released index
		if (!removedComponents.empty())
		{
			const size_t lastRemovedComponentIndex = removedComponents.back();
			removedComponents.pop_back();
			const size_t index = lastRemovedComponentIndex * sizeof(Component);

			Component& storedComponent = reinterpret_cast<Component&>(components[index]);
			storedComponent = std::move(component);

			++size;
			return lastRemovedComponentIndex;
		}

		const size_t newSize = components.size() + sizeof(Component);
		components.resize(newSize);

		const size_t index = components.size() - sizeof(Component);
		Component& storedComponent = reinterpret_cast<Component&>(components[index]);
		storedComponent = std::move(component);

		++size;

		return index / sizeof(Component);
	}

	inline void TypeLessVector::remove(size_t index)
	{
		if (index >= size)
			return;

		if (std::ranges::contains(removedComponents, index))
			return;

		removedComponents.push_back(index);

		// TODO: Invoke destructor of the removed component

		--size;
	}

	template<class Component>
	Component* TypeLessVector::get(size_t index)
	{
		if (index >= size)
			return {};

		if (GetTypeID<Component>() != typeID)
			return {};

		if (std::ranges::contains(removedComponents, index))
			return {};

		const size_t classSize = sizeof(Component);
		const size_t offset = index * classSize;

		if (offset + classSize > components.size())
			return {};

		return reinterpret_cast<Component*>(components.data() + offset);
	}
}