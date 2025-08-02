#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtClassDefaultObjectRegistry.hpp"
#include "Zinet/Core/ZtObjectBase.hpp"

#include <vector>
#include <memory>
#include <type_traits>

namespace zt::core
{
	class Archive;
	class JsonArchive;

	class ZINET_CORE_API Object : public ObjectBase
	{
	public:

		Object() = delete;
		Object(std::string_view newDisplaName) : displayName{ newDisplaName } {}
		Object(const Object& other) = default;
		Object(Object&& other) = default;
		~Object() ZINET_API_POST = default;

		Object& operator = (const Object& other) ZINET_API_POST = default;
		Object& operator = (Object&& other) ZINET_API_POST = default;

		virtual std::string asString() const ZINET_API_POST { return "Object"; }

		virtual void operator << ([[maybe_unused]] Archive& archive) ZINET_API_POST {}

		virtual bool serialize([[maybe_unused]] JsonArchive& archive) ZINET_API_POST { return true; }

		virtual bool deserialize([[maybe_unused]] JsonArchive& archive) ZINET_API_POST { return true; }

		virtual std::string getDisplayName() ZINET_API_POST { return displayName; }

		bool isInspectable = true;
		virtual void imGui() ZINET_API_POST {}

	protected:

		std::string displayName;

	};
}