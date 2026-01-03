#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtTypes.hpp"
#include "Zinet/Core/ZtDebug.hpp"

#include "Zinet/Core/ECS/ZtEntity.hpp"

#include <algorithm>

namespace zt::core
{
	class TypeLessVector;

	class TypeLessVectorIterator
	{
	public:

		TypeLessVectorIterator(TypeLessVector* vector, size_t startingIndex = 0) noexcept
			: vector(vector), currentIndex(startingIndex)
		{
#	if ZINET_SANITY_CHECK
			if (!Ensure(vector))
				Terminate();
#	endif
		}

		TypeLessVectorIterator() noexcept = delete;
		TypeLessVectorIterator(const TypeLessVectorIterator& other) noexcept = default;
		TypeLessVectorIterator(TypeLessVectorIterator&& other) noexcept = default;

		TypeLessVectorIterator& operator = (const TypeLessVectorIterator& other) noexcept = default;
		TypeLessVectorIterator& operator = (TypeLessVectorIterator&& other) noexcept = default;

		~TypeLessVectorIterator() noexcept = default;

		bool operator == (const TypeLessVectorIterator& other) const noexcept;

		bool operator != (const TypeLessVectorIterator& other) const noexcept;

		TypeLessVectorIterator& operator ++ () noexcept;

		void* operator * () const noexcept;

	private:

		TypeLessVector* vector{};
		size_t currentIndex = 0;

	};

	class TypeLessVector
	{
	public:

		TypeLessVector() noexcept = delete;
		TypeLessVector(const TypeLessVector& other) noexcept = delete;
		TypeLessVector(TypeLessVector&& other) noexcept
			: typeID(other.typeID), destructor(other.destructor), typeSize(other.typeSize), isTriviallyDestructible(other.isTriviallyDestructible)
		{ 
			*this = std::move(other); 
		};

		TypeLessVector& operator = (const TypeLessVector& other) noexcept = delete;
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

		void clear();

		// Return the index of added component
		template<class T>
		size_t add(T&& object);

		bool remove(size_t index);

		template<class T>
		auto get(this auto& self, size_t index);

		// The returned pointer can point to removed element
		void* getPtr(size_t index) noexcept { return buffer.data() + (index * typeSize); }

		bool isValidIndex(size_t index) const noexcept;

		size_t getFirstValidIndex() const noexcept;

		// TODO: Test it
		size_t getLastIndex() const noexcept { return objectsCount + removedObjects.size() - 1; }

		template<class T>
		bool hasType() const noexcept { return GetTypeID<T>() == typeID; }

		size_t getObjectsCount() const noexcept { return objectsCount; }

		bool isEmpty() const noexcept { return getObjectsCount() == 0; }

		size_t getObjectsCapacity() const noexcept { return objectsCapacity; }

		auto getTypeID() const noexcept { return typeID; }

		auto getTypeSize() const noexcept { return typeSize; }

		TypeLessVectorIterator begin() noexcept;
		TypeLessVectorIterator end() noexcept;

		const void* data() const noexcept { return buffer.data(); }

	private:

		using Buffer = std::vector<std::byte>;

		TypeLessVector(const ID& typeID, Function<void, void*> destructor, size_t typeSize, bool isTriviallyDestructible)
			: typeID(typeID), destructor(destructor), typeSize(typeSize), isTriviallyDestructible(isTriviallyDestructible)
		{
		}

		template<class T>
		void reallocateElements();

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

namespace zt::core
{
	template<class T>
	TypeLessVector TypeLessVector::Create()
	{
		using ObjectT = std::decay_t<T>;

		// Lambda that invokes destructor
		auto destructor = [](void* objectVoidPtr) 
		{
			ObjectT* objectPtr = reinterpret_cast<ObjectT*>(objectVoidPtr);
			std::destroy_at(objectPtr);
		};

		return TypeLessVector
		(
			GetTypeID<ObjectT>(), destructor, sizeof(ObjectT), std::is_trivially_constructible_v<ObjectT>
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

		// Index for a new component at a released index
		if (!removedObjects.empty())
		{
			objectIndex = removedObjects.back();
			removedObjects.pop_back();
			byteIndex = objectIndex * typeSize;
		}
		else // Index for a new component at the end
		{
			reallocateElements<Object>();

			byteIndex = typeSize * objectsCount;
			objectIndex = byteIndex / typeSize;
		}

		objectsCapacity = buffer.size() / typeSize;
		++objectsCount;

		Object& storedObject = reinterpret_cast<Object&>(buffer[byteIndex]);
		std::construct_at(&storedObject, std::forward<Object>(object));

		return objectIndex;
	}

	template<class T>
	auto TypeLessVector::get(this auto& self, size_t index)
	{
		using ReturnT = std::conditional_t<IsSelfConst<decltype(self)>(), const T*, T*>;
		using Object = std::decay_t<T>;

		auto& typeSize = self.typeSize;
		auto& removedObjects = self.removedObjects;
		auto& buffer = self.buffer;
		auto& objectsCount = self.objectsCount;
		auto& typeID = self.typeID;

		constexpr size_t objectSize = sizeof(Object);
		if (objectSize != typeSize)
			return static_cast<ReturnT>(nullptr);

		if (index >= objectsCount + removedObjects.size())
			return static_cast<ReturnT>(nullptr);

		if (GetTypeID<Object>() != typeID)
			return static_cast<ReturnT>(nullptr);

		if (std::ranges::contains(removedObjects, index))
			return static_cast<ReturnT>(nullptr);

		const size_t offset = index * typeSize;

#	if ZINET_SANITY_CHECK
		if (offset + typeSize > buffer.size())
		{
			Ensure(false); // Invalid offset
			return static_cast<ReturnT>(nullptr);
		}
#	endif

		return reinterpret_cast<ReturnT>(buffer.data() + offset);
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