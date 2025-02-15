#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObjectBase.hpp"

#include <vector>
#include <memory>

namespace zt::core
{

	class ZINET_CORE_API ClassDefaultObjectRegistry
	{

	public:
		ClassDefaultObjectRegistry(const ClassDefaultObjectRegistry& other) = delete;
		ClassDefaultObjectRegistry(ClassDefaultObjectRegistry&& other) = delete;

		ClassDefaultObjectRegistry& operator = (const ClassDefaultObjectRegistry& other) = delete;
		ClassDefaultObjectRegistry& operator = (ClassDefaultObjectRegistry&& other) = delete;

		static ClassDefaultObjectRegistry& Get();

		void registerClass(ObjectBase* object);

		const auto& getCDOs() const { return CDOList; }

		std::unique_ptr<ObjectBase> createObjectByClassName(const std::string& name) const;

	private:

		ClassDefaultObjectRegistry() = default;
		~ClassDefaultObjectRegistry() noexcept = default;

		std::vector<std::unique_ptr<ObjectBase>> CDOList;

	};

}