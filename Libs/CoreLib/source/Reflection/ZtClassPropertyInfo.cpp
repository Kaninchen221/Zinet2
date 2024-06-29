#include "Zinet/Core/Reflection/ZtClassPropertyInfo.hpp"

namespace zt::core::reflection
{

	ClassPropertiesInfos::ClassPropertiesInfos()
	{}

	ClassPropertiesInfos::ClassPropertiesInfos(const std::vector<ClassPropertyInfo>& properties)
		: propertiesInfos{ properties }
	{}

	std::optional<ClassPropertyInfo> ClassPropertiesInfos::findFirstWithPropertyName(const std::string_view propertyName)
	{
		for (const auto& info : propertiesInfos)
		{
			if (info.getPropertyName() == propertyName)
			{
				return info;
			}
		}
		return {};
	}

	std::optional<zt::core::reflection::ClassPropertyInfo> ClassPropertiesInfos::findFirstWithPropertyName(const std::string_view propertyName) const
	{
		for (const auto& info : propertiesInfos)
		{
			if (info.getPropertyName() == propertyName)
			{
				return info;
			}
		}
		return {};
	}

	ClassPropertyInfo::ClassPropertyInfo(size_t offset, std::string_view name, std::string_view typeName)
		: memoryOffset{ offset }, propertyName{ name }, propertyTypeName{typeName}
	{

	}

}