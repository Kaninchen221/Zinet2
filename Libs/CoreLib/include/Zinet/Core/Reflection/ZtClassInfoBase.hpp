#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

namespace zt::core
{
	class ZINET_CORE_API ClassInfoBase
	{

	public:
		ClassInfoBase() = default;
		ClassInfoBase(const ClassInfoBase& other) = default;
		ClassInfoBase(ClassInfoBase&& other) = default;

		~ClassInfoBase() noexcept = default;

		ClassInfoBase& operator = (const ClassInfoBase& other) = default;
		ClassInfoBase& operator = (ClassInfoBase&& other) = default;


	protected:

	};

}