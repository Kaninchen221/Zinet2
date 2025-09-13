#pragma once

#include <type_traits>

#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtTexture.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtQueueUtils.hpp"
#include "Zinet/VulkanRenderer/ZtSampler.hpp"

#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtImage.hpp"

// TODO: Create lib with utils for tests
namespace zt::vulkan_renderer::tests
{
	template<class ClassT, class ObjectHandleT, class ...ConstructorArgsTs>
	inline constexpr bool VulkanObjectDecoratorStaticTest()
	{
		if constexpr (!std::is_base_of_v<VulkanObject<ObjectHandleT>, ClassT>) return false;

		if constexpr (!std::is_constructible_v<ClassT, ObjectHandleT, ConstructorArgsTs...>) return false;
		if constexpr (std::is_default_constructible_v<ClassT>) return false;
		if constexpr (std::is_copy_constructible_v<ClassT>) return false;
		if constexpr (std::is_copy_assignable_v<ClassT>) return false;
		if constexpr (!std::is_move_constructible_v<ClassT>) return false;
		if constexpr (!std::is_move_assignable_v<ClassT>) return false;
		if constexpr (!std::is_destructible_v<ClassT>) return false;

		return true;
	}

	inline ShaderModule CreateShaderModule(std::string_view sourceCodeFileName, ShaderType shaderType, const Device& device)
	{
		ShadersCompiler shadersCompiler;
		ShaderModule shaderModule{ nullptr };

		const auto contentFolderPath = core::Paths::CurrentProjectRootPath() / "test_files";

		const auto fullPath = contentFolderPath / sourceCodeFileName;
		const auto compileResult = shadersCompiler.compileFromFile(fullPath, shaderType);
		if (compileResult.empty())
		{
			Ensure(false, "Compile returned empty result");
			return shaderModule;
		}

		shaderModule.create(device, compileResult);

		return shaderModule;
	}

	inline Buffer CreateUniformBuffer(const auto& uniformData, const VMA& vma)
	{
		Buffer buffer{ nullptr };
		const auto createInfo = Buffer::GetUniformBufferCreateInfo(uniformData);
		if (!buffer.createBuffer(createInfo, vma))
		{
			Ensure(false, "Buffer create returned false");
			return Buffer{ nullptr };
		}
		return buffer;
	}

	inline Texture CreateTexture(const RendererContext& rendererContext)
	{
		const auto& vma = rendererContext.getVMA();
		const auto& device = rendererContext.getDevice();
		const auto& queue = rendererContext.getQueue();
		const auto& commandPool = rendererContext.getCommandPool();

		const std::filesystem::path testFolderPath = core::Paths::CurrentProjectRootPath() / "test_files";
		const std::filesystem::path imagePath = testFolderPath / "image.png";

		core::Image sourceImage;
		if (!sourceImage.loadFromFile(imagePath, 4))
		{
			Ensure(false, "Image load returned false");
			return {};
		}

		Buffer buffer{ nullptr };
		const auto bufferCreateInfo = Buffer::GetImageBufferCreateInfo(sourceImage);
		if (!buffer.createBuffer(bufferCreateInfo, vma))
		{
			Ensure(false, "Buffer create returned false");
			return {};
		}

		if (!buffer.fillWithImage(sourceImage, vma))
		{
			Ensure(false, "Buffer fill returned false");
			return {};
		}

		Texture texture;
		if (!texture.create(device, vma, { sourceImage.getWidth(), sourceImage.getHeight() }))
		{
			Ensure(false, "Texture create returned false");
			return {};
		}

		const auto commands = [&texture = texture, &sourceImage = sourceImage, &buffer = buffer](const CommandBuffer& commandBuffer)
		{
			FillWithImageBufferInput input
			{
				.buffer = buffer,
				.commandBuffer = commandBuffer,
				.imageExtent = { sourceImage.getWidth(), sourceImage.getHeight() }
			};
			texture.fillWithImageBuffer(input);
		};
		SubmitSingleCommandBufferWaitIdle(device, queue, commandPool, commands);

		buffer.destroy(vma);

		return texture;
	}

	inline Sampler CreateSampler(const Device& device)
	{
		Sampler sampler{ nullptr };
		const auto samplerCreateInfo = Sampler::GetDefaultCreateInfo();
		if (!sampler.create(device, samplerCreateInfo))
		{
			Ensure(false, "Sampler create returned false");
			return Sampler{ nullptr };
		}

		return sampler;
	}
}