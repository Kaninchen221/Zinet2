#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtTypeTraits.hpp"

#include <typeinfo>
#include <optional>
#include <ranges>
#include <xutility>
#include <string>
#include <vector>

namespace zt::core::reflection
{
	class ClassPropertyInfo;

	class ZINET_CORE_API ClassPropertiesInfos
	{
	public:
		
		ClassPropertiesInfos();
		ClassPropertiesInfos(const std::vector<ClassPropertyInfo>& properties);

		auto& get() { return propertiesInfos; }
		auto& get() const { return propertiesInfos; }

		std::optional<ClassPropertyInfo> findFirstWithPropertyName(const std::string_view propertyName);
		std::optional<ClassPropertyInfo> findFirstWithPropertyName(const std::string_view propertyName) const;

		auto* operator -> () { return &propertiesInfos; }
		auto* operator -> () const { return &propertiesInfos; }

	protected:
		std::vector<ClassPropertyInfo> propertiesInfos;

	};

	class ZINET_CORE_API ClassPropertyInfo
	{
	public:

		ClassPropertyInfo() = default;
		ClassPropertyInfo(size_t offset, std::string_view name, std::string_view typeName);

		size_t getMemoryOffset() const { return memoryOffset; }
		std::string_view getPropertyName() const { return propertyName; }
		std::string_view getPropertyTypeName() const { return propertyTypeName; }

		template<typename T>
		bool is();

		template<typename T>
		bool is() const;

		template<typename ReturnT, typename ClassT>
		ReturnT& cast(ClassT* classObjectPtr);

	protected:

		size_t memoryOffset = 0;
		std::string_view propertyName;
		std::string_view propertyTypeName;
	};

	template<typename T>
	bool ClassPropertyInfo::is()
	{
		const std::type_info& typeInfo = typeid(T);
		const std::string_view typeName = typeInfo.name();
		const std::array<std::string, 2u> prefixes = { "", "::" };
		for (const auto& prefix : prefixes)
		{
			auto const endsWith = prefix + std::string(propertyTypeName);
			if (typeName.ends_with(endsWith))
				return true;
		}
		return false;
	}

	template<typename T>
	bool ClassPropertyInfo::is() const
	{
		const std::type_info& typeInfo = typeid(T);
		const std::string_view typeName = typeInfo.name();
		const std::array<std::string, 2u> prefixes = {"", "::"};
		for (const auto& prefix : prefixes)
		{
			auto const endsWith = prefix + std::string(propertyTypeName);
			if (typeName.ends_with(endsWith))
				return true;
		}
		return false;
	}

	template<typename ReturnT, typename ClassT>
	ReturnT& ClassPropertyInfo::cast(ClassT* classObjectPtr)
	{
		ReturnT* objectPointer = reinterpret_cast<ReturnT*>(classObjectPtr);
		objectPointer += memoryOffset / 4;
		return *objectPointer;
	}
}