#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include <memory_resource>

namespace zt::core
{
	class ZINET_CORE_API ObjectRefCounter
	{
		inline static auto Logger = ConsoleLogger::Create("zt::core::ObjectRefCounter");

	public:

		ObjectRefCounter() ZINET_API_POST = default;
		ObjectRefCounter(const ObjectRefCounter& other) ZINET_API_POST = delete;
		ObjectRefCounter(ObjectRefCounter&& other) ZINET_API_POST = default;
		~ObjectRefCounter() noexcept = default;

		ObjectRefCounter& operator = (const ObjectRefCounter& other) ZINET_API_POST = delete;
		ObjectRefCounter& operator = (ObjectRefCounter&& other) ZINET_API_POST = default;

		template<std::derived_from<Object> ObjectT>
		void create(const std::string_view displayName) ZINET_API_POST
		{
			if (isValid())
			{
				Logger->warn("Object already exists");
				Ensure(false);
				return;
			}

			object = std::make_unique<ObjectT>();
			object->setDisplayName(displayName);
			refCount = 0;
		}

		void increment() ZINET_API_POST 
		{ 
			++refCount; 
		}

		void decrement() ZINET_API_POST 
		{ 
			--refCount; 
			if (refCount == 0)
				reset();
		}

		void reset() ZINET_API_POST
		{
			refCount = 0;
			object.reset();
		}

		bool isValid() const ZINET_API_POST { return object.operator bool(); }

		operator bool() const ZINET_API_POST { return isValid(); }

		size_t getRefCount() const ZINET_API_POST { return refCount; }

		Object* get() const ZINET_API_POST { return object.get(); }

	protected:
		size_t refCount = 0;
		std::unique_ptr<Object> object;
	};

	template<std::derived_from<Object> ObjectType = Object>
	class ObjectHandle
	{
		inline static auto Logger = ConsoleLogger::Create("zt::core::ObjectHandle");

	public:

		using ObjectT = ObjectType;

		ObjectHandle() ZINET_API_POST = default;
		ObjectHandle(ObjectRefCounter* newObjectRefCounter) ZINET_API_POST 
		{ 
			objectRefCounter = newObjectRefCounter; 
			if (objectRefCounter)
				objectRefCounter->increment();
		}
		ObjectHandle(const ObjectHandle& other) ZINET_API_POST { *this = other; }
		ObjectHandle(ObjectHandle&& other) ZINET_API_POST { *this = other; }
		~ObjectHandle() noexcept
		{
			if (objectRefCounter)
				objectRefCounter->decrement();
		}

		ObjectHandle& operator = (const ObjectHandle& other) ZINET_API_POST 
		{
			if (!other.objectRefCounter)
			{
				Logger->warn("Attempting to assign an ObjectHandle with invalid object ref counter");
				Ensure(false);
				return *this;
			}

			objectRefCounter = other.objectRefCounter;
			objectRefCounter->increment();
			return *this;
		}
		ObjectHandle& operator = (ObjectHandle&& other) ZINET_API_POST
		{
			objectRefCounter = other.objectRefCounter;
			other.objectRefCounter = nullptr;
			return *this;
		}

		bool isValid() const ZINET_API_POST { return objectRefCounter; }

		operator bool() const ZINET_API_POST { return isValid(); }

		ObjectT* operator->() const ZINET_API_POST { get(); }

		ObjectT* get() const ZINET_API_POST 
		{ 
			if (!objectRefCounter)
				return nullptr;

			return objectRefCounter->get(); 
		}

		void invalidate() ZINET_API_POST 
		{ 
			if (objectRefCounter)
			{
				objectRefCounter->decrement();
				objectRefCounter = nullptr;
			}
		}

		size_t getRefCount() const ZINET_API_POST { return objectRefCounter ? objectRefCounter->getRefCount() : 0; }

	protected:

		ObjectRefCounter* objectRefCounter;

	};

	class ZINET_CORE_API ObjectsStorage
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("zt::core::ObjectsStorage");

	public:

		using MemoryPool = std::pmr::unsynchronized_pool_resource;
		using Objects = std::pmr::vector<ObjectRefCounter>;

		ObjectsStorage() ZINET_API_POST = default;
		ObjectsStorage(const ObjectsStorage& other) ZINET_API_POST = delete;
		ObjectsStorage(ObjectsStorage&& other) ZINET_API_POST = default;
		~ObjectsStorage() noexcept = default;

		ObjectsStorage& operator = (const ObjectsStorage& other) ZINET_API_POST = delete;
		ObjectsStorage& operator = (ObjectsStorage&& other) ZINET_API_POST = default;

		template<std::derived_from<Object> ObjectT>
		ObjectHandle<ObjectT> createObject(const std::string_view displayName) ZINET_API_POST;

	protected:

		MemoryPool memoryPool;
		Objects objects{ &memoryPool };

	};

	template<std::derived_from<Object> ObjectT> 
	ObjectHandle<ObjectT> ObjectsStorage::createObject(const std::string_view displayName) ZINET_API_POST
	{
		Logger->info("Creating object with display name '{}'", displayName);

		auto& objectRefCount = objects.emplace_back();
		objectRefCount.create<ObjectT>(displayName);

		return ObjectHandle<ObjectT>(&objectRefCount);
	}

}