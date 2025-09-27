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

	// TODO: Refactor it to some interface like VulkanObject but for more than one handle
	class  DescriptorSets
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::vulkan_renderer::DescriptorSet");

	public:
		using HandleType = VkDescriptorSet;
		using Handles = std::vector<HandleType>;

		using VkDescriptorSetLayouts = std::vector<DescriptorSetLayout::HandleType>;
		using VkWriteDescriptorSets = std::vector<VkWriteDescriptorSet>;

		DescriptorSets(const Handles& newHandles) noexcept
			: handles{ newHandles }
		{
			if (handles.empty())
			{
				handles = { nullptr };
			}
		}

		DescriptorSets() noexcept = default;
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

		bool isValid() const noexcept;

		operator bool() const noexcept { return isValid(); }

		HandleType& get(size_t index = 0) noexcept { return handles.at(index); }
		const HandleType& get(size_t index = 0) const noexcept { return handles.at(index); }

		HandleType* data() noexcept { return &handles.front(); }
		const HandleType* data() const noexcept { return &handles.front(); }

		// TODO: Add free method (vkFreeDescriptorSets) destroy it without destroying the pool
		// And remove the invalidate method? So we never make a dangling handle

		void invalidate() noexcept;

		void update(const Device& device, const DescriptorSetsUpdateData& updateData) const noexcept;

		auto getCount() const noexcept { return handles.size(); }

	protected:

		std::vector<VkDescriptorSet> handles = { nullptr };

	};
}