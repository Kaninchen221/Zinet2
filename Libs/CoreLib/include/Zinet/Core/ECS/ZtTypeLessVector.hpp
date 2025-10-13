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

		~TypeLessVector() noexcept;

		template<class Component>
		static TypeLessVector Create();

		// Return the index of added component
		template<class Component>
		size_t add(Component&& component);

		bool remove(size_t index);

		template<class Component>
		Component* get(size_t index);

		template<class Component>
		bool hasType() const noexcept { return GetTypeID<Component>() == typeID; }

		size_t getComponentsCount() const noexcept { return componentsCount - removedComponents.size(); }

		auto getTypeID() const noexcept { return typeID; }

	private:

		using Components = std::vector<std::byte>;

		TypeLessVector(const ID& newTypeID, Function<void, void*> newDestructor, size_t newComponentTypeSize)
			: typeID(newTypeID), destructor(newDestructor), componentTypeSize(newComponentTypeSize)
		{
		}

		TypeLessVector() noexcept = default;
		TypeLessVector(const TypeLessVector& other) noexcept = default;

		TypeLessVector& operator = (const TypeLessVector& other) noexcept = default;

		const ID typeID;
		Components components; // Of the same type

		std::vector<size_t> removedComponents;

		size_t componentsCount = 0;

		size_t componentTypeSize = 0;
		Function<void, void*> destructor;
	};

	inline TypeLessVector::~TypeLessVector() noexcept
	{
		for (size_t i = 0; i < componentsCount; ++i)
		{
			if (std::ranges::contains(removedComponents, i))
				continue;

			const size_t index = i * componentTypeSize;
			destructor.invoke(&components[index]);
		}
	}

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
			GetTypeID<Component>(), destructor, sizeof(T)
		);
	}

	template<class Component>
	size_t TypeLessVector::add(Component&& component)
	{
#	if ZINET_SANITY_CHECK
		if (typeID != GetTypeID<Component>())
		{
			Ensure(false); // Tried to add component of different type
			return InvalidID;
		}
#	endif

		components.reserve(1'000'000); // TODO: Remove that and fix the problem while zi_view

		constexpr size_t typeSize = sizeof(Component);

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
			const size_t newSize = components.size() + typeSize;
			components.resize(newSize);

			byteIndex = components.size() - typeSize;
			componentIndex = byteIndex / typeSize;

			++componentsCount;
		}

		Component& storedComponent = reinterpret_cast<Component&>(components[byteIndex]);
		std::construct_at(&storedComponent, std::move(component));

		return componentIndex;
	}

	inline bool TypeLessVector::remove(size_t index)
	{
		if (index >= componentsCount)
			return false;

		if (std::ranges::contains(removedComponents, index))
			return false;

		removedComponents.push_back(index);

		auto* address = &components[index * componentTypeSize];
		destructor.invoke(address);

		// Fill the range of buffer with zeros to avoid dangling data
		const std::vector<std::byte> zeroBuffer{ componentTypeSize, std::byte{} };
		std::memcpy(std::launder(address), zeroBuffer.data(), componentTypeSize);

		return true;
	}

	template<class Component>
	Component* TypeLessVector::get(size_t index)
	{
		if (index >= componentsCount)
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