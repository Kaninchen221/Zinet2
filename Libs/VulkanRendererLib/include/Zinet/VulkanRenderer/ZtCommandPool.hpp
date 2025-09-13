#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;
	class Queue;

	class  CommandPool : public VulkanObject<VkCommandPool>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRCommandPool");

	public:

		CommandPool(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		CommandPool() noexcept = delete;
		CommandPool(const CommandPool& other) noexcept = delete;
		CommandPool(CommandPool&& other) noexcept = default;
		~CommandPool() noexcept = default;

		CommandPool& operator = (const CommandPool& other) noexcept = delete;
		CommandPool& operator = (CommandPool&& other) noexcept = default;

		bool create(const Device& device, const Queue& queue);

		void destroy(const Device& device) noexcept;

	};
}