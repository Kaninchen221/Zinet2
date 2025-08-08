#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtClassDefaultObjectRegistry.hpp"
#include "Zinet/Core/ZtObjectBase.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include <vector>
#include <memory>
#include <type_traits>

#include <fmt/format.h>

namespace zt::core
{
	class Archive;
	class JsonArchive;

	class ZINET_CORE_API Object : public ObjectBase
	{
	public:

		using ObjectPtr = std::shared_ptr<Object>;

		Object() = default;
		Object(const Object& other) = default;
		Object(Object&& other) = default;
		~Object() ZINET_API_POST = default;

		Object& operator = (const Object& other) ZINET_API_POST = default;
		Object& operator = (Object&& other) ZINET_API_POST = default;

		virtual ObjectPtr createCopy() const ZINET_API_POST { return {}; }

		virtual std::string asString() const ZINET_API_POST { return "Object"; }

		virtual void operator << ([[maybe_unused]] Archive& archive) ZINET_API_POST {}

		virtual bool serialize([[maybe_unused]] JsonArchive& archive) ZINET_API_POST { return true; }

		virtual bool deserialize([[maybe_unused]] JsonArchive& archive) ZINET_API_POST { return true; }

		virtual std::string getClassName() const ZINET_API_POST { return "zt::core::Object"; }

		void setDisplayName(const std::string_view newDisplayName) ZINET_API_POST { displayName = newDisplayName; }
		const auto& getDisplayName() const ZINET_API_POST { return displayName; }

		bool isInspectable = true;
		virtual void imGui() ZINET_API_POST {}

		bool isSaveable = false;

	protected:

		std::string displayName;

	};

	class ZINET_CORE_API ObjectRefCounter
	{
		inline static auto Logger = ConsoleLogger::Create("zt::core::ObjectRefCounter");

	public:

		ObjectRefCounter() ZINET_API_POST = default;
		ObjectRefCounter(const ObjectRefCounter& other) ZINET_API_POST = delete;
		ObjectRefCounter(ObjectRefCounter&& other) ZINET_API_POST
		{
			refCount = other.refCount;
			other.refCount = 0;
			object = std::move(other.object);
		}
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
	
	template<class ObjectType = Object, bool StrongRef = true>
	class ObjectHandle
	{
		inline static auto Logger = ConsoleLogger::Create("zt::core::ObjectHandle");

	public:

		using ObjectT = ObjectType;

		/// Not in template to avoid compile time error
		/// Example: std::vector<ObjectHandle<Node>> in the Node class
		/// TODO: Check base class
		//static_assert(std::derived_from<ObjectType, Object> || std::is_base_of_v<Object, ObjectT>, "ObjectType must be derived from Object");

		ObjectHandle() ZINET_API_POST = default;
		ObjectHandle(ObjectRefCounter* newObjectRefCounter) ZINET_API_POST
		{
			objectRefCounter = newObjectRefCounter;
			increment();
		}

		template<class ObjectHandleT>
		ObjectHandle(ObjectHandleT& objectHandle) ZINET_API_POST
			: ObjectHandle(objectHandle.getRefCounter())
		{
			static_assert(std::derived_from<typename ObjectHandleT::ObjectT, typename ObjectHandle<ObjectT, StrongRef>::ObjectT>,
				"ObjectHandleT must be derived from ObjectHandle with the same ObjectT type");
		}

		ObjectHandle(const ObjectHandle& other) ZINET_API_POST { *this = other; }
		ObjectHandle(ObjectHandle&& other) ZINET_API_POST { *this = other; }
		~ObjectHandle() noexcept { decrement(); }

		ObjectHandle& operator = (const ObjectHandle& other) ZINET_API_POST
		{
			if (!other.objectRefCounter)
			{
				Logger->warn("Attempting to assign an ObjectHandle with invalid object ref counter");
				Ensure(false);
				return *this;
			}

			objectRefCounter = other.objectRefCounter;
			increment();
			return *this;
		}
		ObjectHandle& operator = (ObjectHandle&& other) ZINET_API_POST
		{
			objectRefCounter = other.objectRefCounter;
			other.objectRefCounter = nullptr;
			return *this;
		}

		bool isValid() const ZINET_API_POST { return objectRefCounter && objectRefCounter->isValid(); }

		operator bool() const ZINET_API_POST { return isValid(); }

		ObjectT* operator->() const ZINET_API_POST { return get(); }

		ObjectT* get() const ZINET_API_POST
		{
			if (!objectRefCounter)
				return nullptr;

			return dynamic_cast<ObjectT*>(objectRefCounter->get());
		}

		ObjectRefCounter* getRefCounter() const ZINET_API_POST { return objectRefCounter; }

		ObjectT& operator*() const ZINET_API_POST
		{
			if (!objectRefCounter)
			{
				Logger->warn("Attempting to dereference an ObjectHandle with invalid object ref counter");
				Ensure(false);
			}

			return *get();
		}

		size_t getRefCount() const ZINET_API_POST { return objectRefCounter ? objectRefCounter->getRefCount() : 0; }

		void invalidate() ZINET_API_POST { decrement(); objectRefCounter = nullptr; }

		auto createHandle() ZINET_API_POST
		{
			return ObjectHandle<ObjectT, true>(objectRefCounter);
		}

		auto createWeakHandle() ZINET_API_POST
		{
			return ObjectHandle<ObjectT, false>(objectRefCounter);
		}

	protected:

		inline void increment() ZINET_API_POST
		{
			if constexpr (StrongRef)
			{
				if (objectRefCounter)
					objectRefCounter->increment();
			}
		}

		inline void decrement() ZINET_API_POST
		{
			if constexpr (StrongRef)
			{
				if (objectRefCounter)
					objectRefCounter->decrement();
			}
		}

		ObjectRefCounter* objectRefCounter{};

	};
}

/// Not in "core" namespace because used too often
namespace zt
{
	/// Increment/Decrement ref count
	template<class ObjectT = core::Object>
	using ObjectHandle = core::ObjectHandle<ObjectT, true>;

	/// Doesn't increment/decrement ref count
	template<class ObjectT = core::Object>
	using ObjectWeakHandle = core::ObjectHandle<ObjectT, false>;
}