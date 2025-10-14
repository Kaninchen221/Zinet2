#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include "Zinet/Core/ECS/ZtEntity.hpp"
#include "Zinet/Core/ECS/ZtTypes.hpp"
#include "Zinet/Core/ZtDebug.hpp"

#include <algorithm>

namespace zt::core::ecs
{
	// TODO: Separate part of it to cpp file
	class TypeLessVector
	{
	public:

		TypeLessVector(TypeLessVector&& other) noexcept = default;
		TypeLessVector& operator = (TypeLessVector&& other) noexcept = default;

		~TypeLessVector() noexcept;

		template<class Component>
		static TypeLessVector Create();

		// Return the index of added component
		template<class Component>
		size_t add(Component&& component);

		bool remove(size_t index);

		template<class Component>
		Component* get(size_t index);

		bool isValidIndex(size_t index) const noexcept;

		size_t getFirstValidIndex() const noexcept;

		template<class Component>
		bool hasType() const noexcept { return GetTypeID<Component>() == typeID; }

		size_t getComponentsCount() const noexcept { return componentsCapacity - removedComponents.size(); }

		size_t getComponentsCapacity() const noexcept { return componentsCapacity; }

		auto getTypeID() const noexcept { return typeID; }

	private:

		using Components = std::vector<std::byte>;

		TypeLessVector(const ID& newTypeID, Function<void, void*> newDestructor, size_t newTypeSize, bool newIsTriviallyDestructible)
			: typeID(newTypeID), destructor(newDestructor), typeSize(newTypeSize), isTriviallyDestructible(newIsTriviallyDestructible)
		{
		}

		template<class Component>
		void reallocateElements();

		TypeLessVector() noexcept = default;
		TypeLessVector(const TypeLessVector& other) noexcept = default;

		TypeLessVector& operator = (const TypeLessVector& other) noexcept = default;

		const ID typeID;
		Components components; // Of the same type

		std::vector<size_t> removedComponents;

		size_t componentsCapacity = 0;

		// Type info
		size_t typeSize = 0;
		const bool isTriviallyDestructible = false;
		Function<void, void*> destructor;
	};
}

namespace zt::core::ecs
{
	template<class Component>
	TypeLessVector TypeLessVector::Create()
	{
		using T = std::decay_t<Component>;

		// Lambda that invokes destructor
		auto destructor = [](void* rawComponent) 
		{
			T* component = reinterpret_cast<T*>(rawComponent);
			std::destroy_at(component);
		};

		return TypeLessVector
		(
			GetTypeID<Component>(), destructor, sizeof(T), std::is_trivially_constructible_v<Component>
		);
	}

	template<class Component>
	size_t TypeLessVector::add(Component&& component)
	{
		using ComponentT = std::decay_t<Component>;

#	if ZINET_SANITY_CHECK
		if (typeID != GetTypeID<ComponentT>())
		{
			Ensure(false); // Tried to add component of different type
			return InvalidIndex;
		}
#	endif

		size_t byteIndex = InvalidIndex;
		size_t componentIndex = InvalidIndex;

		// Try to place a new component at a released index
		if (!removedComponents.empty())
		{
			componentIndex = removedComponents.back();
			removedComponents.pop_back();
			byteIndex = componentIndex * typeSize;
		}
		else // Place new component at the end
		{
			reallocateElements<ComponentT>();

			byteIndex = components.size() - typeSize;
			componentIndex = byteIndex / typeSize;
		}

		componentsCapacity = components.size() / typeSize;

		Component& storedComponent = reinterpret_cast<Component&>(components[byteIndex]);
		std::construct_at(&storedComponent, std::move(component));

		if (!std::is_trivially_destructible_v<ComponentT>)
			destructor.invoke(&component);

		return componentIndex;
	}

	template<class Component>
	Component* TypeLessVector::get(size_t index)
	{
		if (index >= componentsCapacity)
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

	template<class Component>
	void TypeLessVector::reallocateElements()
	{
		using ComponentT = std::decay_t<Component>;

		const size_t desiredSize = components.size() + typeSize;

		if (desiredSize > components.capacity())
		{
			const auto newSize = desiredSize;
			Components newVector{ newSize, std::byte{} };

			// Move the existing components to the newVector, destroy old components and move the newVector to the components
			for (size_t elementIndex = 0; elementIndex < componentsCapacity; ++elementIndex)
			{
				auto* element = get<ComponentT>(elementIndex);
				// Element index can point at removed element
				if (!element)
					continue;

				const size_t byteIndex = elementIndex * typeSize;

				auto newElement = reinterpret_cast<ComponentT*>(&newVector[byteIndex]);
				auto oldElement = reinterpret_cast<ComponentT*>(&components[byteIndex]);
				std::construct_at(newElement, std::move(*oldElement));

				if (!std::is_trivially_destructible_v<ComponentT>)
					destructor.invoke(oldElement);
			}

			// Move the new vector to the old vector
			components = std::move(newVector);
		}
	}
}