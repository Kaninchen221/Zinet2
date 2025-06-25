#pragma once

#include <limits>
#include <cstdint>
#include <utility>

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::vulkan_renderer
{
	template<class HandleT>
	class VulkanObject;

	template<typename T>
	concept IsVulkanObjectT = requires {
		typename T::HandleType;
	}&&
	std::derived_from<T, VulkanObject<typename T::HandleType>>;

	template<class HandleT>
	class VulkanObject
	{
	public:

		// Not "std::numeric_limits<std::uint32_t>::max();" because MSVC see the "max()" part as the "max()" macro
		inline static constexpr std::uint32_t InvalidIndex = UINT32_MAX;

		using HandleType = HandleT;

		VulkanObject(HandleType newObjectHandle)
			: objectHandle(newObjectHandle) {}

		VulkanObject() noexcept = default;
		VulkanObject(const VulkanObject& other) = delete;
		VulkanObject(VulkanObject&& other) noexcept { *this = std::move(other); }
		~VulkanObject() noexcept;

		VulkanObject& operator = (const VulkanObject& other) = delete;
		VulkanObject& operator = (VulkanObject&& other) noexcept;

		const HandleType get() const noexcept { return objectHandle; }
		HandleType get() noexcept { return objectHandle; }

		bool isValid() const noexcept { return objectHandle != nullptr; }

	protected:

		HandleType objectHandle = nullptr;

	};

	inline void invalidateAll(auto&& container) noexcept
	{
		for (auto&& element : container)
		{
			element.invalidate();
		}
	}

	template<class ...ArgsT>
	inline void destroyAll(auto&& container, ArgsT&& ... args) noexcept
	{
		for (auto&& element : container)
		{
			element.destroy(std::forward<ArgsT>(args)...);
		}
	}

	template<class ResultContainerT>
	inline ResultContainerT VulkanObjectsToVkObjects(auto&& container) noexcept
	{
		ResultContainerT resultContainer;
		for (auto&& vulkanObject : container)
		{
			resultContainer.push_back(vulkanObject.get());
		}

		return resultContainer;
	}

	template<class HandleT>
	VulkanObject<HandleT>& VulkanObject<HandleT>::operator=(VulkanObject&& other) noexcept
	{
		if (isValid())
		{
			auto Logger = core::ConsoleLogger::Create("VulkanObject");
			Logger->error("Object must be manually destroyed/invalidated before move call");
		}

		objectHandle = other.objectHandle;
		other.objectHandle = nullptr;
		return *this;
	}

	template<class HandleT>
	VulkanObject<HandleT>::~VulkanObject() noexcept
	{
		if (isValid())
		{
			auto Logger = core::ConsoleLogger::Create("VulkanObject");
			Logger->error("Object must be manually destroyed/invalidated before destructor call");
		}
	}

}