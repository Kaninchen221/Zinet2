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

		Sampler() ZINET_API_POST = delete;
		Sampler(const Sampler& other) ZINET_API_POST = delete;
		Sampler(Sampler&& other) ZINET_API_POST = default;
		~Sampler() ZINET_API_POST = default;

		Sampler& operator = (const Sampler& other) ZINET_API_POST = delete;
		Sampler& operator = (Sampler&& other) ZINET_API_POST = default;

		static VkSamplerCreateInfo GetDefaultCreateInfo() ZINET_API_POST;

		bool create(const Device& device, const VkSamplerCreateInfo& createInfo) ZINET_API_POST;

		void destroy(const Device& device) ZINET_API_POST;

	};
}