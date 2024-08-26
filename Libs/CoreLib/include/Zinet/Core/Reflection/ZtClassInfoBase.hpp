#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <string_view>

namespace zt::core
{
	class ZINET_CORE_API ClassInfoBase
	{

	public:
		ClassInfoBase() = default;
		ClassInfoBase(const ClassInfoBase& other) = default;
		ClassInfoBase(ClassInfoBase&& other) = default;

		virtual ~ClassInfoBase() noexcept = default;

		ClassInfoBase& operator = (const ClassInfoBase& other) = default;
		ClassInfoBase& operator = (ClassInfoBase&& other) = default;

		virtual std::string_view getClassName() const = 0;

	protected:

	};

}