#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "Zinet/Core/ZtFile.hpp"
#include "Zinet/Core/ZtImage.hpp"

#include "Zinet/VulkanRenderer/ZtQueueUtils.hpp"

#include <imgui.h>

#include <fmt/format.h>

#include "imgui_impl_vulkan.h"

namespace zt::vulkan_renderer
{
	class CommandBuffer;
}

namespace zt::gameplay::assets
{
	bool AssetTexture::load([[maybe_unused]] const core::Path& rootPath) ZINET_API_POST
	{
		if (isLoaded())
			return true;

		core::File file;
		const auto filePath = rootPath / metaData.value("fileRelativePath", "");
		file.open(filePath, core::FileOpenMode::Read, true);
		if (!file.isOpen())
		{
			Logger->error("Couldn't open file, path: {}", filePath.generic_string());
			return false;
		}

		if (!file.isOkay())
		{
			Logger->error("Something went wrong during reading the file");
			return false;
		}

		const auto rawData = file.readData();
		file.close();

		core::Image image;
		if (!image.loadFromData(rawData, 4))
		{
			Logger->error("Couldn't load an image from a data");
			return false;
		}

		auto& engineContext = EngineContext::Get();
		auto& rendererContext = engineContext.systemRenderer.getRenderer().getRendererContext();
		auto& device = rendererContext.device;
		auto& vma = rendererContext.vma;
		auto& queue = rendererContext.queue;
		auto& commandPool = rendererContext.commandPool;
		Vector2ui size = { image.getWidth(), image.getHeight() };
		if (!texture.create(device, vma, size))
		{
			Logger->error("Couldn't create a texture from an image");
			return false;
		}
		auto textureImageFileName = fmt::format("Texture {} image", metaData.value("fileName", "TextureImageFileName"));
		device.setDebugName(texture.getImage(), textureImageFileName, VK_OBJECT_TYPE_IMAGE);

		auto textureImageViewFileName = fmt::format("Texture {} image view", metaData.value("fileName", "TextureImageFileName"));
		device.setDebugName(texture.getImageView(), textureImageViewFileName, VK_OBJECT_TYPE_IMAGE_VIEW);
		
		auto samplerCreateInfo = vulkan_renderer::Sampler::GetDefaultCreateInfo();
		if (!sampler.create(device, samplerCreateInfo))
		{
			Logger->error("Couldn't create a sampler");
			return false;
		}

		vulkan_renderer::Buffer buffer{ nullptr };
		const auto bufferCreateInfo = vulkan_renderer::Buffer::GetImageBufferCreateInfo(image);
		if (!buffer.createBuffer(bufferCreateInfo, vma))
		{
			Logger->error("Couldn't create an image buffer");
			return false;
		}

		if (!buffer.fillWithImage(image, vma))
		{
			Logger->error("Filling a buffer with an image failed");
			buffer.destroy(vma);
			return false;
		}

		const auto commands = [&texture = texture, &image = image, &buffer = buffer]
		(const vulkan_renderer::CommandBuffer& commandBuffer)
		{
			vulkan_renderer::FillWithImageBufferInput input
			{
				.buffer = buffer,
				.commandBuffer = commandBuffer,
				.imageExtent = { image.getWidth(), image.getHeight() }
			};
			texture.fillWithImageBuffer(input);
		};
		if (!vulkan_renderer::SubmitSingleCommandBufferWaitIdle(device, queue, commandPool, commands))
		{
			Logger->error("Filling a texture witn an buffer failed");
			buffer.destroy(vma);
			return false;
		}
		buffer.destroy(vma);

		auto vkDescriptorSet = ImGui_ImplVulkan_AddTexture(
			sampler.get(),
			texture.getImageView().get(),
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		);

		descriptorSet = vulkan_renderer::DescriptorSets{ vkDescriptorSet };

		loaded = true;
		return true;
	}

	void AssetTexture::unload() ZINET_API_POST
	{
		if (!isLoaded())
			return;

		auto& engineContext = EngineContext::Get();
		auto& rendererContext = engineContext.systemRenderer.getRenderer().getRendererContext();
		auto& device = rendererContext.device;
		auto& vma = rendererContext.vma;
		device.waitIdle();

		descriptorSet.invalidate();
		texture.destroy(device, vma);
		sampler.destroy(device);
		loaded = false;
	}

	void AssetTexture::imGuiAssetInspect() ZINET_API_POST
	{
		Asset::imGuiAssetInspect();
		if (!isLoaded())
			return;
		
		if (!texture.isValid())
			return;

		ImGui::Separator();
		ImGui::Text("Texture image:");
		ImVec2 size = ImVec2(ImGui::GetContentRegionAvail());
		ImGui::Image((ImTextureID)descriptorSet.get(), size);
	}

}

