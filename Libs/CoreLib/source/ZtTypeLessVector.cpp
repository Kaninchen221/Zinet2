#include "Zinet/Core/ZtTypeLessVector.hpp"

namespace zt::core
{
	bool TypeLessVectorIterator::operator==(const TypeLessVectorIterator& other) const noexcept
	{
		return vector == other.vector && currentIndex == other.currentIndex;
	}

	bool TypeLessVectorIterator::operator!=(const TypeLessVectorIterator& other) const noexcept
	{
		return !operator==(other);
	}

	TypeLessVectorIterator& TypeLessVectorIterator::operator++() noexcept
	{
		do
		{
			currentIndex++;
		} 
		while (!vector->isValidIndex(currentIndex) && currentIndex < vector->getLastIndex() + 1);

		return *this;
	}

	void* TypeLessVectorIterator::operator*() const noexcept
	{
		return vector->getPtr(currentIndex);
	}

	TypeLessVector::~TypeLessVector() noexcept
	{
		clear();
	}

	void TypeLessVector::clear()
	{
		if (!isTriviallyDestructible)
		{
			for (size_t i = 0; i < objectsCount + removedObjects.size(); ++i)
			{
				if (std::ranges::contains(removedObjects, i))
					continue;

				const size_t index = i * typeSize;
				destructor.invoke(&buffer[index]);
			}
		}

		objectsCount = 0;
		objectsCapacity = 0;
		buffer.clear();
		buffer.shrink_to_fit();
		removedObjects.clear();
		removedObjects.shrink_to_fit();
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

	TypeLessVectorIterator TypeLessVector::begin() noexcept
	{
		if (isEmpty())
			return end();

		return TypeLessVectorIterator{ this, getFirstValidIndex() };
	}

	TypeLessVectorIterator TypeLessVector::end() noexcept
	{
		return TypeLessVectorIterator{ this, getLastIndex() + 1 };
	}

}