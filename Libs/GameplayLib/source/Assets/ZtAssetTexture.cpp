#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"

#include "Zinet/Core/ZtFile.hpp"
#include "Zinet/Core/ZtImgui.hpp"

#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

namespace zt::gameplay::asset
{
	bool Texture::load()
	{
		if (isLoaded())
			return true;

		core::File file;
		const auto filePath = core::Paths::RootPath() / metaData.value("fileRelativePath", "");
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

		if (!image.loadFromData(rawData, 4))
		{
			Logger->error("Couldn't load an image from a data");
			return false;
		}

		loaded = true;
		return true;
	}

	void Texture::unload()
	{
		if (!isLoaded())
			return;

		image.destroy();

		loaded = false;
	}

	Texture::ResourceOptT Texture::createResource(vulkan_renderer::RendererContext& rendererContext)
	{
		using namespace vulkan_renderer;
		vulkan_renderer::Texture texture;

		auto& device = rendererContext.getDevice();
		auto& vma = rendererContext.getVMA();

		Buffer buffer{ nullptr };
		{
			if (!buffer.create(vma, Buffer::GetImageBufferCreateInfo(image)))
			{
				Logger->error("Couldn't create a buffer for texture resource from an asset");
				return {};
			}

			if (!buffer.fillWithImage(vma, image))
			{
				Logger->error("Couldn't fill a buffer for texture resource from an asset");
				return {};
			}
		}

		const auto size = Vector2ui{ static_cast<uint32_t>(image.getWidth()), static_cast<uint32_t>(image.getHeight()) };
		if (!texture.create(device, vma, size))
		{
			Logger->error("Couldn't create a texture resource from an asset");
			return {};
		}

		// TODO: Not the best way to give the texture a buffer with image data
		auto commands = [&texture = texture, size = size, buffer = std::make_shared<Buffer>( std::move(buffer) )]
		(const CommandBuffer& commandBuffer) mutable -> void
		{
			FillWithImageBufferInput input
			{
				.buffer = std::move(*buffer),
				.commandBuffer = commandBuffer,
				.imageExtent = size
			};
			texture.fillWithImageBuffer(input);
		};

		rendererContext.enqueueTransferCommands(commands);

		return texture;
	}
}

