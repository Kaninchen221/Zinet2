#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include <filesystem>

#include <nlohmann/json.hpp>

namespace zt::core::assets
{
	ZT_REFLECT_CLASS()
	class ZINET_CORE_API Asset : public Object
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("Asset");

	public:

		std::vector<uint8_t> rawData;

		using json = nlohmann::json;
		json metaData;

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<Asset>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<Asset>(); *result = *this; return result; }
		
		Asset() = default;
		Asset(const Asset& other) = default;
		Asset(Asset&& other) = default;
		~Asset() ZINET_API_POST = default;
		
		Asset& operator = (const Asset& other) = default;
		Asset& operator = (Asset&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "Asset"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
/*GENERATED_CODE_END*/
	};

}