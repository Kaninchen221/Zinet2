#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::vulkan_renderer
{
	class RendererContext;

	class ZINET_VULKAN_RENDERER_API Sprite
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRSprite");

	public:

		Sprite() noexcept = default;
		Sprite(const Sprite& other) noexcept = delete;
		Sprite(Sprite&& other) noexcept = default;
		~Sprite() noexcept = default;

		Sprite& operator = (const Sprite& other) noexcept = delete;
		Sprite& operator = (Sprite&& other) noexcept = default;

	protected:

	};
}