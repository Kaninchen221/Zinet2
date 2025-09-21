#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "Zinet/Core/ZtFile.hpp"
#include "Zinet/Core/ZtImage.hpp"
#include "Zinet/Core/ZtImgui.hpp"

#include "Zinet/VulkanRenderer/ZtQueueUtils.hpp"

#include "imgui_impl_vulkan.h"

namespace zt::vulkan_renderer
{
	class CommandBuffer;
}

namespace zt::gameplay
{
	bool AssetTexture::load(const core::Path& rootPath)
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
		auto systemRenderer = engineContext.getSystem<SystemRenderer>();
		if (!systemRenderer)
		{
			Logger->error("System renderer is invalid");
			return false;
		}

		auto& rendererContext = systemRenderer->getRenderer().getRendererContext();
		auto& device = rendererContext.getDevice();
		auto& vma = rendererContext.getVMA();
		auto& queue = rendererContext.getQueue();
		auto& commandPool = rendererContext.getCommandPool();
		const Vector2ui size = { image.getWidth(), image.getHeight() };
		if (!texture.create(device, vma, size))
		{
			Logger->error("Couldn't create a texture from an image");
			return false;
		}
		auto textureImageFileName = fmt::format("Texture {} image", metaData.value("fileRelativePath", "fileRelativePath"));
		device.setDebugName(texture.getImage(), textureImageFileName, VK_OBJECT_TYPE_IMAGE);

		auto textureImageViewFileName = fmt::format("Texture {} image view", metaData.value("fileRelativePath", "fileRelativePath"));
		device.setDebugName(texture.getImageView(), textureImageViewFileName, VK_OBJECT_TYPE_IMAGE_VIEW);

		vulkan_renderer::Buffer buffer{ nullptr };
		const auto bufferCreateInfo = vulkan_renderer::Buffer::GetImageBufferCreateInfo(image);
		if (!buffer.createBuffer(vma, bufferCreateInfo))
		{
			Logger->error("Couldn't create an image buffer");
			return false;
		}

		if (!buffer.fillWithImage(vma, image))
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
			Logger->error("Filling a texture with an buffer failed");
			buffer.destroy(vma);
			return false;
		}
		buffer.destroy(vma);

		loaded = true;
		return true;
	}

	void AssetTexture::unload()
	{
		if (!isLoaded())
			return;

		auto& engineContext = EngineContext::Get();
		auto systemRenderer = engineContext.getSystem<SystemRenderer>();
		if (!systemRenderer)
		{
			Logger->error("System renderer is invalid");
			return;
		}
		systemRenderer->waitCompleteJobs();

		auto& rendererContext = systemRenderer->getRenderer().getRendererContext();
		auto& device = rendererContext.getDevice();
		auto& vma = rendererContext.getVMA();

		if (descriptorSets.isValid())
		{
			ImGui_ImplVulkan_RemoveTexture(descriptorSets.get());
			descriptorSets.invalidate();
		}

		texture.destroy(device, vma);
		loaded = false;
	}

	void AssetTexture::show()
	{
		Asset::show();

		sampler.show();

		if (!sampler || !sampler->isLoaded())
		{
			ImGui::TextCStr("Sampler asset is invalid or not loaded");
			return;
		}

		if (!isLoaded())
			return;
		
		if (!texture.isValid())
			return;

		if (!descriptorSets.isValid())
		{
			auto vkDescriptorSet = ImGui_ImplVulkan_AddTexture(
				sampler->sampler.get(),
				texture.getImageView().get(),
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			);
			descriptorSets = vulkan_renderer::DescriptorSets{ {vkDescriptorSet} };
		}

		ImGui::Separator();
		ImGui::TextCStr("Texture image:");
		ImVec2 size = ImVec2(ImGui::GetContentRegionAvail());
		ImGui::Image((ImTextureID)descriptorSets.get(), size);
	}

	bool AssetTexture::serialize(core::JsonArchive& archive)
	{
		if (!Asset::serialize(archive))
			return false;

		if (!sampler.serialize(archive))
			return false;

		return true;
	}

	bool AssetTexture::deserialize(core::JsonArchive& archive)
	{
		if (!Asset::deserialize(archive))
			return false;

		if (!sampler.deserialize(archive))
			return false;

		return true;
	}

}

