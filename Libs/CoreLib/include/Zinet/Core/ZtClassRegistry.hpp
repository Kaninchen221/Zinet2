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
	class ClassRegistry : public Object
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("zt::core::ClassRegistry");

	public:

		using BaseClassT = BaseClassType;
		using KeyT = std::string;
		using Classes = std::map<KeyT, std::shared_ptr<BaseClassType>>;

		ClassRegistry() ZINET_API_POST = default;
		ClassRegistry(const ClassRegistry& other) ZINET_API_POST = default;
		ClassRegistry(ClassRegistry&& other) ZINET_API_POST = default;
		~ClassRegistry() ZINET_API_POST = default;

		ClassRegistry& operator = (const ClassRegistry& other) ZINET_API_POST = default;
		ClassRegistry& operator = (ClassRegistry&& other) ZINET_API_POST = default;

		template<std::derived_from<BaseClassT> ClassT>
		void registerClass(const KeyT& name) ZINET_API_POST
		{
			cdos.insert({ name, std::make_shared<ClassT>() });
		}

		const auto& getCDOs() const ZINET_API_POST { return cdos; }

		std::shared_ptr<BaseClassT> getClassByName(const std::string& name) ZINET_API_POST
		{
			if (auto result = cdos.find(name); result != cdos.end())
				return result->second;

			return std::shared_ptr<BaseClassT>{};
		}

	protected:

		Classes cdos;

	};

}

#include "Zinet/Core/ZtClassRegistryImpl.hpp"