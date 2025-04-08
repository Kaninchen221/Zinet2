#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	template<class HandleT, bool CheckObjectHandle = true>
	class VulkanObject
	{
	public:

		inline static constexpr std::int32_t InvalidIndex = -1;

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