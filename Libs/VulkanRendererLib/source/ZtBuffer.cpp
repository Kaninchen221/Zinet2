#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"

namespace zt::vulkan_renderer
{

	bool Buffer::createBuffer(const VkBufferCreateInfo& createInfo, const VMA& vma) noexcept
	{
		if (isValid())
			return false;

		VmaAllocationCreateInfo allocInfo = {};
		allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
		allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

		const auto result = vmaCreateBuffer(vma.get(), &createInfo, &allocInfo, &objectHandle, &allocation, nullptr);
		if (result == VK_SUCCESS)
		{
			size = static_cast<std::uint32_t>(createInfo.size);
			return true;
		}
		else
		{
			Logger->error("Couldn't create buffer, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	void Buffer::destroy(const VMA& vma) noexcept
	{
		if (isValid())
		{
			vmaDestroyBuffer(vma.get(), objectHandle, allocation);
			objectHandle = nullptr;
			allocation = nullptr;
			size = 0u;
		}
	}

}