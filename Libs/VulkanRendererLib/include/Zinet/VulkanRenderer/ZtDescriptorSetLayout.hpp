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

	class ZINET_VULKAN_RENDERER_API DescriptorSetLayout : public VulkanObject<VkDescriptorSetLayout>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRDescriptorSetLayout");

	public:

		DescriptorSetLayout(HandleType newObjectHandle) noexcept
			: VulkanObject(newObjectHandle) {}

		DescriptorSetLayout() noexcept = delete;
		DescriptorSetLayout(const DescriptorSetLayout& other) noexcept = delete;
		DescriptorSetLayout(DescriptorSetLayout&& other) noexcept = default;
		~DescriptorSetLayout() noexcept = default;

		DescriptorSetLayout& operator = (const DescriptorSetLayout& other) noexcept = delete;
		DescriptorSetLayout& operator = (DescriptorSetLayout&& other) noexcept = default;

		static VkDescriptorSetLayoutBinding GetDefaultUniformLayoutBinding() noexcept;
		static VkDescriptorSetLayoutBinding GetDefaultImageLayoutBinding() noexcept;

		using Bindings = std::vector<VkDescriptorSetLayoutBinding>;
		static VkDescriptorSetLayoutCreateInfo GetDefaultCreateInfo(const Bindings& bindings) noexcept;
		static VkDescriptorSetLayoutCreateInfo GetDefaultCreateInfo(Bindings&& bindings) noexcept = delete;

		bool create(const VkDescriptorSetLayoutCreateInfo& createInfo, const Device& device);

		void destroy(const Device& device) noexcept;

	};
}