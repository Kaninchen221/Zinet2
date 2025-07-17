#include "Zinet/VulkanRenderer/ZtSampler.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

namespace zt::vulkan_renderer
{
	VkSamplerCreateInfo Sampler::GetDefaultCreateInfo() ZINET_API_POST
	{
		return 
		{
			.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.magFilter = VK_FILTER_LINEAR,
			.minFilter = VK_FILTER_LINEAR,
			.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
			.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.mipLodBias = 0.f,
			.anisotropyEnable = VK_FALSE,
			.maxAnisotropy = 0.f,
			.compareEnable = VK_FALSE,
			.compareOp = VK_COMPARE_OP_ALWAYS,
			.minLod = 0.f,
			.maxLod = 0.f,
			.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
			.unnormalizedCoordinates = VK_FALSE
		};
	}

	bool Sampler::create(const Device& device, const VkSamplerCreateInfo& createInfo) ZINET_API_POST
	{
		if (isValid())
			return false;

		const auto result = vkCreateSampler(device.get(), &createInfo, nullptr, &objectHandle);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't create sampler, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	void Sampler::destroy(const Device& device) ZINET_API_POST
	{
		if (isValid())
		{
			vkDestroySampler(device.get(), objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

}