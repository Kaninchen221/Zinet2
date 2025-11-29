#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObjectBase.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtDebug.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include <vector>
#include <memory>
#include <type_traits>
#include <utility>

namespace zt::core
{
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
		// Use TypeLessVector to store assets?
		std::unique_ptr<Object> object;
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