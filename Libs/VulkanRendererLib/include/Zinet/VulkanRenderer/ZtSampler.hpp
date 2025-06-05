#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;

	class ZINET_VULKAN_RENDERER_API Sampler : public VulkanObject<VkSampler>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRSampler");

	public:

		Sampler(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle)
		{}

		Sampler() noexcept = delete;
		Sampler(const Sampler& other) noexcept = delete;
		Sampler(Sampler&& other) noexcept = default;
		~Sampler() noexcept = default;

		Sampler& operator = (const Sampler& other) noexcept = delete;
		Sampler& operator = (Sampler&& other) noexcept = default;

		static VkSamplerCreateInfo GetDefaultCreateInfo() noexcept;

		bool create(const Device& device, const VkSamplerCreateInfo& createInfo) noexcept;

		void destroy(const Device& device) noexcept;

	};
}