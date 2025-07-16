#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include "Zinet/Core/Reflection/ZtClassInfoBase.hpp"

#include <memory>

namespace zt::core
{
	class ClassInfoBase;

	class ZINET_CORE_API ObjectBase
	{
	public:

		ObjectBase() = default;
		ObjectBase(const ObjectBase& other) = default;
		ObjectBase(ObjectBase&& other) = default;
		virtual ~ObjectBase() ZINET_API_POST = default;

		ObjectBase& operator = (const ObjectBase& other) = default;
		ObjectBase& operator = (ObjectBase&& other) = default;

	};
}