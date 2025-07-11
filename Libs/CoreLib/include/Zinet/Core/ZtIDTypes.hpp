#pragma once

#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <limits>

namespace zt::core
{
	class ZINET_CORE_API ID
	{
	public:

		const inline static constexpr size_t InvalidIDNumber = std::numeric_limits<size_t>::max();

		ID() = default;
		ID(size_t newNumber) : number{ newNumber } {}
		ID(const ID& other) = default;
		ID(ID&& other) = default;

		ID& operator = (const ID& other) = default;
		ID& operator = (ID&& other) = default;

		~ID() ZINET_API_POST = default;

		bool operator == (size_t otherNumber) const { return number == otherNumber; }
		bool operator == (const ID& other) const { return number == other.number; }

		size_t getNumber() const { return number; }

	protected:

		size_t number = InvalidIDNumber;

	};

	class ZINET_CORE_API UniqueID : public ID
	{
	public:

		UniqueID() = default;
		UniqueID(size_t newNumber) : ID{newNumber} {}
		UniqueID(const UniqueID& other) = delete;
		UniqueID(UniqueID&& other);
		
		UniqueID& operator = (const UniqueID& other) = delete;
		UniqueID& operator = (UniqueID&& other);
		
		~UniqueID() ZINET_API_POST = default;

		ID createID() const { return ID{ number }; }

	};

}

