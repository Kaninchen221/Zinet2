#include "Zinet/Core/ZtObjectRefCounter.hpp"

namespace zt::core
{
	ObjectRefCounter::ObjectRefCounter(Object* objectPtr) noexcept
		: object{ objectPtr } 
	{}

	ObjectRefCounter::ObjectRefCounter(ObjectRefCounter&& other) noexcept
	{
		*this = std::forward<ObjectRefCounter>(other);
	}

	ObjectRefCounter::~ObjectRefCounter() noexcept
	{
		object.reset();
	}

	ObjectRefCounter& ObjectRefCounter::operator=(ObjectRefCounter&& other) noexcept
	{
		refCount = other.refCount;
		other.refCount = 0;
		object = std::move(other.object);
		return *this;
	}

	void ObjectRefCounter::increment() noexcept
	{
		++refCount;
	}

	void ObjectRefCounter::decrement() noexcept
	{
		if (refCount <= 0)
		{
			Ensure(false, "refCount couldn't be less than 0");
			return;
		}

		--refCount;
	}

	void ObjectRefCounter::reset()
	{
		refCount = 0;
		object->onDestroy();
		object.reset();
	}

	bool ObjectRefCounter::isValid() const noexcept
	{
		return object.operator bool();
	}

	Object* ObjectRefCounter::get() const noexcept
	{
		if (!isValid())
			TerminateDebug();

		return object.get();
	}

}