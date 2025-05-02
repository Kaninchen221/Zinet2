#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtVertex.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>

namespace zt::vulkan_renderer
{
	class VMA;

	class ZINET_VULKAN_RENDERER_API Buffer : public VulkanObject<VkBuffer>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRBuffer");

	public:

		Buffer(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		Buffer() noexcept = delete;
		Buffer(const Buffer& other) noexcept = delete;
		Buffer(Buffer&& other) noexcept = default;
		~Buffer() noexcept = default;

		Buffer& operator = (const Buffer& other) noexcept = delete;
		Buffer& operator = (Buffer&& other) noexcept = default;

		bool createVertexBuffer(const auto& vertices, const VMA& vma) noexcept;

		bool fill(const auto& contiguousContainer, const VMA& vma) noexcept;

		bool getData(auto& contiguousContainer, const VMA& vma) const noexcept;

		void destroy(const VMA& vma) noexcept;

		std::uint32_t getSize() const noexcept { return size; }

	protected:

		VmaAllocation allocation{};
		std::uint32_t size{};

	};

	bool Buffer::createVertexBuffer(const auto& vertices, const VMA& vma) noexcept
	{
		if (isValid())
			return false;

		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = sizeof(Vertex) * vertices.size();
		createInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

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

	bool Buffer::fill(const auto& contiguousContainer, const VMA& vma) noexcept
	{
		if (!isValid())
			return false;

		const auto containerSize = sizeof(std::remove_reference_t<decltype(contiguousContainer)>::value_type) * contiguousContainer.size();
		if (containerSize != size)
			return false;

		void* mappedData{};
		const auto mapResult = vmaMapMemory(vma.get(), allocation, &mappedData);

		if (mapResult == VK_SUCCESS)
		{
			std::memcpy(mappedData, contiguousContainer.data(), size);
			vmaUnmapMemory(vma.get(), allocation);
			return true;
		}
		else
		{
			Logger->error("Couldn't fill buffer, result: {}", static_cast<std::int32_t>(mapResult));
			return false;
		}
	}

	bool Buffer::getData(auto& contiguousContainer, const VMA& vma) const noexcept
	{
		if (!isValid())
			return false;

		const auto containerSize = sizeof(std::remove_reference_t<decltype(contiguousContainer)>::value_type) * contiguousContainer.size();
		if (containerSize != size)
			return false;

		void* mappedData{};
		const auto mapResult = vmaMapMemory(vma.get(), allocation, &mappedData);

		if (mapResult == VK_SUCCESS)
		{
			std::memcpy(contiguousContainer.data(), mappedData, size);
			vmaUnmapMemory(vma.get(), allocation);
			return true;
		}
		else
		{
			Logger->error("Couldn't get buffer data, result: {}", static_cast<std::int32_t>(mapResult));
			return false;
		}
	}
}