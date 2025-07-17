#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::vulkan_renderer
{
	class Device;

	class ZINET_VULKAN_RENDERER_API Fence : public VulkanObject<VkFence>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VR_Fence");

	public:

		Fence(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		Fence() ZINET_API_POST = delete;
		Fence(const Fence& other) ZINET_API_POST = delete;
		Fence(Fence&& other) ZINET_API_POST = default;
		~Fence() ZINET_API_POST = default;

		Fence& operator = (const Fence& other) ZINET_API_POST = delete;
		Fence& operator = (Fence&& other) ZINET_API_POST = default;

		bool create(const Device& device, bool signaled = true) ZINET_API_POST;

		void destroy(const Device& device) ZINET_API_POST;

		bool wait(const Device& device, std::uint64_t timeout = UINT64_MAX) const ZINET_API_POST;

		bool isSignaled(const Device& device) const ZINET_API_POST;

		bool reset(const Device& device) ZINET_API_POST;

	};
}