#pragma once

#include <limits>

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtDebug.hpp"

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
		inline static auto Logger = zt::core::ConsoleLogger::Create("zt::vulkan_renderer::VulkanObject");

	public:

		inline static constexpr std::uint32_t InvalidIndex = std::numeric_limits<uint32_t>::max();

		using HandleType = HandleT;

		VulkanObject(HandleType newObjectHandle) noexcept
			: objectHandle(newObjectHandle) {}

		VulkanObject() = delete;
		VulkanObject(const VulkanObject& other) = delete;
		VulkanObject(VulkanObject&& other) noexcept { *this = std::move(other); }
		~VulkanObject() noexcept;

		VulkanObject& operator = (const VulkanObject& other) = delete;
		VulkanObject& operator = (VulkanObject&& other) noexcept;

		const HandleType get() const noexcept { return objectHandle; }
		HandleType get() noexcept { return objectHandle; }

		bool isValid() const noexcept { return objectHandle != nullptr; }

		operator bool() const noexcept { return isValid(); }

		// TODO: Add a release method that will release the handle without destroying the object?

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
	inline void destroyAll(auto&& container, ArgsT&& ... args)
	{
		for (auto&& element : container)
		{
			element.destroy(std::forward<ArgsT>(args)...);
		}
	}

	template<class ResultContainerT>
	inline ResultContainerT VulkanObjectsToVkObjects(auto&& container)
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
			Ensure(false);
			try 
			{
				Logger->error("Object must be manually destroyed/invalidated before destructor call");
			}
			catch ([[maybe_unused]] std::exception& ex)
			{
				TerminateDebug();
			}
		}
	}

}