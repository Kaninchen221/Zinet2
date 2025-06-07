#include "Zinet/VulkanRenderer/ZtDescriptorSetLayout.hpp"

namespace zt::vulkan_renderer
{

	VkDescriptorSetLayoutBinding DescriptorSetLayout::GetDefaultLayoutBinding() noexcept
	{
		return VkDescriptorSetLayoutBinding
		{
			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			.pImmutableSamplers = nullptr
		};
	}

	VkDescriptorSetLayoutCreateInfo DescriptorSetLayout::GetDefaultCreateInfo(const Bindings& bindings) noexcept
	{
		return VkDescriptorSetLayoutCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = static_cast<std::uint32_t>(bindings.size()),
			.pBindings = bindings.data()
		};
	}

	bool DescriptorSetLayout::create(const VkDescriptorSetLayoutCreateInfo& createInfo, const Device& device) noexcept
	{
		const auto result = vkCreateDescriptorSetLayout(device.get(), &createInfo, nullptr, &objectHandle);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't create DescriptorSetLayout, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	void DescriptorSetLayout::destroy(const Device& device) noexcept
	{
		if (isValid())
		{
			vkDestroyDescriptorSetLayout(device.get(), objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

}