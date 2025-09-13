#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;

	inline VkFilter SamplerTypeFromString(std::string_view str)
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

	class Sampler : public VulkanObject<VkSampler>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRSampler");

	public:

		Sampler(VulcanType newHandle) noexcept
			: VulkanObject(newHandle)
		{}

		Sampler() noexcept = delete;
		Sampler(const Sampler& other) noexcept = delete;
		Sampler(Sampler&& other) noexcept = default;
		~Sampler() noexcept = default;

		Sampler& operator = (const Sampler& other) noexcept = delete;
		Sampler& operator = (Sampler&& other) noexcept = default;

		static VkSamplerCreateInfo GetDefaultCreateInfo() noexcept;

		bool create(const Device& device, const VkSamplerCreateInfo& createInfo);

		void destroy(const Device& device) noexcept;

	};
}