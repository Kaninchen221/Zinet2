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

		DescriptorSets(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {
			if (newObjectHandle) 
				count = 1;
		}

		DescriptorSets() ZINET_API_POST = delete;
		DescriptorSets(const DescriptorSets& other) ZINET_API_POST = delete;
		DescriptorSets(DescriptorSets&& other) ZINET_API_POST = default;
		~DescriptorSets() ZINET_API_POST = default;

		DescriptorSets& operator = (const DescriptorSets& other) ZINET_API_POST = delete;
		DescriptorSets& operator = (DescriptorSets&& other) ZINET_API_POST = default;

		static VkDescriptorSetAllocateInfo GetDefaultAllocateInfo(
			const DescriptorPool& descriptorPool, const std::vector<DescriptorSetLayout::HandleType>& vkDescriptorSetLayouts) ZINET_API_POST;

		static VkDescriptorSetAllocateInfo GetDefaultAllocateInfo(
			const DescriptorPool& descriptorPool, std::vector<DescriptorSetLayout::HandleType>&& descriptorSetLayouts) ZINET_API_POST = delete;

		bool create(const Device& device, const VkDescriptorSetAllocateInfo& allocateInfo) ZINET_API_POST;

		void invalidate() { objectHandle = nullptr; }

		static VkWriteDescriptorSet GetDefaultWriteDescriptorSet() ZINET_API_POST;

		static VkDescriptorBufferInfo GetBufferInfo(const Buffer& buffer) ZINET_API_POST;

		static VkDescriptorImageInfo GetImageInfo(const ImageView& imageView, const Sampler& sampler) ZINET_API_POST;

		void update(const Device& device, const std::vector<VkWriteDescriptorSet>& writeDescriptorSets) const ZINET_API_POST;

		auto getCount() const ZINET_API_POST { return count; }

	protected:

		uint32_t count = 0;

	};
}