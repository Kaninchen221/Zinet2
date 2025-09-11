#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtSampler.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSets.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtTexture.hpp"

namespace zt::vulkan_renderer
{
	DescriptorSetLayout::Bindings DescriptorInfo::createBindings()
	{
		DescriptorSetLayout::Bindings result;

		if (!uniformBuffers.empty())
		{
			cachedUniformBuffersBinding = static_cast<uint32_t>(result.size());
			result.push_back(
				VkDescriptorSetLayoutBinding
				{
					.binding = cachedUniformBuffersBinding,
					.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
					.descriptorCount = static_cast<uint32_t>(uniformBuffers.size()),
					.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
					.pImmutableSamplers = nullptr
				}
			);
		}

		if (!texturesInfos.empty())
		{
			cachedTexturesBinding = static_cast<uint32_t>(result.size());
			result.push_back(
				VkDescriptorSetLayoutBinding
				{
					.binding = cachedTexturesBinding,
					.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
					.descriptorCount = static_cast<uint32_t>(texturesInfos.size()),
					.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
					.pImmutableSamplers = nullptr
				}
			);
		}

		return result;
	}

	VkWriteDescriptorSet DescriptorInfo::GetDefaultWriteDescriptorSet() noexcept
	{
		return VkWriteDescriptorSet
		{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.pNext = nullptr,
			.dstSet = nullptr,
			.dstBinding = 0,
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.pImageInfo = nullptr,
			.pBufferInfo = nullptr,
			.pTexelBufferView = nullptr
		};
	}

	VkDescriptorBufferInfo DescriptorInfo::GetBufferInfo(const Buffer& buffer) noexcept
	{
		return
		{
			.buffer = buffer.get(),
			.offset = 0,
			.range = buffer.getSize()
		};
	}

	VkDescriptorImageInfo DescriptorInfo::GetImageInfo(const ImageView& imageView, const Sampler& sampler) noexcept
	{
		return
		{
			.sampler = sampler.get(),
			.imageView = imageView.get(),
			.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL // TODO: Refactor, image view or/and image should remember layout
		};
	}

	DescriptorSetsUpdateData DescriptorInfo::createDescriptorSetsUpdateData(const DescriptorSets& descriptorSets) const
	{
		DescriptorSetsUpdateData result;

		auto& writeDescriptorSets = result.writeDescriptorSets;
		auto& descriptorBuffersInfos = result.descriptorBuffersInfos;
		auto& descriptorImagesInfos = result.descriptorImagesInfos;

		for (const auto& uniformBufferInfo : uniformBuffers)
		{
			auto& uniformBuffer = uniformBufferInfo.uniformBuffer;
			if (!uniformBuffer || !uniformBuffer->isValid())
				continue;

			auto& descriptorBufferInfo = descriptorBuffersInfos.emplace_back(GetBufferInfo(*uniformBuffer));
			descriptorBufferInfo.offset = 0;
		}

		/// Write Descriptor for uniform buffers
		if (!uniformBuffers.empty())
		{
			auto& writeDescriptorSet = writeDescriptorSets.emplace_back(GetDefaultWriteDescriptorSet());
			writeDescriptorSet.dstSet = descriptorSets.get();
			writeDescriptorSet.dstBinding = cachedUniformBuffersBinding;
			writeDescriptorSet.dstArrayElement = 0;
			writeDescriptorSet.descriptorCount = static_cast<uint32_t>(descriptorBuffersInfos.size());
			writeDescriptorSet.pBufferInfo = descriptorBuffersInfos.data();
			writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		}

		for (auto& textureInfo : texturesInfos)
		{
			if (!textureInfo.texture || !textureInfo.texture->isValid() || !textureInfo.sampler || !textureInfo.sampler->isValid())
				continue;

			[[maybe_unused]]
			auto& imageDescriptorInfo = descriptorImagesInfos.emplace_back(GetImageInfo(textureInfo.texture->getImageView(), *textureInfo.sampler));
		}

		/// Write Descriptor for textures
		if (!texturesInfos.empty())
		{
			auto& writeDescriptorSet = writeDescriptorSets.emplace_back(GetDefaultWriteDescriptorSet());
			writeDescriptorSet.dstSet = descriptorSets.get();
			writeDescriptorSet.dstBinding = cachedTexturesBinding;
			writeDescriptorSet.dstArrayElement = 0;
			writeDescriptorSet.descriptorCount = static_cast<uint32_t>(descriptorImagesInfos.size());
			writeDescriptorSet.pImageInfo = descriptorImagesInfos.data();
			writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		}

		return result;
	}

	DescriptorInfo& DescriptorInfo::operator += (const DescriptorInfo& other)
	{
		uniformBuffers.append_range(other.uniformBuffers);
		texturesInfos.append_range(other.texturesInfos);
		return *this;
	}
}
