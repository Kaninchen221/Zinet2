#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtTexture.hpp"

namespace zt::vulkan_renderer
{
	bool GraphicsPipeline::create(GraphicsPipelineCreateInfo& createInfo)
	{
		if (isValid())
			return false;

		auto& rendererContext = createInfo.rendererContext;
		auto& device = rendererContext.getDevice();
		auto& swapChain = rendererContext.getSwapChain();
		auto& renderPass = rendererContext.getRenderPass();
		auto& shaderModules = createInfo.shaderModules;

		if (!createDescriptors(createInfo))
		{
			Logger->error("Couldn't create descriptors");
			return false;
		}

		// TODO: Use global descriptor set
		//auto& globalDescriptorSetLayout = rendererContext.globalDescriptorSetLayout;
		const std::vector<DescriptorSetLayout::HandleType>& vkDescriptorSetLayouts = 
		{
			//globalDescriptorSetLayout.get(),
			pipelineDescriptorSetLayout.get(),
			objectDescriptorSetLayout.get()
		};

		auto pipelineLayoutCreateInfo = PipelineLayout::GetDefaultCreateInfo(vkDescriptorSetLayouts);
		if (!pipelineLayout.create(device, pipelineLayoutCreateInfo))
			return false;

		const auto extent = swapChain.getExtent();
		const VkViewport viewport{ 0, 0, static_cast<float>(extent.width), static_cast<float>(extent.height) };
		const VkRect2D scissor{ { 0, 0 }, extent };
		if (!pipeline.create(device, pipelineLayout, renderPass, viewport, scissor, shaderModules, createInfo.createVertexInput))
			return false;

		return true;
	}

	void GraphicsPipeline::destroy(const RendererContext& rendererContext) noexcept
	{
		const auto& device = rendererContext.getDevice();
		const auto& descriptorPool = rendererContext.getDescriptorPool();

		pipeline.destroy(device);
		pipelineLayout.destroy(device);

		pipelineDescriptorSets.destroy(device, descriptorPool);
		pipelineDescriptorSetLayout.destroy(device);

		objectDescriptorSets.destroy(device, descriptorPool);
		objectDescriptorSetLayout.destroy(device);
	}

	void GraphicsPipeline::draw(
		[[maybe_unused]] RendererContext& rendererContext, const DrawInfo& drawInfo, CommandBuffer& commandBuffer)
	{
		vkDescriptorSets =
		{
			//rendererContext.globalDescriptorSet.get(), // TODO: Use global descriptor set
			pipelineDescriptorSets.get(rendererContext.getCurrentDisplayImageIndex()),
			objectDescriptorSets.get(rendererContext.getCurrentDisplayImageIndex())
		};

		vkCmdBindDescriptorSets(
			commandBuffer.get(),
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipelineLayout.get(),
			0, // firstSet
			static_cast<uint32_t>(vkDescriptorSets.size()),
			vkDescriptorSets.data(),
			0, // dynamicOffsetCount
			nullptr // pDynamicOffsets
		);

		// Vertex Buffer
		const std::array<VkBuffer, 1> vertexBuffers = { drawInfo.vertexBuffer->get() };
		const std::array<VkDeviceSize, 1> offsets = { 0 };
		vkCmdBindVertexBuffers(commandBuffer.get(), 0, 1, vertexBuffers.data(), offsets.data());
		
		// Draw with Index Buffer
		if (drawInfo.indexBuffer->isValid())
		{
			vkCmdBindIndexBuffer(commandBuffer.get(), drawInfo.indexBuffer->get(), 0, VK_INDEX_TYPE_UINT16);
			vkCmdDrawIndexed(commandBuffer.get(), drawInfo.indexCount, drawInfo.instances, 0, 0, 0);
		}
		else
		{
			commandBuffer.draw(static_cast<uint32_t>(drawInfo.vertexBuffer->getSize()), drawInfo.instances, 0, 0);
		}
	}

	bool GraphicsPipeline::isValid() const noexcept
	{
		// Ignore objects related to descriptors
		return
			pipeline.isValid() &&
			pipelineLayout.isValid() && 
			pipelineDescriptorSetLayout.isValid() &&
			pipelineDescriptorSets.isValid() && 
			objectDescriptorSetLayout.isValid() &&
			objectDescriptorSets.isValid();
	}

	bool GraphicsPipeline::createDescriptors(GraphicsPipelineCreateInfo& graphicsPipelineCreateInfo)
	{
		auto& rendererContext = graphicsPipelineCreateInfo.rendererContext;
		auto descriptorSetsCount = graphicsPipelineCreateInfo.descriptorSetsCount;
		auto& device = rendererContext.getDevice();
		auto& descriptorPool = rendererContext.getDescriptorPool();

		if (graphicsPipelineCreateInfo.descriptorInfos.size() != 2)
		{
			Logger->error("Descriptors Infos must have size of 2," 
				"one for pipeline descriptor set and second for object descriptor set");
		}

		{ // Pipeline descriptor set & layout
			auto& pipelineDescriptorInfo = graphicsPipelineCreateInfo.descriptorInfos[0];
			const auto bindings = pipelineDescriptorInfo.createBindings();
			const auto createInfo = DescriptorSetLayout::GetDefaultCreateInfo(bindings);
			if (!pipelineDescriptorSetLayout.create(createInfo, device))
				return false;
		
			const DescriptorSets::VkDescriptorSetLayouts vkDescriptorSetLayouts{ descriptorSetsCount, pipelineDescriptorSetLayout.get() };
		
			const auto allocateInfo = DescriptorSets::GetDefaultAllocateInfo(descriptorPool, vkDescriptorSetLayouts);
			if (!pipelineDescriptorSets.create(device, allocateInfo))
				return false;
		
			auto descriptorSetsUpdateData =
				pipelineDescriptorInfo.createDescriptorSetsUpdateData(pipelineDescriptorSets);
		
			pipelineDescriptorSets.update(device, descriptorSetsUpdateData);
		}
		
		{ // Object descriptor set & layout
			auto& objectDescriptorInfo = graphicsPipelineCreateInfo.descriptorInfos[1];
			const auto bindings = objectDescriptorInfo.createBindings();
			const auto createInfo = DescriptorSetLayout::GetDefaultCreateInfo(bindings);
			if (!objectDescriptorSetLayout.create(createInfo, device))
				return false;
		
			const DescriptorSets::VkDescriptorSetLayouts vkDescriptorSetLayouts = { descriptorSetsCount, objectDescriptorSetLayout.get() };
		
			const auto allocateInfo = DescriptorSets::GetDefaultAllocateInfo(descriptorPool, vkDescriptorSetLayouts);
			if (!objectDescriptorSets.create(device, allocateInfo))
				return false;
		
			auto descriptorSetsUpdateData =
				objectDescriptorInfo.createDescriptorSetsUpdateData(objectDescriptorSets);
		
			objectDescriptorSets.update(device, descriptorSetsUpdateData);
		}

		return true;
	}

}