#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include "Zinet/Core/ECS/ZtEntity.hpp"
#include "Zinet/Core/ECS/ZtTypes.hpp"
#include "Zinet/Core/ZtDebug.hpp"

namespace zt::core::ecs
{
	class TypeLessVector
	{
		using Components = std::vector<std::byte>;

		TypeLessVector(const std::type_info& typeInfo)
			: typeInfo(typeInfo)
		{}

		TypeLessVector() noexcept = default;
		TypeLessVector(const TypeLessVector& other) noexcept = default;

		TypeLessVector& operator = (const TypeLessVector& other) noexcept = default;

		const std::type_info& typeInfo;
		Components components; // Of the same type

		std::vector<bool> markers; // Mark if the component under index N is valid

		size_t size = 0;

	public:

		TypeLessVector(TypeLessVector&& other) noexcept = default;
		TypeLessVector& operator = (TypeLessVector&& other) noexcept = default;

		~TypeLessVector() noexcept = default;

		template<class Component>
		static TypeLessVector Create();

		template<class Component>
		size_t add(const Component& component);

		void remove(size_t index);

		template<class Component>
		Component* get(size_t index);

		template<class Component>
		bool hasType() const noexcept { return typeid(Component) == typeInfo; }

		size_t getSize() const noexcept { return size; }

		auto& getTypeInfo() const noexcept { return typeInfo; }
	};

	template<class Component>
	TypeLessVector TypeLessVector::Create()
	{
		return TypeLessVector
		(
			typeid(Component)
		);
	}

	template<class Component>
	size_t TypeLessVector::add(const Component& component)
	{
#	if ZINET_SANITY_CHECK
		if (typeInfo != typeid(Component))
		{
			Ensure(false); // Tried to add component of different type
			return std::numeric_limits<size_t>::max();
		}
#	endif

		// Try to place a new component at a released index
		for (size_t markerIndex = 0; markerIndex < markers.size(); ++markerIndex)
		{
			auto marker = markers[markerIndex];
			if (!marker)
			{
				size_t index = markerIndex * sizeof(Component);
				std::memcpy(&components[index], &component, sizeof(Component));
				++size;
				markers[markerIndex] = true;
				return markerIndex;
			}

			++markerIndex;
		}

		// Place a new component at the end of the storage
		markers.push_back(true);

		size_t newSize = components.size() + sizeof(Component);
		components.resize(newSize);

		size_t index = components.size() - sizeof(Component);
		std::memcpy(&components[index], &component, sizeof(Component));

		++size;

		return markers.size() - 1;
	}

	inline void TypeLessVector::remove(size_t index)
	{
		if (index >= size)
			return;

		if (!markers[index])
			return;

		markers[index] = false;

		--size;
	}

	template<class Component>
	Component* TypeLessVector::get(size_t index)
	{
		if (index >= size)
			return {};

		if (typeid(Component) != typeInfo)
			return {};

		if (!markers[index])
			return {};

		const size_t classSize = sizeof(Component);
		const size_t offset = index * classSize;

		if (offset + classSize > components.size())
			return {};

		return reinterpret_cast<Component*>(components.data() + offset);
	}
}