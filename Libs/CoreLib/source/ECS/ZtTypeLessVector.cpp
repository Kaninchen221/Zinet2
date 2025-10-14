#include "Zinet/Core/ECS/ZtTypeLessVector.hpp"

namespace zt::core::ecs
{
	TypeLessVector::~TypeLessVector() noexcept
	{
		if (isTriviallyDestructible)
			return;

		for (size_t i = 0; i < componentsCapacity; ++i)
		{
			if (std::ranges::contains(removedComponents, i))
				continue;

			const size_t index = i * typeSize;
			destructor.invoke(&components[index]);
		}
	}

	bool TypeLessVector::remove(size_t index)
	{
		if (index >= componentsCapacity)
			return false;

		if (std::ranges::contains(removedComponents, index))
			return false;

		removedComponents.push_back(index);

		auto* address = &components[index * typeSize];

		if (!isTriviallyDestructible)
			destructor.invoke(address);

		// Fill the range of buffer with zeros to avoid dangling data
		const std::vector<std::byte> zeroBuffer{ typeSize, std::byte{} };
		std::memcpy(address, zeroBuffer.data(), typeSize);

		return true;
	}

	bool TypeLessVector::isValidIndex(size_t index) const noexcept
	{
		if (index >= componentsCapacity)
			return false;

		if (std::ranges::contains(removedComponents, index))
			return false;

		return true;
	}

	size_t TypeLessVector::getFirstValidIndex() const noexcept
	{
		if (componentsCapacity == 0 || componentsCapacity == removedComponents.size())
			return InvalidIndex;

		for (size_t i = 0; i < componentsCapacity; ++i)
		{
			if (!std::ranges::contains(removedComponents, i))
				return i;
		}

		return InvalidIndex;
	}
}