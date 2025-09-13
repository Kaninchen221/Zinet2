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
	struct DescriptorSetsUpdateData;

	class  DescriptorSets : public VulkanObject<VkDescriptorSet>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::vulkan_renderer::DescriptorSet");

	public:
		using VkDescriptorSetLayouts = std::vector<DescriptorSetLayout::HandleType>;
		using VkWriteDescriptorSets = std::vector<VkWriteDescriptorSet>;

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
			const DescriptorPool& descriptorPool, const VkDescriptorSetLayouts& vkDescriptorSetLayouts) noexcept;

		static VkDescriptorSetAllocateInfo GetDefaultAllocateInfo(
			const DescriptorPool& descriptorPool, VkDescriptorSetLayouts&& descriptorSetLayouts) noexcept = delete;

		bool create(const Device& device, const VkDescriptorSetAllocateInfo& allocateInfo);

		// TODO: Add free method (vkFreeDescriptorSets) destroy it without destroying the pool
		// And remove the invalidate method? So we never make a dangling handle

		void invalidate() noexcept { objectHandle = nullptr; }

		void update(const Device& device, const DescriptorSetsUpdateData& updateData) const noexcept;

		auto getCount() const noexcept { return count; }

	protected:

		uint32_t count = 0;

	};
}