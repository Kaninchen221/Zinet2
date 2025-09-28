#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVertex.hpp"
#include "Zinet/VulkanRenderer/ZtShaderType.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSetLayout.hpp"
#include "Zinet/VulkanRenderer/ZtTextureInfo.hpp"

#include "Zinet/Core/ZtFunction.hpp"

#include <span>
#include <map>

namespace zt::vulkan_renderer
{
	class Texture;
	class Sampler;
	class DescriptorSets;
	class Buffer;
	class ImageView;

	struct DescriptorSetsUpdateData
	{
		std::vector<VkWriteDescriptorSet> writeDescriptorSets;
		std::vector<VkDescriptorBufferInfo> descriptorBuffersInfos;
		std::vector<VkDescriptorImageInfo> descriptorImagesInfos;
	};

	struct BuffersPack
	{
		using Buffers = std::vector<Buffer*>;

		uint32_t binding = 0;
		std::map<VkDescriptorType, Buffers> buffersPerType;

		BuffersPack& operator += (const BuffersPack& other);
	};

	BuffersPack operator + (const BuffersPack& first, const BuffersPack& second);

	// TODO: REALY TEST IT
	struct DescriptorInfo
	{
		std::vector<BuffersPack> buffersPacks;
		std::vector<TextureInfo> texturesInfos;

		uint32_t cachedTexturesBinding = 0;

		DescriptorSetLayout::Bindings createBindings();

		static VkWriteDescriptorSet GetDefaultWriteDescriptorSet() noexcept;

		static VkDescriptorBufferInfo GetBufferInfo(const Buffer& buffer) noexcept;

		static VkDescriptorImageInfo GetImageInfo(const ImageView& imageView, const Sampler& sampler) noexcept;

		DescriptorSetsUpdateData createDescriptorSetsUpdateData(const DescriptorSets& descriptorSets) const;

		DescriptorInfo& operator += (const DescriptorInfo& other);
	};

	DescriptorInfo operator + (const DescriptorInfo& first, const DescriptorInfo& second);
}