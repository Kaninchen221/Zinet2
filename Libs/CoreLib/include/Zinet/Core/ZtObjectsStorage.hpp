#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include <memory_resource>

namespace zt::core
{
	class ObjectsStorage
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("zt::core::ObjectsStorage");

	public:

		using Objects = std::list<ObjectRefCounter>;

		ZINET_CORE_API ObjectsStorage() noexcept = default;
		ZINET_CORE_API ObjectsStorage(const ObjectsStorage& other) noexcept = delete;
		ZINET_CORE_API ObjectsStorage(ObjectsStorage&& other) noexcept = default;
		ZINET_CORE_API ~ObjectsStorage() noexcept = default;

		ZINET_CORE_API ObjectsStorage& operator = (const ObjectsStorage& other) noexcept = delete;
		ZINET_CORE_API ObjectsStorage& operator = (ObjectsStorage&& other) noexcept = default;

		template<std::derived_from<Object> ObjectT>
		ObjectHandle<ObjectT> createObject(const std::string_view displayName);

	protected:

		Objects objects{};

	};

	template<std::derived_from<Object> ObjectT> 
	ObjectHandle<ObjectT> ObjectsStorage::createObject(const std::string_view displayName)
	{
		Logger->info("Creating object with display name '{}'", displayName);

		auto& objectRefCount = objects.emplace_back();
		objectRefCount.create<ObjectT>(displayName);

		return ObjectHandle<ObjectT>(&objectRefCount);
	}

}