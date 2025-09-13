#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;

	class  DescriptorPool : public VulkanObject<VkDescriptorPool>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::vulkan_renderer::DescriptorPool");

	public:
		using DescriptorPoolSizes = std::vector<VkDescriptorPoolSize>;

		DescriptorPool(VulcanType newHandle) noexcept
			: VulkanObject(newHandle) {}

		DescriptorPool() noexcept = delete;
		DescriptorPool(const DescriptorPool& other) noexcept = delete;
		DescriptorPool(DescriptorPool&& other) noexcept = default;
		~DescriptorPool() noexcept = default;

		DescriptorPool& operator = (const DescriptorPool& other) noexcept = delete;
		DescriptorPool& operator = (DescriptorPool&& other) noexcept = default;

		static uint32_t GetDefaultMaxSets() noexcept { return 100; } 

		static DescriptorPoolSizes GetDefaultPoolSizes() noexcept;

		static VkDescriptorPoolCreateInfo GetDefaultCreateInfo(const std::vector<VkDescriptorPoolSize>& poolSizes) noexcept;
		static VkDescriptorPoolCreateInfo GetDefaultCreateInfo(const std::vector<VkDescriptorPoolSize>&& poolSizes) noexcept = delete;

		bool create(const Device& device, const VkDescriptorPoolCreateInfo& createInfo);

		void destroy(const Device& device) noexcept;

	};
}