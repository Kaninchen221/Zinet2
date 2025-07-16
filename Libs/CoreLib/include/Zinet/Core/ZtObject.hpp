#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtClassDefaultObjectRegistry.hpp"
#include "Zinet/Core/ZtObjectBase.hpp"

#include <vector>
#include <memory>
#include <type_traits>

namespace zt::core
{
	class ZINET_CORE_API Object : public ObjectBase
	{
	public:

		Object() = default;
		Object(const Object& other) = default;
		Object(Object&& other) = default;
		~Object() ZINET_API_POST = default;

		Object& operator = (const Object& other) ZINET_API_POST = default;
		Object& operator = (Object&& other) ZINET_API_POST = default;

		virtual std::string asString() const { return {}; }
	};
}