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

	VkResult Buffer::fillWithData(const void* src, size_t srcSize, const VMA& vma) const noexcept
	{
		if (srcSize != size)
			return VK_ERROR_MEMORY_MAP_FAILED;

		void* mappedData{};
		const auto mapResult = vmaMapMemory(vma.get(), allocation, &mappedData);
		if (mapResult != VK_SUCCESS)
			return mapResult;

		std::memcpy(mappedData, src, size);
		vmaUnmapMemory(vma.get(), allocation);

		return VK_SUCCESS;
	}

	VkResult Buffer::getData(void* dst, size_t dstSize, const VMA& vma) const noexcept
	{
		if (dstSize != size)
			return VK_ERROR_MEMORY_MAP_FAILED;

		void* mappedData{};
		const auto mapResult = vmaMapMemory(vma.get(), allocation, &mappedData);
		if (mapResult != VK_SUCCESS)
			return mapResult;

		std::memcpy(dst, mappedData, size);
		vmaUnmapMemory(vma.get(), allocation);

		return VK_SUCCESS;
	}

}