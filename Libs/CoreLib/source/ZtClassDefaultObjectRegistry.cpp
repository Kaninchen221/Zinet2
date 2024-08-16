#include "Zinet/Core/ZtClassDefaultObjectRegistry.hpp"

#include "Zinet/Core/Reflection/ZtClassInfoBase.hpp"

namespace zt::core
{

	ClassDefaultObjectRegistry& ClassDefaultObjectRegistry::Get()
	{
		static ClassDefaultObjectRegistry CDORegistry;
		return CDORegistry;
	}

	void ClassDefaultObjectRegistry::registerClass(ObjectBase* object)
	{
		CDOList.push_back(std::unique_ptr<ObjectBase>(object));
	}

	std::unique_ptr<ObjectBase> ClassDefaultObjectRegistry::createObjectByClassName(const std::string& name) const
	{
		for (const auto& classDefaultObject : CDOList)
		{
			const zt::core::ClassInfoBase* classInfo = classDefaultObject->getClassInfo();
			if (classInfo->getClassName() == name)
			{
				return classDefaultObject->createCopy();
			}
		}

		return nullptr;
	}

}