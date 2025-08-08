#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include <memory_resource>

namespace zt::core
{
	class ZINET_CORE_API ObjectsStorage
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("zt::core::ObjectsStorage");

	public:

		using Objects = std::list<ObjectRefCounter>;

		ObjectsStorage() ZINET_API_POST = default;
		ObjectsStorage(const ObjectsStorage& other) ZINET_API_POST = delete;
		ObjectsStorage(ObjectsStorage&& other) ZINET_API_POST = default;
		~ObjectsStorage() noexcept = default;

		ObjectsStorage& operator = (const ObjectsStorage& other) ZINET_API_POST = delete;
		ObjectsStorage& operator = (ObjectsStorage&& other) ZINET_API_POST = default;

		template<std::derived_from<Object> ObjectT>
		ObjectHandle<ObjectT> createObject(const std::string_view displayName) ZINET_API_POST;

	protected:

		Objects objects{};

	};

	template<std::derived_from<Object> ObjectT> 
	ObjectHandle<ObjectT> ObjectsStorage::createObject(const std::string_view displayName) ZINET_API_POST
	{
		Logger->info("Creating object with display name '{}'", displayName);

		auto& objectRefCount = objects.emplace_back();
		objectRefCount.create<ObjectT>(displayName);

		return ObjectHandle<ObjectT>(&objectRefCount);
	}

}