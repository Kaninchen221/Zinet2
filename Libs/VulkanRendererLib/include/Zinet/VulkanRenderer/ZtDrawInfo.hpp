#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVertex.hpp"
#include "Zinet/VulkanRenderer/ZtShaderType.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSetLayout.hpp"

#include "Zinet/Core/ZtFunction.hpp"

#include <span>
#include <map>

namespace zt::vulkan_renderer
{
	class Texture;
	class Sampler;
	class CommandBuffer;
	class DescriptorSets;
	class Buffer;
	class ImageView;
	class ShaderModule;

	struct  TextureInfo
	{
		Texture* texture{};
		Sampler* sampler{};
		ShaderType shaderType = ShaderType::Invalid;
	};

	// TODO: Refactor ~ move it to separate file
	struct DescriptorSetsUpdateData
	{
		std::vector<VkWriteDescriptorSet> writeDescriptorSets;
		std::vector<VkDescriptorBufferInfo> descriptorBuffersInfos;
		std::vector<VkDescriptorImageInfo> descriptorImagesInfos;
	};
	
	struct DescriptorInfo
	{
		using VkWriteDescriptorSets = std::vector<VkWriteDescriptorSet>;

		using Buffers = std::vector<Buffer*>;
		std::map<VkDescriptorType, Buffers> buffersPerType;

		std::vector<TextureInfo> texturesInfos;

		uint32_t cachedBuffersBinding = 0;
		uint32_t cachedTexturesBinding = 0;

		DescriptorSetLayout::Bindings createBindings();

		static VkWriteDescriptorSet GetDefaultWriteDescriptorSet() noexcept;

		static VkDescriptorBufferInfo GetBufferInfo(const Buffer& buffer) noexcept;

		static VkDescriptorImageInfo GetImageInfo(const ImageView& imageView, const Sampler& sampler) noexcept;

		DescriptorSetsUpdateData createDescriptorSetsUpdateData(const DescriptorSets& descriptorSets) const;

		// TODO: Test it
		// REALY TEST IT
		DescriptorInfo& operator += (const DescriptorInfo& other);
	};

	DescriptorInfo operator + (const DescriptorInfo& first, const DescriptorInfo& second);

	struct DrawInfo
	{
		using Vertices = std::vector<Vertex>;
		using Indices = std::vector<std::uint16_t>;

		ShaderModule* vertexShaderModule{};
		ShaderModule* fragmentShaderModule{};
		Buffer* vertexBuffer{};
		Buffer* indexBuffer{};
		uint32_t indexCount{};
		uint32_t instances = 1;

		DescriptorInfo objectDescriptorInfo;

		using AdditionalCommands = std::vector<core::Function<void, const CommandBuffer&>>;
		AdditionalCommands additionalCommands;
	};
}