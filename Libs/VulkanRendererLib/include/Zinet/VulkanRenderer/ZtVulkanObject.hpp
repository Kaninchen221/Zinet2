#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	template<class HandleT>
	class VulkanObject
	{
	public:

		using HandleType = HandleT;

		VulkanObject() noexcept = default;
		VulkanObject(const VulkanObject& other) = delete;
		VulkanObject(VulkanObject&& other) noexcept = default;
		~VulkanObject() noexcept;

		VulkanObject& operator = (const VulkanObject& other) = delete;
		VulkanObject& operator = (VulkanObject&& other) noexcept = default;

		const HandleType get() const noexcept { return objectHandle; }
		HandleType get() noexcept { return objectHandle; }

		bool isValid() const noexcept { return objectHandle != nullptr; }

	protected:

		HandleType objectHandle = nullptr;

	};

	template<class HandleT>
	VulkanObject<HandleT>::~VulkanObject() noexcept
	{
		if (isValid())
		{
			auto Logger = core::ConsoleLogger::Create("VulkanObject");
			Logger->error("Object must be manually destroyed before destructor call");
		}
	}

}