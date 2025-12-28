#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObjectBase.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtDebug.hpp"
#include "Zinet/Core/ZtObjectHandle.hpp"

#include <vector>
#include <memory>
#include <type_traits>
#include <utility>

namespace zt::core
{
	class Object;
	class Archive;
	class JsonArchive;

	// TODO: Review and refactor this class
	class Object : public ObjectBase
	{
		ObjectWeakHandle<Object> self;

	public:

		using ObjectPtr = std::unique_ptr<Object>;

		Object() = default;
		Object(const std::string& newDisplayName) : displayName{ newDisplayName } {}
		Object(const Object& other) = default;
		Object(Object&& other) noexcept = default;
		~Object() noexcept = default;

		Object& operator = (const Object& other) = default;
		Object& operator = (Object&& other) noexcept = default;

		ObjectWeakHandle<Object> getSelf() const noexcept { return self; }

		virtual void onCreate(ObjectWeakHandle<Object> newSelf) { self = newSelf; }

		virtual void onDestroy() {}

		virtual ObjectPtr createCopy() const { return {}; }

		virtual std::string asString() const { return "Object"; }

		virtual void operator << ([[maybe_unused]] Archive& archive) {}

		virtual bool serialize([[maybe_unused]] JsonArchive& archive) { return true; }

		virtual bool deserialize([[maybe_unused]] JsonArchive& archive) { return true; }

		virtual const std::string_view getClassName() const { return "zt::core::Object"; }

		void setDisplayName(const std::string_view newDisplayName) { displayName = newDisplayName; }
		const auto& getDisplayName() const { return displayName; }

		void setInspectable(bool value) noexcept { inspectable = value; }
		bool isInspectable() const noexcept { return inspectable; }

		void setSaveable(bool value) noexcept { saveable = value; }
		bool isSaveable() const noexcept { return saveable; }

		virtual void show() {}

	protected:

		bool inspectable = true;
		bool saveable = false;
		std::string displayName{};

	};
}