#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSetLayout.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

#include <span>

namespace zt::vulkan_renderer
{
	class Device;
	class DescriptorPool;
	class DescriptorSetLayout;
	class Buffer;
	class ImageView;
	class Sampler;

	class ZINET_VULKAN_RENDERER_API DescriptorSet : public VulkanObject<VkDescriptorSet>
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
			const DescriptorPool& descriptorPool, const std::vector<DescriptorSetLayout::HandleType>& vkDescriptorSetLayouts) noexcept;

		static VkDescriptorSetAllocateInfo GetDefaultAllocateInfo(
			const DescriptorPool& descriptorPool, std::vector<DescriptorSetLayout::HandleType>&& descriptorSetLayouts) noexcept = delete;

		bool create(const Device& device, const VkDescriptorSetAllocateInfo& allocateInfo) noexcept;

		void invalidate() { objectHandle = nullptr; }

		static VkWriteDescriptorSet GetDefaultWriteDescriptorSet() noexcept;

		static VkDescriptorBufferInfo GetBufferInfo(const Buffer& buffer) noexcept;

		static VkDescriptorImageInfo GetImageInfo(const ImageView& imageView, const Sampler& sampler) noexcept;

		void update(const Device& device, const std::span<const VkWriteDescriptorSet> writeDescriptorSets) const noexcept;
	};
}