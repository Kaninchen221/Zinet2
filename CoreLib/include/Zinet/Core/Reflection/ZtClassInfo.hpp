#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/Reflection/ZtClassPropertyInfo.hpp"

namespace zt::core::reflection
{

	class ZINET_CORE_API ClassInfo
	{
	public:

		virtual std::string_view getClassName() const;

		virtual ClassPropertiesInfos getClassPropertiesInfos() { return {}; }

	};

}