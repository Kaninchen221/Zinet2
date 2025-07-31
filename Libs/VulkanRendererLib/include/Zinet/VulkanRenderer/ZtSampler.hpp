#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;

	inline VkFilter SamplerTypeFromString(std::string_view str) ZINET_API_POST
	{
		if (str == "nearest")
			return VkFilter::VK_FILTER_NEAREST;
		else if (str == "linear")
			return VkFilter::VK_FILTER_LINEAR;
		else if (str == "cubic")
			return VkFilter::VK_FILTER_CUBIC_EXT;

		auto Logger = core::ConsoleLogger::Create("SamplerTypeFromString");
		Logger->warn("Couldn't match str: {} to any filter type, using nearest as default", str);
		return VkFilter::VK_FILTER_NEAREST;
	}

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