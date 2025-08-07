#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtClassDefaultObjectRegistry.hpp"
#include "Zinet/Core/ZtObjectBase.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include <vector>
#include <memory>
#include <type_traits>

#include <fmt/format.h>

namespace zt::core
{
	class Archive;
	class JsonArchive;

	class ZINET_CORE_API Object : public ObjectBase
	{
	public:

		using ObjectPtr = std::shared_ptr<Object>;

		Object() = default;
		Object(const Object& other) = default;
		Object(Object&& other) = default;
		~Object() ZINET_API_POST = default;

		Object& operator = (const Object& other) ZINET_API_POST = default;
		Object& operator = (Object&& other) ZINET_API_POST = default;

		virtual ObjectPtr createCopy() const ZINET_API_POST { return {}; }

		virtual std::string asString() const ZINET_API_POST { return "Object"; }

		virtual void operator << ([[maybe_unused]] Archive& archive) ZINET_API_POST {}

		virtual bool serialize([[maybe_unused]] JsonArchive& archive) ZINET_API_POST { return true; }

		virtual bool deserialize([[maybe_unused]] JsonArchive& archive) ZINET_API_POST { return true; }

		virtual std::string getClassName() const ZINET_API_POST { return "zt::core::Object"; }

		void setDisplayName(const std::string_view newDisplayName) ZINET_API_POST { displayName = newDisplayName; }
		const auto& getDisplayName() const ZINET_API_POST { return displayName; }

		bool isInspectable = true;
		virtual void imGui() ZINET_API_POST {}

		bool isSaveable = false;

	protected:

		std::string displayName;

	};
}

/// Not in "core" namespace because used too often
namespace zt 
{
	template<class NodeT = core::Object>
	using ObjectHandle = std::shared_ptr<NodeT>;

	template<class NodeT = core::Object>
	using ObjectWeakHandle = std::weak_ptr<NodeT>;

	// TODO: Don't use std smart pointers in code
	template<std::derived_from<core::Object> ObjectT>
	auto CreateObject(const std::string& name) ZINET_API_POST
	{
		auto object = std::make_shared<ObjectT>();
		if (!object)
		{
			auto logger = core::ConsoleLogger::Create("CreateObject");
			logger->error("Couldn't create an object, name: {}", name);
			return ObjectHandle<ObjectT>{};
		}

		object->setDisplayName(name);
		return object;
	}
}