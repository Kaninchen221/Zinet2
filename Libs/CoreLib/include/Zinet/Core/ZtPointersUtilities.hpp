#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <memory>

namespace zt::core::PointersUtilities
{

	template<class BaseClass, std::derived_from<BaseClass> DerivedClass>
	std::unique_ptr<DerivedClass> DynamicCastUniquePtr(std::unique_ptr<BaseClass>& object)
	{
		BaseClass* rawPointer = object.release();
		DerivedClass* rawPointerCasted = dynamic_cast<DerivedClass*>(rawPointer);
		return std::move(std::unique_ptr<DerivedClass>{rawPointerCasted});
	}

	template<class BaseClass, std::derived_from<BaseClass> DerivedClass>
	std::unique_ptr<DerivedClass> StaticCastUniquePtr(std::unique_ptr<BaseClass>& object)
	{
		BaseClass* rawPointer = object.release();
		DerivedClass* rawPointerCasted = static_cast<DerivedClass*>(rawPointer);
		return std::move(std::unique_ptr<DerivedClass>{rawPointerCasted});
	}

}