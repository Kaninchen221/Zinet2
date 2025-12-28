#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObjectBase.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtDebug.hpp"

#include <vector>
#include <memory>
#include <type_traits>
#include <utility>

namespace zt::core
{
	class Object;
	class ObjectRefCounter;

	template<class ObjectType = Object, bool StrongRef = true, class ObjectRefCounterT = ObjectRefCounter>
	class ObjectHandle
	{
		inline static auto Logger = ConsoleLogger::Create("zt::core::ObjectHandle");

	public:

		using ObjectT = ObjectType;

		ObjectHandle() noexcept = default;
		ObjectHandle(ObjectRefCounterT* newObjectRefCounter) noexcept
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
			if (!other.isValid())
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

		auto getRefCounter() const noexcept 
		{
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

		using CanChangeCounter = std::bool_constant<StrongRef && std::is_same_v<ObjectRefCounterT, ObjectRefCounter>>;

		inline void increment() noexcept
		{
			if constexpr (CanChangeCounter{})
			{
				if (isValid())
					objectRefCounter->increment();
			}
		}

		inline void decrement() noexcept
		{
			if constexpr (CanChangeCounter{})
			{
				if (isValid())
					objectRefCounter->decrement();
			}
		}

		ObjectRefCounterT* objectRefCounter{};

	};
}

/// Not in "core" namespace because used too often
namespace zt
{
	/// Increment/Decrement ref count
	template<class ObjectT = core::Object, class ObjectRefCounterT = core::ObjectRefCounter>
	using ObjectHandle = core::ObjectHandle<ObjectT, true, ObjectRefCounterT>;

	/// Doesn't increment/decrement ref count
	template<class ObjectT = core::Object, class ObjectRefCounterT = core::ObjectRefCounter>
	using ObjectWeakHandle = core::ObjectHandle<ObjectT, false, ObjectRefCounterT>;
}