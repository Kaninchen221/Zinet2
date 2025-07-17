#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::vulkan_renderer
{
	class Device;

	class ZINET_VULKAN_RENDERER_API Semaphore : public VulkanObject<VkSemaphore>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VR_Semaphore");

	public:

		Semaphore(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		Semaphore() ZINET_API_POST = delete;
		Semaphore(const Semaphore& other) ZINET_API_POST = delete;
		Semaphore(Semaphore&& other) ZINET_API_POST = default;
		~Semaphore() ZINET_API_POST = default;

		Semaphore& operator = (const Semaphore& other) ZINET_API_POST = delete;
		Semaphore& operator = (Semaphore&& other) ZINET_API_POST = default;

		bool create(const Device& device) ZINET_API_POST;

		void destroy(const Device& device) ZINET_API_POST;

	};
}