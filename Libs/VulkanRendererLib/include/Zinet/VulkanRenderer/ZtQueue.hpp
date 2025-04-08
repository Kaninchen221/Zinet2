#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class ZINET_VULKAN_RENDERER_API Queue : public VulkanObject<VkQueue, false>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRQueue");

	public:

		Queue(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		Queue() noexcept = delete;
		Queue(const Queue& other) noexcept = delete;
		Queue(Queue&& other) noexcept = default;
		~Queue() noexcept = default;

		Queue& operator = (const Queue& other) noexcept = delete;
		Queue& operator = (Queue&& other) noexcept = default;
	};
}