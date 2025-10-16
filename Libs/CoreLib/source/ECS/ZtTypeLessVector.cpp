#include "Zinet/Core/ECS/ZtTypeLessVector.hpp"

namespace zt::core::ecs
{
	TypeLessVector::~TypeLessVector() noexcept
	{
		if (isTriviallyDestructible)
			return;

		for (size_t i = 0; i < objectsCount + removedObjects.size(); ++i)
		{
			if (std::ranges::contains(removedObjects, i))
				continue;

			const size_t index = i * typeSize;
			destructor.invoke(&buffer[index]);
		}
	}

	bool TypeLessVector::remove(size_t index)
	{
		if (index >= objectsCount + removedObjects.size())
			return false;

		if (std::ranges::contains(removedObjects, index))
			return false;

		removedObjects.push_back(index);

		auto* address = &buffer[index * typeSize];

		if (!isTriviallyDestructible)
			destructor.invoke(address);

		--objectsCount;

		// Fill the range of buffer with zeros to avoid dangling data
		const std::vector<std::byte> zeroBuffer{ typeSize, std::byte{} };
		std::memcpy(address, zeroBuffer.data(), typeSize);

		return true;
	}

	bool TypeLessVector::isValidIndex(size_t index) const noexcept
	{
		if (index >= objectsCount + removedObjects.size())
			return false;

		if (std::ranges::contains(removedObjects, index))
			return false;

		return true;
	}

	size_t TypeLessVector::getFirstValidIndex() const noexcept
	{
		if (objectsCount == 0)
			return InvalidIndex;

		for (size_t i = 0; i < objectsCount + removedObjects.size(); ++i)
		{
			if (!std::ranges::contains(removedObjects, i))
				return i;
		}

		return InvalidIndex;
	}
}