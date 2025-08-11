#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

namespace zt::core
{
	class ClassInfoBase;

	class ZINET_CORE_API ObjectBase
	{
	public:

		ObjectBase() noexcept = default;
		ObjectBase(const ObjectBase& other) noexcept = default;
		ObjectBase(ObjectBase&& other) noexcept = default;
		virtual ~ObjectBase() noexcept = default;

		ObjectBase& operator = (const ObjectBase& other) noexcept = default;
		ObjectBase& operator = (ObjectBase&& other) noexcept = default;

	};
}