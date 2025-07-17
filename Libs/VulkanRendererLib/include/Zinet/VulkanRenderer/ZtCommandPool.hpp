#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;
	class Queue;

	class ZINET_VULKAN_RENDERER_API CommandPool : public VulkanObject<VkCommandPool>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRCommandPool");

	public:

		CommandPool(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		CommandPool() ZINET_API_POST = delete;
		CommandPool(const CommandPool& other) ZINET_API_POST = delete;
		CommandPool(CommandPool&& other) ZINET_API_POST = default;
		~CommandPool() ZINET_API_POST = default;

		CommandPool& operator = (const CommandPool& other) ZINET_API_POST = delete;
		CommandPool& operator = (CommandPool&& other) ZINET_API_POST = default;

		bool create(const Device& device, const Queue& queue) ZINET_API_POST;

		void destroy(const Device& device) ZINET_API_POST;

	};
}