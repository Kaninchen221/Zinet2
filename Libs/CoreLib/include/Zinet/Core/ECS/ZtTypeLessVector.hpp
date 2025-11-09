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

		TypeLessVector(TypeLessVector&& other) noexcept
			: typeID(other.typeID), destructor(other.destructor), typeSize(other.typeSize), isTriviallyDestructible(other.isTriviallyDestructible)
		{ 
			*this = std::move(other); 
		};

		TypeLessVector& operator = (TypeLessVector&& other) noexcept
		{
			buffer = std::move(other.buffer);
			removedObjects = std::move(other.removedObjects);
			objectsCapacity = other.objectsCapacity;
			objectsCount = other.objectsCount;
			
			other.objectsCapacity = 0;
			other.objectsCount = 0;

			return *this;
		}

		~TypeLessVector() noexcept;

		template<class T>
		static TypeLessVector Create();

		// Return the index of added component
		template<class T>
		size_t add(T&& object);

		bool remove(size_t index);

		template<class T>
		T* get(size_t index);

		// TODO: Reduce duplications of const methods
		template<class T>
		const T* get(size_t index) const;

		bool isValidIndex(size_t index) const noexcept;

		size_t getFirstValidIndex() const noexcept;

		template<class T>
		bool hasType() const noexcept { return GetTypeID<std::decay_t<T>>() == typeID; }

		size_t getObjectsCount() const noexcept { return objectsCount; }

		size_t getObjectsCapacity() const noexcept { return objectsCapacity; }

		auto getTypeID() const noexcept { return typeID; }

	private:

		using Buffer = std::vector<std::byte>;

		TypeLessVector(const ID& typeID, Function<void, void*> destructor, size_t typeSize, bool isTriviallyDestructible)
			: typeID(typeID), destructor(destructor), typeSize(typeSize), isTriviallyDestructible(isTriviallyDestructible)
		{
		}

		template<class T>
		void reallocateElements();

		TypeLessVector() noexcept = delete;
		TypeLessVector(const TypeLessVector& other) noexcept = delete;

		TypeLessVector& operator = (const TypeLessVector& other) noexcept = delete;

		Buffer buffer;

		std::vector<size_t> removedObjects;

		size_t objectsCapacity = 0;
		size_t objectsCount = 0;

		// Type info
		const ID typeID;
		const size_t typeSize = 0;
		const bool isTriviallyDestructible = false;
		const Function<void, void*> destructor;
	};
}

namespace zt::core::ecs
{
	template<class T>
	TypeLessVector TypeLessVector::Create()
	{
		using Object = std::decay_t<T>;

		// Lambda that invokes destructor
		auto destructor = [](void* rawComponent) 
		{
			Object* component = reinterpret_cast<Object*>(rawComponent);
			std::destroy_at(component);
		};

		return TypeLessVector
		(
			GetTypeID<Object>(), destructor, sizeof(Object), std::is_trivially_constructible_v<Object>
		);
	}

	template<class T>
	size_t TypeLessVector::add(T&& object)
	{
		using Object = std::decay_t<T>;

#	if ZINET_SANITY_CHECK
		if (typeID != GetTypeID<Object>())
		{
			Ensure(false); // Tried to add component of different type
			return InvalidIndex;
		}
#	endif

		size_t byteIndex = InvalidIndex;
		size_t objectIndex = InvalidIndex;

		// Try to place a new component at a released index
		if (!removedObjects.empty())
		{
			objectIndex = removedObjects.back();
			removedObjects.pop_back();
			byteIndex = objectIndex * typeSize;
		}
		else // Place new component at the end
		{
			reallocateElements<Object>();

			byteIndex = typeSize * objectsCount;
			objectIndex = byteIndex / typeSize;
		}

		objectsCapacity = buffer.size() / typeSize;
		++objectsCount;

		Object& storedObject = reinterpret_cast<Object&>(buffer[byteIndex]);
		std::construct_at(&storedObject, std::move(object));

		return objectIndex;
	}

	template<class T>
	T* TypeLessVector::get(size_t index)
	{
		using Object = std::decay_t<T>;

		constexpr size_t objectSize = sizeof(Object);
		if (objectSize != typeSize)
			return {};

		if (index >= objectsCount + removedObjects.size())
			return {};

		if (GetTypeID<Object>() != typeID)
			return {};

		if (std::ranges::contains(removedObjects, index))
			return {};

		const size_t offset = index * typeSize;

#	if ZINET_SANITY_CHECK
		if (offset + typeSize > buffer.size())
		{
			Ensure(false); // Invalid offset
			return {};
		}
#	endif

		return reinterpret_cast<T*>(buffer.data() + offset);
	}

	template<class T>
	const T* TypeLessVector::get(size_t index) const
	{
		using Object = std::decay_t<T>;

		constexpr size_t objectSize = sizeof(Object);
		if (objectSize != typeSize)
			return {};

		if (index >= objectsCount + removedObjects.size())
			return {};

		if (GetTypeID<Object>() != typeID)
			return {};

		if (std::ranges::contains(removedObjects, index))
			return {};

		const size_t offset = index * typeSize;

#	if ZINET_SANITY_CHECK
		if (offset + typeSize > buffer.size())
		{
			Ensure(false); // Invalid offset
			return {};
		}
#	endif

		return reinterpret_cast<const T*>(buffer.data() + offset);
	}

	template<class T>
	void TypeLessVector::reallocateElements()
	{
		using Object = std::decay_t<T>;

		const size_t desiredSize = (objectsCount * typeSize) + typeSize;

		if (desiredSize > buffer.capacity())
		{
			const auto newSize = static_cast<size_t>(desiredSize * 1.5);
			Buffer newBuffer{ newSize, std::byte{} };

			// Destroy old objects and move the newBuffer to the buffer
			for (size_t objectIndex = 0; objectIndex < objectsCount + removedObjects.size(); ++objectIndex)
			{
				auto* element = get<T>(objectIndex);
				// Element index can point at removed element
				if (!element)
					continue;

				const size_t byteIndex = objectIndex * typeSize;

				auto newObject = reinterpret_cast<Object*>(&newBuffer[byteIndex]);
				auto oldObject = reinterpret_cast<Object*>(&buffer[byteIndex]);
				std::construct_at(newObject, std::move(*oldObject));

				if (!std::is_trivially_destructible_v<T>)
					destructor.invoke(oldObject);
			}

			// Move the new vector to the old vector
			buffer = std::move(newBuffer);
		}
	}
}