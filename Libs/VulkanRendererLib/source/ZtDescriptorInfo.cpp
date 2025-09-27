#include "Zinet/VulkanRenderer/ZtDescriptorInfo.hpp"
#include "Zinet/VulkanRenderer/ZtSampler.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSets.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtTexture.hpp"

namespace zt::vulkan_renderer
{
	namespace
	{
		inline static auto Logger = core::ConsoleLogger::Create("DescriptorInfo");
	}

	BuffersPack& BuffersPack::operator+=(const BuffersPack& other)
	{
		for (auto& [descriptorType, buffers] : other.buffersPerType)
		{
			auto findResult = buffersPerType.find(descriptorType);
			if (findResult != buffersPerType.end())
			{
				findResult->second.append_range(buffers);
			}
			else
			{
				buffersPerType.insert({ descriptorType, buffers });
			}
		}

		return *this;
	}

	BuffersPack operator+(const BuffersPack& first, const BuffersPack& second)
	{
		auto result = first;
		result += second;
		return result;
	}

	DescriptorSetLayout::Bindings DescriptorInfo::createBindings()
	{
		DescriptorSetLayout::Bindings result;

		for (auto& buffersPack : buffersPacks)
		{
			for (auto& [descriptorType, buffers] : buffersPack.buffersPerType)
			{
				result.push_back(
					VkDescriptorSetLayoutBinding
					{
						.binding = buffersPack.binding,
						.descriptorType = descriptorType,
						.descriptorCount = static_cast<uint32_t>(buffers.size()),
						.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
						.pImmutableSamplers = nullptr
					}
				);
			}
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

		// Reserve to avoid reallocation
		writeDescriptorSets.reserve(descriptorSets.getCount());

		size_t buffersCount = 0;
		for (const auto& buffersPack : buffersPacks)
		{
			for (const auto& [descriptorType, buffers] : buffersPack.buffersPerType)
			{
				buffersCount += buffers.size();
			}
		}
		descriptorBuffersInfos.reserve(buffersCount);

		descriptorImagesInfos.reserve(texturesInfos.size());

		size_t buffersOffset = 0;
		for (auto& buffersPack : buffersPacks)
		{
			for (auto& [descriptorType, buffers] : buffersPack.buffersPerType)
			{
				for (const auto& buffer : buffers)
				{
					if (!buffer || !buffer->isValid())
					{
						Logger->critical("Found an invalid buffer");
						return {};
					}

#				if ZINET_DEBUG // Sanity check
					if (descriptorBuffersInfos.size() >= descriptorBuffersInfos.capacity())
					{
						Logger->error("We can't reallocate the container because the pointers in the writeDescriptorSets will be invalid");
						continue;
					}
#				endif

					auto& descriptorBufferInfo = descriptorBuffersInfos.emplace_back(GetBufferInfo(*buffer));
					descriptorBufferInfo.offset = 0;
				}

				/// Write Descriptor for buffers per descriptor set
				for (size_t descriptorSetIndex = 0; descriptorSetIndex < descriptorSets.getCount(); ++descriptorSetIndex)
				{
					if (!buffers.empty())
					{
						auto& writeDescriptorSet = writeDescriptorSets.emplace_back(GetDefaultWriteDescriptorSet());
						writeDescriptorSet.dstSet = descriptorSets.get(descriptorSetIndex);
						writeDescriptorSet.dstBinding = buffersPack.binding;
						writeDescriptorSet.dstArrayElement = 0;
						writeDescriptorSet.descriptorCount = static_cast<uint32_t>(buffers.size());
						writeDescriptorSet.pBufferInfo = &descriptorBuffersInfos[buffersOffset];
						writeDescriptorSet.descriptorType = descriptorType;
					}
				}

				buffersOffset += buffers.size();
			}
		}

		for (auto& textureInfo : texturesInfos)
		{
			if (!textureInfo.texture || !textureInfo.sampler)
				continue;

			if (!textureInfo.texture->isValid() || !textureInfo.sampler->isValid())
				continue;

			[[maybe_unused]]
			auto& imageDescriptorInfo = descriptorImagesInfos.emplace_back(GetImageInfo(textureInfo.texture->getImageView(), *textureInfo.sampler));
		}

		/// Write Descriptor for textures
		for (size_t descriptorSetIndex = 0; descriptorSetIndex < descriptorSets.getCount(); ++descriptorSetIndex)
		{
			if (!texturesInfos.empty())
			{
				auto& writeDescriptorSet = writeDescriptorSets.emplace_back(GetDefaultWriteDescriptorSet());
				writeDescriptorSet.dstSet = descriptorSets.get(descriptorSetIndex);
				writeDescriptorSet.dstBinding = cachedTexturesBinding;
				writeDescriptorSet.dstArrayElement = 0;
				writeDescriptorSet.descriptorCount = static_cast<uint32_t>(descriptorImagesInfos.size());
				writeDescriptorSet.pImageInfo = descriptorImagesInfos.data();
				writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			}
		}

		return result;
	}

	DescriptorInfo& DescriptorInfo::operator += (const DescriptorInfo& other)
	{
		buffersPacks.append_range(other.buffersPacks);
		texturesInfos.append_range(other.texturesInfos);
		return *this;
	}

	DescriptorInfo operator+(const DescriptorInfo& first, const DescriptorInfo& second)
	{
		auto result = first;
		result += second;
		return result;
	}

}
