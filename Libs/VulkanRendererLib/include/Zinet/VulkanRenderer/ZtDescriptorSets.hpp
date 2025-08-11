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

	class ZINET_VULKAN_RENDERER_API DescriptorSets : public VulkanObject<VkDescriptorSet>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRDescriptorSet");

	public:

		DescriptorSets(HandleType newObjectHandle) noexcept
			: VulkanObject(newObjectHandle) {
			if (newObjectHandle) 
				count = 1;
		}

		DescriptorSets() noexcept = delete;
		DescriptorSets(const DescriptorSets& other) noexcept = delete;
		DescriptorSets(DescriptorSets&& other) noexcept = default;
		~DescriptorSets() noexcept = default;

		DescriptorSets& operator = (const DescriptorSets& other) noexcept = delete;
		DescriptorSets& operator = (DescriptorSets&& other) noexcept = default;

		static VkDescriptorSetAllocateInfo GetDefaultAllocateInfo(
			const DescriptorPool& descriptorPool, const std::vector<DescriptorSetLayout::HandleType>& vkDescriptorSetLayouts) noexcept;

		static VkDescriptorSetAllocateInfo GetDefaultAllocateInfo(
			const DescriptorPool& descriptorPool, std::vector<DescriptorSetLayout::HandleType>&& descriptorSetLayouts) noexcept = delete;

		bool create(const Device& device, const VkDescriptorSetAllocateInfo& allocateInfo);

		void invalidate() noexcept { objectHandle = nullptr; }

		static VkWriteDescriptorSet GetDefaultWriteDescriptorSet() noexcept;

		static VkDescriptorBufferInfo GetBufferInfo(const Buffer& buffer) noexcept;

		static VkDescriptorImageInfo GetImageInfo(const ImageView& imageView, const Sampler& sampler) noexcept;

		void update(const Device& device, const std::vector<VkWriteDescriptorSet>& writeDescriptorSets) const noexcept;

		auto getCount() const noexcept { return count; }

	protected:

		uint32_t count = 0;

	};
}