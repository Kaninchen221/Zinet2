#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include "Zinet/Core/Reflection/ZtClassInfo.hpp"

#include <tuple>
#include <memory>
#include <vector>

namespace zt::core::reflection
{
	class ZINET_CORE_API ClassesInfos
	{
	public:

		ClassesInfos() = default;
		ClassesInfos(const ClassesInfos& other) = delete;
		ClassesInfos(ClassesInfos&& other) = delete;

		~ClassesInfos() noexcept = default;

		ClassesInfos& operator = (const ClassesInfos& other) = delete;
		ClassesInfos& operator = (ClassesInfos&& other) = delete;

		static ClassesInfos& Get();

		template<class ClassInfoChildClass>
		void addClassInfo();

		std::shared_ptr<ClassInfo> getByClassName(std::string_view className);

		auto& get() { return internalClassInfos; }
		const auto& get() const { return internalClassInfos; }

	protected:

		std::vector<std::shared_ptr<ClassInfo>> internalClassInfos;

	};

	template<class ClassInfoChildClass>
	void ClassesInfos::addClassInfo()
	{
		internalClassInfos.push_back(std::make_shared<ClassInfoChildClass>());
		internalClassInfos.reserve(internalClassInfos.size() * 2u);
	}

}