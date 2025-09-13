#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;

	class  DescriptorSetLayout : public VulkanObject<VkDescriptorSetLayout>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::vulkan_renderer::DescriptorSetLayout");

	public:
		using Bindings = std::vector<VkDescriptorSetLayoutBinding>;

		DescriptorSetLayout(VulcanType newHandle) noexcept
			: VulkanObject(newHandle) {}

		DescriptorSetLayout() noexcept = delete;
		DescriptorSetLayout(const DescriptorSetLayout& other) noexcept = delete;
		DescriptorSetLayout(DescriptorSetLayout&& other) noexcept = default;
		~DescriptorSetLayout() noexcept = default;

		DescriptorSetLayout& operator = (const DescriptorSetLayout& other) noexcept = delete;
		DescriptorSetLayout& operator = (DescriptorSetLayout&& other) noexcept = default;

		static VkDescriptorSetLayoutBinding GetDefaultUniformLayoutBinding() noexcept;
		static VkDescriptorSetLayoutBinding GetDefaultImageLayoutBinding() noexcept;

		static VkDescriptorSetLayoutCreateInfo GetDefaultCreateInfo(const Bindings& bindings) noexcept;
		static VkDescriptorSetLayoutCreateInfo GetDefaultCreateInfo(Bindings&& bindings) noexcept = delete;

		bool create(const VkDescriptorSetLayoutCreateInfo& createInfo, const Device& device);

		void destroy(const Device& device) noexcept;

	};
}