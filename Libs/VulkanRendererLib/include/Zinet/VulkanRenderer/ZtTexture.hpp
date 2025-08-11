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

	struct FillWithImageBufferInput
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

		Texture() noexcept = default;
		Texture(const Texture& other) noexcept = delete;
		Texture(Texture&& other) noexcept = default;
		~Texture() noexcept = default;

		Texture& operator = (const Texture& other) noexcept = delete;
		Texture& operator = (Texture&& other) noexcept = default;

		bool create(const Device& device, const VMA& vma, const Vector2ui& size);

		void destroy(const Device& device, const VMA& vma) noexcept;

		bool isValid() const noexcept;

		const auto& getImage() const noexcept { return image; }
		const auto& getImageView() const noexcept { return imageView; }

		void fillWithImageBuffer(const FillWithImageBufferInput& input) const noexcept;

	protected:

		Image image{ nullptr };
		ImageView imageView{ nullptr };

	};
}