#include "Zinet/VulkanRenderer/ZtDisplayImage.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtTexture.hpp"
#include "Zinet/VulkanRenderer/ZtSampler.hpp"

namespace zt::vulkan_renderer
{

	bool DisplayImage::create(RendererContext& rendererContext, VkImage swapChainImage)
	{
		auto& swapChain = rendererContext.swapChain;
		auto& device = rendererContext.device;
		auto& renderPass = rendererContext.renderPass;
		auto& commandPool = rendererContext.commandPool;

		image = swapChainImage;

		const auto imageViewCreateInfo = ImageView::GetDefaultCreateInfo(image, swapChain.getFormat());
		if (!imageView.create(device, imageViewCreateInfo))
		{
			Logger->error("Couldn't create an image view from one of the swap chain images");
			return false;
		}

		const auto swapChainSize = swapChain.getExtent();
		const Vector2ui framebufferSize{ swapChainSize.width, swapChainSize.height };
		if (!framebuffer.create(device, renderPass, imageView, framebufferSize))
		{
			Logger->error("Couldn't create framebuffer from one of the swap chain images");
			return false;
		}

		if (!fence.create(device, true))
			return false;

		if (!commandBuffer.create(device, commandPool))
			return false;

		const DescriptorPoolSizes poolSizes
		{
			VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		// Descriptors
		DescriptorSetLayout::Bindings pipelineBindings;
		DescriptorSetLayout::Bindings objectBindings;

		auto pipelineDescriptorInfo = DescriptorInfo{ {}, { {} } };
		createDescriptorData(pipelineBindings, pipelineDescriptorInfo);
		pipelineDescriptorSetLayout = createDescriptorSetLayout(device, pipelineBindings);

		auto drawCallDescriptorInfo = DescriptorInfo{ { {} }, {} };
		createDescriptorData(objectBindings, drawCallDescriptorInfo);
		objectDescriptorSetLayout = createDescriptorSetLayout(device, objectBindings);

		const auto descriptorPoolCreateInfo = DescriptorPool::GetDefaultCreateInfo(poolSizes);
		if (!descriptorPool.create(device, descriptorPoolCreateInfo))
			return false;

		std::vector<VkDescriptorSetLayout> vkDescriptorSetLayouts;
		pipelineDescriptorSet = createDescriptorSet(device, pipelineDescriptorSetLayout, vkDescriptorSetLayouts);
		objectDescriptorSet = createDescriptorSet(device, objectDescriptorSetLayout, vkDescriptorSetLayouts);

		//UpdateDescriptorSet(device, drawInfo.pipelineDescriptorInfo, pipelineDescriptorSet);

		return true;
	}

	void DisplayImage::destroy(RendererContext& rendererContext)
	{
		auto& device = rendererContext.device;

		image = nullptr;
		framebuffer.destroy(device);
		imageView.destroy(device);
		fence.destroy(device);
		commandBuffer.invalidate();
		descriptorPool.destroy(device);
		pipelineDescriptorSetLayout.destroy(device);
		pipelineDescriptorSet.invalidate();
		objectDescriptorSetLayout.destroy(device);
		objectDescriptorSet.invalidate();

		vkDescriptorSets.clear();
	}

	void DisplayImage::createDescriptorData(
		DescriptorSetLayout::Bindings& outBindings,
		DescriptorInfo& descriptorInfo) const
	{
		auto& uniformBuffer = descriptorInfo.uniformBuffers;
		if (!uniformBuffer.empty())
		{
			descriptorInfo.cachedUniformBuffersBinding = static_cast<uint32_t>(outBindings.size());
			uint32_t descriptorsCount = static_cast<uint32_t>(descriptorInfo.uniformBuffers.size());

			auto layoutBinding = DescriptorSetLayout::GetDefaultUniformLayoutBinding();
			layoutBinding.binding = descriptorInfo.cachedUniformBuffersBinding;
			layoutBinding.descriptorCount = descriptorsCount;
			outBindings.push_back(layoutBinding);
		}

		if (!descriptorInfo.texturesInfos.empty())
		{
			descriptorInfo.cachedTexturesBinding = static_cast<uint32_t>(outBindings.size());
			uint32_t descriptorsCount = static_cast<uint32_t>(descriptorInfo.texturesInfos.size());

			auto layoutBinding = DescriptorSetLayout::GetDefaultImageLayoutBinding();
			layoutBinding.binding = descriptorInfo.cachedTexturesBinding;
			layoutBinding.descriptorCount = descriptorsCount;
			outBindings.push_back(layoutBinding);
		}
	}

	DescriptorSetLayout DisplayImage::createDescriptorSetLayout(
		const Device& device, DescriptorSetLayout::Bindings& bindings)
	{
		if (bindings.empty())
			return DescriptorSetLayout{ nullptr };

		const auto descriptorSetLayoutCreateInfo = DescriptorSetLayout::GetDefaultCreateInfo(bindings);
		DescriptorSetLayout descriptorSetLayout{ nullptr };
		descriptorSetLayout.create(descriptorSetLayoutCreateInfo, device);

		return descriptorSetLayout;
	}

	DescriptorSets DisplayImage::createDescriptorSet(
		const Device& device,
		const DescriptorSetLayout& layout,
		std::vector<VkDescriptorSetLayout>& outLayouts)
	{
		DescriptorSets descriptorSet{ nullptr };

		if (layout.isValid())
		{
			const std::vector<VkDescriptorSetLayout> vkDescriptorSetLayouts{ layout.get() };
			outLayouts.push_back(layout.get());
			const auto allocateInfo = DescriptorSets::GetDefaultAllocateInfo(descriptorPool, vkDescriptorSetLayouts);
			if (descriptorSet.create(device, allocateInfo))
			{
				vkDescriptorSets.push_back(descriptorSet.get());
			}
		}

		return descriptorSet;
	}

	void DisplayImage::UpdateDescriptorSet(
		const Device& device, const DescriptorInfo& descriptorInfo, const DescriptorSets& descriptorSet)
	{
		std::vector<VkWriteDescriptorSet> writeDescriptorSets;
		std::vector<VkDescriptorBufferInfo> descriptorBuffersInfos;
		std::vector<VkDescriptorImageInfo> descriptorImagesInfos;

		for (const auto& uniformBufferInfo : descriptorInfo.uniformBuffers)
		{
			auto& uniformBuffer = uniformBufferInfo.uniformBuffer;
			if (!uniformBuffer || !uniformBuffer->isValid())
				continue;

			auto& descriptorBufferInfo = descriptorBuffersInfos.emplace_back(DescriptorSets::GetBufferInfo(*uniformBuffer));
			descriptorBufferInfo.offset = 0;
		}

		/// Write Descriptor for uniform buffers
		if (!descriptorInfo.uniformBuffers.empty())
		{
			auto& writeDescriptorSet = writeDescriptorSets.emplace_back(DescriptorSets::GetDefaultWriteDescriptorSet());
			writeDescriptorSet.dstSet = descriptorSet.get();
			writeDescriptorSet.dstBinding = descriptorInfo.cachedUniformBuffersBinding;
			writeDescriptorSet.dstArrayElement = 0;
			writeDescriptorSet.descriptorCount = static_cast<uint32_t>(descriptorBuffersInfos.size());
			writeDescriptorSet.pBufferInfo = descriptorBuffersInfos.data();
			writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		}

		for (auto& textureInfo : descriptorInfo.texturesInfos)
		{
			if (!textureInfo.texture || !textureInfo.texture->isValid() || !textureInfo.sampler || !textureInfo.sampler->isValid())
				continue;

			[[maybe_unused]]
			auto& imageDescriptorInfo = descriptorImagesInfos.emplace_back(DescriptorSets::GetImageInfo(textureInfo.texture->getImageView(), *textureInfo.sampler));
		}

		/// Write Descriptor for textures
		if (!descriptorInfo.texturesInfos.empty())
		{
			auto& writeDescriptorSet = writeDescriptorSets.emplace_back(DescriptorSets::GetDefaultWriteDescriptorSet());
			writeDescriptorSet.dstSet = descriptorSet.get();
			writeDescriptorSet.dstBinding = descriptorInfo.cachedTexturesBinding;
			writeDescriptorSet.dstArrayElement = 0;
			writeDescriptorSet.descriptorCount = static_cast<uint32_t>(descriptorImagesInfos.size());
			writeDescriptorSet.pImageInfo = descriptorImagesInfos.data();
			writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		}

		if (!writeDescriptorSets.empty())
			descriptorSet.update(device, writeDescriptorSets);
	}
}
