#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/Assets/ZtAssetsFinder.hpp"
#include "Zinet/Core/Assets/ZtAsset.hpp"

#include <unordered_map>
#include <tuple>

namespace zt::core
{
	template<std::derived_from<Object> BaseClassType>
	struct ClassInfo
	{
		using Names = std::vector<std::string_view>;
		using ClassDefaultObject = std::shared_ptr<BaseClassType>;

		Names names;
		ClassDefaultObject cdo;
	};

	template<std::derived_from<Object> BaseClassType>
	class ClassRegistry : public Object
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("zt::core::ClassRegistry");

	public:

		using BaseClassT = BaseClassType;
		using Names = std::vector<std::string_view>;

		ClassRegistry() ZINET_API_POST = default;
		ClassRegistry(const ClassRegistry& other) ZINET_API_POST = default;
		ClassRegistry(ClassRegistry&& other) ZINET_API_POST = default;
		~ClassRegistry() ZINET_API_POST = default;

		ClassRegistry& operator = (const ClassRegistry& other) ZINET_API_POST = default;
		ClassRegistry& operator = (ClassRegistry&& other) ZINET_API_POST = default;

		template<std::derived_from<BaseClassT> ClassT>
		void registerClass(const Names& names) ZINET_API_POST
		{
			ClassInfo<BaseClassT> classInfo
			{
				.names = names,
				.cdo = std::make_unique<ClassT>()
			};

			classes.push_back(classInfo);
		}

		const auto& getClasses() const ZINET_API_POST { return classes; }

		auto getClassByName(std::string_view name) ZINET_API_POST
		{
			for (const auto& classInfo : classes)
			{
				if (std::ranges::contains(classInfo.names, name))
					return classInfo.cdo;
			}

			return std::shared_ptr<BaseClassT>{};
		}

	protected:

		std::vector<ClassInfo<BaseClassT>> classes;

	};

}

#include "Zinet/Core/ZtClassRegistryImpl.hpp"