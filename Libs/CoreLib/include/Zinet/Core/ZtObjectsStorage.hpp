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

		ObjectsStorage() noexcept = default;
		ObjectsStorage(const ObjectsStorage& other) noexcept = delete;
		ObjectsStorage(ObjectsStorage&& other) noexcept = default;
		~ObjectsStorage() noexcept = default;

		ObjectsStorage& operator = (const ObjectsStorage& other) noexcept = delete;
		ObjectsStorage& operator = (ObjectsStorage&& other) noexcept = default;

		template<std::derived_from<Object> ObjectT>
		ObjectHandle<ObjectT> createObject(const std::string_view displayName);

	protected:

		struct {
			Objects objects{};
		} data;
		Objects& objects = data.objects;

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