#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtClassDefaultObjectRegistry.hpp"
#include "Zinet/Core/ZtObjectBase.hpp"

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

		Object() = default;
		Object(const Object& other) = default;
		Object(Object&& other) = default;
		~Object() ZINET_API_POST = default;

		Object& operator = (const Object& other) ZINET_API_POST = default;
		Object& operator = (Object&& other) ZINET_API_POST = default;

		virtual std::string asString() const ZINET_API_POST { return "Object"; }

		virtual void operator << ([[maybe_unused]] Archive& archive) ZINET_API_POST {}

		virtual bool serialize([[maybe_unused]] JsonArchive& archive) ZINET_API_POST { return true; }

		virtual bool deserialize([[maybe_unused]] JsonArchive& archive) ZINET_API_POST { return true; }

		virtual std::string getClassName() const ZINET_API_POST { return "zt::core::Object"; }

		void setDisplayName(std::string_view newDisplayName) ZINET_API_POST { displayName = newDisplayName; }
		const auto& getDisplayName() const ZINET_API_POST { return displayName; }

		bool isInspectable = true;
		virtual void imGui() ZINET_API_POST {}

	protected:

		std::string displayName;

	};
}

/// Not in "core" namespace beause used too often
namespace zt 
{
	template<class NodeT = core::Object>
	using ObjectHandle = std::shared_ptr<NodeT>;

	template<class NodeT = core::Object>
	using ObjectWeakHandle = std::weak_ptr<NodeT>;

	template<std::derived_from<core::Object> ObjectT>
	auto CreateObject(std::string name) ZINET_API_POST
	{
		static size_t Counter = 0;
		if (Counter > 0)
			name = fmt::format("{}_{}", name, Counter);
		++Counter;

		auto object = new ObjectT();
		object->setDisplayName(name);
		auto handle = ObjectHandle<ObjectT>{ object };
		return handle;
	}
}