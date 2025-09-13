#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::vulkan_renderer
{
	class Device;

	class  Semaphore : public VulkanObject<VkSemaphore>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VR_Semaphore");

	public:

		Semaphore(VulcanType newHandle)
			: VulkanObject(newHandle) {}

		Semaphore() noexcept = delete;
		Semaphore(const Semaphore& other) noexcept = delete;
		Semaphore(Semaphore&& other) noexcept = default;
		~Semaphore() noexcept = default;

		Semaphore& operator = (const Semaphore& other) noexcept = delete;
		Semaphore& operator = (Semaphore&& other) noexcept = default;

		bool create(const Device& device);

		void destroy(const Device& device) noexcept;

	};
}