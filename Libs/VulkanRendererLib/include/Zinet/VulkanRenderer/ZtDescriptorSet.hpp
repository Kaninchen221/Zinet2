#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSetLayout.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;
	class DescriptorPool;
	class DescriptorSetLayout;
	class Buffer;

	class ZINET_VULKAN_RENDERER_API DescriptorSet : public VulkanObject<VkDescriptorSet, false>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRDescriptorSet");

	public:

		DescriptorSet(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		DescriptorSet() noexcept = delete;
		DescriptorSet(const DescriptorSet& other) noexcept = delete;
		DescriptorSet(DescriptorSet&& other) noexcept = default;
		~DescriptorSet() noexcept = default;

		DescriptorSet& operator = (const DescriptorSet& other) noexcept = delete;
		DescriptorSet& operator = (DescriptorSet&& other) noexcept = default;

		static VkDescriptorSetAllocateInfo GetDefaultAllocateInfo(
			const DescriptorPool& descriptorPool, const std::vector<DescriptorSetLayout::HandleType>& descriptorSetLayouts) noexcept;

		static VkDescriptorSetAllocateInfo GetDefaultAllocateInfo(
			const DescriptorPool& descriptorPool, std::vector<DescriptorSetLayout::HandleType>&& descriptorSetLayouts) noexcept = delete;

		bool create(const Device& device, const VkDescriptorSetAllocateInfo& allocateInfo) noexcept;

		static VkWriteDescriptorSet GetDefaultWriteDescriptorSet() noexcept;

		static VkDescriptorBufferInfo GetBufferInfo(const Buffer& buffer) noexcept;

		void update(const Device& device, const VkWriteDescriptorSet& writeDescriptorSet) const noexcept;
		void update(const Device& device, VkWriteDescriptorSet&& writeDescriptorSet) const noexcept = delete;
	};
}