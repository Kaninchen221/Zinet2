#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

namespace zt::core
{
	class ClassInfoBase;

	class ZINET_CORE_API ObjectBase
	{
	public:

		ObjectBase() = default;
		ObjectBase(const ObjectBase& other) = default;
		ObjectBase(ObjectBase&& other) = default;

		virtual ~ObjectBase() noexcept = default;

		ObjectBase& operator = (const ObjectBase& other) = default;
		ObjectBase& operator = (ObjectBase&& other) = default;

		virtual const ClassInfoBase* getClassInfo() const = 0;

		virtual ObjectBase* createCopy() const = 0;
	};
}