#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtClassDefaultObjectRegistry.hpp"
#include "Zinet/Core/ZtObjectBase.hpp"

#include <vector>
#include <memory>
#include <type_traits>

namespace zt::core
{
	ZT_REFLECT_CLASS(NO_TEST_BASE_CLASS_OBJECT, NO_CONSTRUCTORS, NO_DESTRUCTOR, NO_CREATE_COPY)
	class ZINET_CORE_API Object : public ObjectBase
	{
	public:

		Object() = default;
		Object(const Object& other) = default;
		Object(Object&& other) = default;
		~Object() noexcept = default;

		template<class T> // Can't use std::derived_from concept because we use this function with incomplete type
		static bool RegisterClass();

		template<std::derived_from<Object> T>
		std::unique_ptr<Object> createCopyInternal() const;

		std::unique_ptr<ObjectBase> createCopy() const override { return nullptr; }

		virtual std::string asString() const { return {}; }

	protected:

		inline static constexpr bool IsObjectClassInherited = true;

	private:

	public:
/*GENERATED_CODE_START*/
		const inline static bool RegisterClassResult = RegisterClass<Object>();
		
		Object& operator = (const Object& other) = default;
		Object& operator = (Object&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "Object"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
/*GENERATED_CODE_END*/
	};

	template<class T>
	bool Object::RegisterClass()
	{
		static_assert(std::is_base_of_v<Object, T>);

		ClassDefaultObjectRegistry& CDORegistry = ClassDefaultObjectRegistry::Get();
		CDORegistry.registerClass(new T{});
		return true;
	}

	template<std::derived_from<Object> T>
	std::unique_ptr<Object> Object::createCopyInternal() const
	{
		if constexpr (std::is_copy_constructible_v<T>)
			return std::unique_ptr<Object>{ new T(*static_cast<const T*>(this)) };
		else
			return std::unique_ptr<Object>{ new T() };
	}

}