#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
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
		Object(Object&& other) noexcept = default;
		~Object() noexcept = default;

		Object& operator = (const Object& other) = default;
		Object& operator = (Object&& other) noexcept = default;

		virtual ObjectPtr createCopy() const { return {}; }

		virtual std::string asString() const { return "Object"; }

		virtual void operator << ([[maybe_unused]] Archive& archive) {}

		virtual bool serialize([[maybe_unused]] JsonArchive& archive) { return true; }

		virtual bool deserialize([[maybe_unused]] JsonArchive& archive) { return true; }

		// TODO: Change return type to string_view?
		virtual std::string getClassName() const { return "zt::core::Object"; }

		void setDisplayName(const std::string_view newDisplayName) { displayName = newDisplayName; }
		const auto& getDisplayName() const { return displayName; }

		bool isInspectable = true;

		virtual void imGui() {}

		bool isSaveable = false;

	protected:

		struct {
			std::string displayName;
		} objInternalData;
		std::string& displayName = objInternalData.displayName;

	};

	class ObjectRefCounter
	{
		inline static auto Logger = ConsoleLogger::Create("zt::core::ObjectRefCounter");

	public:

		ZINET_CORE_API ObjectRefCounter() noexcept = default;
		ZINET_CORE_API ObjectRefCounter(const ObjectRefCounter& other) = delete;
		ZINET_CORE_API ObjectRefCounter(ObjectRefCounter&& other) noexcept
		{
			refCount = other.refCount;
			other.refCount = 0;
			object = std::move(other.object);
		}
		ZINET_CORE_API ~ObjectRefCounter() noexcept = default;

		ZINET_CORE_API ObjectRefCounter& operator = (const ObjectRefCounter& other) noexcept = delete;
		ZINET_CORE_API ObjectRefCounter& operator = (ObjectRefCounter&& other) noexcept = default;

		template<std::derived_from<Object> ObjectT>
		void create(const std::string_view displayName)
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

		void increment() noexcept
		{
			++refCount;
		}

		void decrement() noexcept
		{
			--refCount;
			if (refCount == 0)
				reset();
		}

		void reset() noexcept
		{
			refCount = 0;
			object.reset();
		}

		bool isValid() const noexcept { return object.operator bool(); }

		operator bool() const noexcept { return isValid(); }

		size_t getRefCount() const noexcept { return refCount; }

		Object* get() const noexcept { return object.get(); }

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
			static_assert(std::derived_from<typename ObjectHandleT::ObjectT, typename ObjectHandle<ObjectT, StrongRef>::ObjectT>,
				"ObjectHandleT must be derived from ObjectHandle with the same ObjectT type");
		}

		ObjectHandle(const ObjectHandle& other) noexcept { *this = other; }
		ObjectHandle(ObjectHandle&& other) noexcept { *this = other; }
		~ObjectHandle() noexcept { decrement(); }

		ObjectHandle& operator = (const ObjectHandle& other) noexcept
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

		ObjectHandle& operator = (ObjectHandle&& other) noexcept
		{
			objectRefCounter = other.objectRefCounter;
			other.objectRefCounter = nullptr;
			return *this;
		}

		bool isValid() const noexcept { return objectRefCounter && objectRefCounter->isValid(); }

		operator bool() const noexcept { return isValid(); }

		ObjectT* operator->() const noexcept { return get(); }

		ObjectT* get() const noexcept
		{
			if (!objectRefCounter)
				return nullptr;

			return dynamic_cast<ObjectT*>(objectRefCounter->get());
		}

		ObjectT& operator*() const
		{
			if (!objectRefCounter)
			{
				Logger->error("Attempting to dereference an invalid pointer");
				Terminate();
			}

			return *dynamic_cast<ObjectT*>(objectRefCounter->get());
		}

		ObjectRefCounter* getRefCounter() const noexcept { return objectRefCounter; }

		size_t getRefCount() const noexcept { return objectRefCounter ? objectRefCounter->getRefCount() : 0; }

		void invalidate() noexcept { decrement(); objectRefCounter = nullptr; }

		auto createHandle() noexcept
		{
			return ObjectHandle<ObjectT, true>(objectRefCounter);
		}

		auto createWeakHandle() noexcept
		{
			return ObjectHandle<ObjectT, false>(objectRefCounter);
		}

	protected:

		inline void increment() noexcept
		{
			if constexpr (StrongRef)
			{
				if (objectRefCounter)
					objectRefCounter->increment();
			}
		}

		inline void decrement() noexcept
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