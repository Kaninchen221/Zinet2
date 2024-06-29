#include "Zinet/Core/Reflection/ZtClassesInfos.hpp"

namespace zt::core::reflection
{
	std::shared_ptr<ClassInfo> ClassesInfos::getByClassName(std::string_view className)
	{
		for (const auto& classInfo : internalClassInfos)
		{
			if (!classInfo)
				return {};

			if (classInfo->getClassName() == className)
				return classInfo;
		}

		return {};
	}

	ClassesInfos& ClassesInfos::Get()
	{
		static ClassesInfos classesInfos;
		return classesInfos;
	}

}