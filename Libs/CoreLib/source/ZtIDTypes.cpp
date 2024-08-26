#include "Zinet/Core/ZtIDTypes.hpp"

#include <utility>

namespace zt::core
{

	UniqueID::UniqueID(UniqueID&& other)
	{
		*this = std::move(other);
	}

	UniqueID& UniqueID::operator=(UniqueID&& other)
	{
		number = other.number;
		other.number = InvalidIDNumber;
		
		return *this;
	}

}