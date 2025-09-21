#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObjectBase.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtDebug.hpp"

#include <vector>
#include <memory>
#include <type_traits>
#include <utility>

#include <fmt/format.h>

// TODO: Refactor move to separate files and move functions to .cpp
namespace zt::core
{
	class Object;
	class Archive;
	class JsonArchive;

	class ObjectRefCounter
	{
		inline static auto Logger = ConsoleLogger::Create("zt::core::ObjectRefCounter");

	public:

		ObjectRefCounter() noexcept = default;
		ObjectRefCounter(Object* objectPtr) noexcept;
		ObjectRefCounter(const ObjectRefCounter& other) = delete;
		ObjectRefCounter(ObjectRefCounter&& other) noexcept;

		~ObjectRefCounter() noexcept;

		ObjectRefCounter& operator = (const ObjectRefCounter& other) noexcept = delete;
		ObjectRefCounter& operator = (ObjectRefCounter&& other) noexcept;

		template<std::derived_from<Object> ObjectT>
		void create(const std::string_view displayName);

		void increment() noexcept;

		void decrement() noexcept;

		inline void destroy() { reset(); }

		void reset();

		bool isValid() const noexcept;

		operator bool() const noexcept { return isValid(); }

		size_t getRefCount() const noexcept { return refCount; }

		Object* get() const noexcept;

		Object* operator->() const noexcept { return get(); }

	protected:
		size_t refCount = 0;
		// TODO: Objects should be stored in a better way than some random place in the memory
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

		ObjectHandle() noexcept = default;
		ObjectHandle(ObjectRefCounter* newObjectRefCounter) noexcept
		{
			objectRefCounter = newObjectRefCounter;
			increment();
		}

		template<class ObjectHandleT>
		ObjectHandle(ObjectHandleT& objectHandle) noexcept
			: ObjectHandle(objectHandle.getRefCounter())
		{
		}

		ObjectHandle(const ObjectHandle<ObjectT>& other) noexcept { *this = other; }
		ObjectHandle(ObjectHandle<ObjectT>&& other) noexcept { *this = std::forward<ObjectHandle<ObjectT>>(other); }
		~ObjectHandle() noexcept { decrement(); }

		ObjectHandle<ObjectT>& operator = (const ObjectHandle<ObjectT>& other) noexcept
		{
			if (!Ensure(other.isValid()))
			{
				Logger->warn("Attempting to assign an ObjectHandle with invalid object ref counter");
				return *this;
			}

			objectRefCounter = other.objectRefCounter;
			increment();
			return *this;
		}

		ObjectHandle<ObjectT>& operator = (ObjectHandle<ObjectT>&& other) noexcept
		{
			if (!Ensure(other.isValid()))
				return *this;

			if (isValid())
				invalidate();

			objectRefCounter = other.objectRefCounter;
			other.objectRefCounter = nullptr;
			return *this;
		}

		bool isValid() const noexcept { return objectRefCounter && objectRefCounter->isValid(); }

		operator bool() const noexcept { return isValid(); }

		ObjectT* operator->() const noexcept { return get(); }

		ObjectT* get() const noexcept
		{
			if (!isValid())
				return nullptr;

			return dynamic_cast<class ObjectT*>(objectRefCounter->get());
		}

		ObjectT& operator*() const
		{
			if (!isValid)
			{
				Logger->error("Attempting to dereference an invalid pointer");
				Terminate();
			}

			return *dynamic_cast<class ObjectT*>(objectRefCounter->get());
		}

		template<class ObjectHandleT>
		bool operator == (const ObjectHandleT& other) const noexcept
		{
			return getRefCounter() == other.getRefCounter();
		}

		ObjectRefCounter* getRefCounter() const noexcept 
		{ 
#		if ZINET_DEBUG
			if (!objectRefCounter)
				Terminate();
#		endif

			return objectRefCounter; 
		}

		size_t getRefCount() const noexcept { return objectRefCounter ? objectRefCounter->getRefCount() : 0; }

		void invalidate() noexcept { decrement(); objectRefCounter = nullptr; }

		ObjectRefCounter* release() noexcept
		{
			ObjectRefCounter* temp = objectRefCounter;
			objectRefCounter = nullptr;
			return temp;
		}

		auto createHandle() noexcept
		{
			if (!isValid())
				Terminate();

			return ObjectHandle<ObjectT, true>(objectRefCounter);
		}

		auto createWeakHandle() noexcept
		{
			if (!isValid())
				Terminate();

			return ObjectHandle<ObjectT, false>(objectRefCounter);
		}

		void destroy() { Ensure(objectRefCounter); objectRefCounter->reset(); }

		// TODO: Test it
		template<class DerivedT, bool CastedStrongRef>
		ObjectHandle<DerivedT, CastedStrongRef> castTo() noexcept
		{
			if (!isValid())
				return {};
		
			DerivedT* castedPtr = dynamic_cast<DerivedT*>(get());
			if (!Ensure(castedPtr, "DerivedT is not a derived type from base class"))
				return {};
		
			auto refCounter = getRefCounter();
			auto newObjectHandle = ObjectHandle<DerivedT, CastedStrongRef>(refCounter);
			return newObjectHandle;
		}

	protected:

		inline void increment() noexcept
		{
			if constexpr (StrongRef)
			{
				if (isValid())
					objectRefCounter->increment();
			}
		}

		inline void decrement() noexcept
		{
			if constexpr (StrongRef)
			{
				if (isValid())
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

namespace zt::core
{
	class Object : public ObjectBase
	{
		ObjectWeakHandle<Object> self;

	public:

		using ObjectPtr = std::unique_ptr<Object>;

		Object() = default;
		Object(const std::string& newDisplayName) : displayName{ newDisplayName } {}
		Object(const Object& other) = default;
		Object(Object&& other) noexcept = default;
		~Object() noexcept = default;

		Object& operator = (const Object& other) = default;
		Object& operator = (Object&& other) noexcept = default;

		ObjectWeakHandle<Object> getSelf() const noexcept { return self; }

		virtual void onCreate(ObjectWeakHandle<Object> newSelf) { self = newSelf; }

		virtual void onDestroy() {}

		virtual ObjectPtr createCopy() const { return {}; }

		virtual std::string asString() const { return "Object"; }

		virtual void operator << ([[maybe_unused]] Archive& archive) {}

		virtual bool serialize([[maybe_unused]] JsonArchive& archive) { return true; }

		virtual bool deserialize([[maybe_unused]] JsonArchive& archive) { return true; }

		virtual const std::string_view getClassName() const { return "zt::core::Object"; }

		void setDisplayName(const std::string_view newDisplayName) { displayName = newDisplayName; }
		const auto& getDisplayName() const { return displayName; }

		void setInspectable(bool value) noexcept { inspectable = value; }
		bool isInspectable() const noexcept { return inspectable; }

		void setSaveable(bool value) noexcept { saveable = value; }
		bool isSaveable() const noexcept { return saveable; }

		virtual void show() {}

	protected:

		bool inspectable = true;
		bool saveable = false;
		std::string displayName{};

	};
}

namespace zt::core
{
	template<std::derived_from<Object> ObjectT>
	void ObjectRefCounter::create(const std::string_view displayName)
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
}