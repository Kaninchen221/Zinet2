#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtImage.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>

namespace zt::vulkan_renderer
{
	class Device;
	class VMA;
	class Buffer;
	class CommandBuffer;

	struct ZINET_VULKAN_RENDERER_API FillWithImageBufferInput
	{
		const Buffer& buffer;
		const CommandBuffer& commandBuffer;
		const Vector2ui imageExtent;
		const Vector3i imageOffset = { 0u, 0u, 0u };
	};

	class ZINET_VULKAN_RENDERER_API Texture
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRTexture");

	public:

		Texture() ZINET_API_POST = default;
		Texture(const Texture& other) ZINET_API_POST = delete;
		Texture(Texture&& other) ZINET_API_POST = default;
		~Texture() ZINET_API_POST = default;

		Texture& operator = (const Texture& other) ZINET_API_POST = delete;
		Texture& operator = (Texture&& other) ZINET_API_POST = default;

		bool create(const Device& device, const VMA& vma, const Vector2ui& size) ZINET_API_POST;

		void destroy(const Device& device, const VMA& vma) ZINET_API_POST;

		bool isValid() const ZINET_API_POST;

		const auto& getImage() const ZINET_API_POST { return image; }
		const auto& getImageView() const ZINET_API_POST { return imageView; }

		void fillWithImageBuffer(const FillWithImageBufferInput& input) const ZINET_API_POST;

	protected:

		Image image{ nullptr };
		ImageView imageView{ nullptr };

	};
}