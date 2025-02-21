#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include "Zinet/Core/Reflection/ZtClassInfoBase.hpp"

#include <memory>

namespace zt::core
{
	class ClassInfoBase;

	ZT_REFLECT_CLASS(NO_TEST_BASE_CLASS_OBJECT, NO_CONSTRUCTORS, NO_OPERATORS, NO_CREATE_COPY, NO_GET_CLASS_INFO, NO_REGISTER_CLASS)
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

		virtual std::unique_ptr<ObjectBase> createCopy() const = 0;

		virtual bool canBeCreatedFromAsset() const { return false; }

		virtual std::string_view getAssetExtension() const { return ""; }

	public:
		/*GENERATED_CODE_START*/
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "ObjectBase"; }
		};
		
/*GENERATED_CODE_END*/
	};
}