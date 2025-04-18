#pragma once

#include <limits>
#include <cstdint>
#include <utility>

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::vulkan_renderer
{
	template<class HandleT, bool CheckObjectHandle = true>
	class VulkanObject
	{
	public:

		// Not "std::numeric_limits<std::uint32_t>::max();" because MSVC see "max()" as macro
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

	template<class HandleT, bool CheckObjectHandle>
	VulkanObject<HandleT, CheckObjectHandle>& VulkanObject<HandleT, CheckObjectHandle>::operator=(VulkanObject&& other) noexcept
	{
		if constexpr (CheckObjectHandle)
		{
			if (isValid())
			{
				auto Logger = core::ConsoleLogger::Create("VulkanObject");
				Logger->error("Object must be manually destroyed before move call");
			}
		}

		objectHandle = other.objectHandle;
		other.objectHandle = nullptr;
		return *this;
	}

	template<class HandleT, bool CheckObjectHandle>
	VulkanObject<HandleT, CheckObjectHandle>::~VulkanObject() noexcept
	{
		if constexpr (CheckObjectHandle)
		{
			if (isValid())
			{
				auto Logger = core::ConsoleLogger::Create("VulkanObject");
				Logger->error("Object must be manually destroyed before destructor call");
			}
		}
	}

}