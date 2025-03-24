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

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("Asset");

	public:

		std::unique_ptr<ObjectBase> createObject(const std::filesystem::path& contentRootFolder) const;

		virtual bool loadContentUsingMetaData(const std::filesystem::path& contentRootFolder) { return false; }

	protected:

		ZT_REFLECT_MEMBER(ReadWrite)
		nlohmann::json metaData;

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<Asset>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<Asset>(); *result = *this; return result; }
		
		Asset() = default;
		Asset(const Asset& other) = default;
		Asset(Asset&& other) = default;
		~Asset() noexcept = default;
		
		Asset& operator = (const Asset& other) = default;
		Asset& operator = (Asset&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "Asset"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
		const decltype(metaData)& getMetaData() const { return metaData; }
		decltype(metaData)& getMetaData() { return metaData; }
		void setMetaData(const decltype(metaData)& newValue) { metaData = newValue; }
		
/*GENERATED_CODE_END*/
	};

}