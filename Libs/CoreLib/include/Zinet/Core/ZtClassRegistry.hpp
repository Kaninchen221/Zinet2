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

		ClassRegistry() = default;
		ClassRegistry(const ClassRegistry& other) = default;
		ClassRegistry(ClassRegistry&& other) noexcept = default;
		~ClassRegistry() noexcept = default;

		ClassRegistry& operator = (const ClassRegistry& other) = default;
		ClassRegistry& operator = (ClassRegistry&& other) noexcept = default;

		template<std::derived_from<BaseClassT> ClassT>
		void registerClass()
		{
			auto cdo = std::make_shared<ClassT>();
			auto className = cdo->getClassName();
			cdos.insert({ className, cdo });
		}

		auto getCDOs() const noexcept { return cdos; }

		std::shared_ptr<BaseClassT> getClassByName(const std::string& name)
		{
			if (auto result = cdos.find(name); result != cdos.end())
				return result->second;

			return std::shared_ptr<BaseClassT>{};
		}

	protected:

		Classes cdos;

	};

}