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

		Fence() noexcept = delete;
		Fence(const Fence& other) noexcept = delete;
		Fence(Fence&& other) noexcept = default;
		~Fence() noexcept = default;

		Fence& operator = (const Fence& other) noexcept = delete;
		Fence& operator = (Fence&& other) noexcept = default;

		bool create(const Device& device, bool signaled = true) noexcept;

		void destroy(const Device& device) noexcept;

		bool wait(const Device& device, std::uint64_t timeout = UINT64_MAX) const noexcept;

		bool isSignaled(const Device& device) const noexcept;

		bool reset(const Device& device) noexcept;

	};
}