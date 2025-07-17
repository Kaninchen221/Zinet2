#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtVertex.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtConcepts.hpp"

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>

namespace zt::core
{
	class Image;
}

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

		Buffer() ZINET_API_POST = delete;
		Buffer(const Buffer& other) ZINET_API_POST = delete;
		Buffer(Buffer&& other) ZINET_API_POST = default;
		~Buffer() ZINET_API_POST = default;

		Buffer& operator = (const Buffer& other) ZINET_API_POST = delete;
		Buffer& operator = (Buffer&& other) ZINET_API_POST = default;

		template<core::STDContainer ContainerT>
		static VkBufferCreateInfo GetVertexBufferCreateInfo(const ContainerT& vertices) ZINET_API_POST;

		template<core::STDContainer ContainerT>
		static VkBufferCreateInfo GetIndexBufferCreateInfo(const ContainerT& indices) ZINET_API_POST;

		template<core::NotSTDContainer ObjectT>
		static VkBufferCreateInfo GetUniformBufferCreateInfo(const ObjectT& data) ZINET_API_POST;

		static VkBufferCreateInfo GetImageBufferCreateInfo(const core::Image& image) ZINET_API_POST;

		bool createBuffer(const VkBufferCreateInfo& createInfo, const VMA& vma) ZINET_API_POST;

		template<core::STDContainer ContainerT>
		bool fillWithSTDContainer(const ContainerT& contiguousContainer, const VMA& vma) ZINET_API_POST;

		template<core::NotSTDContainer ObjectT>
		bool fillWithObject(const ObjectT& object, const VMA& vma) ZINET_API_POST;

		bool fillWithImage(const core::Image& image, const VMA& vma) ZINET_API_POST;

		template<core::STDContainer ContainerT>
		bool getDataToSTDContainer(ContainerT& contiguousContainer, const VMA& vma) const ZINET_API_POST;

		template<core::NotSTDContainer ObjectT>
		bool getDataToObject(ObjectT& object, const VMA& vma) const ZINET_API_POST;

		void destroy(const VMA& vma) ZINET_API_POST;

		std::uint32_t getSize() const ZINET_API_POST { return size; }

		VmaAllocation getAllocation() const ZINET_API_POST { return allocation; }

	protected:

		VkResult fillWithData(const void* src, size_t srcSize, const VMA& vma) const ZINET_API_POST;

		VkResult getData(void* dst, size_t dstSize, const VMA& vma) const ZINET_API_POST;

		VmaAllocation allocation{};
		std::uint32_t size{};

	};

	template<core::STDContainer ContainerT>
	VkBufferCreateInfo Buffer::GetVertexBufferCreateInfo(const ContainerT& vertices) ZINET_API_POST
	{
		return VkBufferCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = sizeof(typename ContainerT::value_type) * vertices.size(),
			.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE
		};
	}

	template<core::STDContainer ContainerT>
	VkBufferCreateInfo Buffer::GetIndexBufferCreateInfo(const ContainerT& indices) ZINET_API_POST
	{
		return VkBufferCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = sizeof(typename ContainerT::value_type) * indices.size(),
			.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE
		};
	}

	template<core::NotSTDContainer ObjectT>
	VkBufferCreateInfo Buffer::GetUniformBufferCreateInfo([[maybe_unused]] const ObjectT& data) ZINET_API_POST
	{
		return VkBufferCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = sizeof(ObjectT),
			.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE
		};
	}

	template<core::STDContainer ContainerT>
	bool Buffer::fillWithSTDContainer(const ContainerT& stdContainer, const VMA& vma) ZINET_API_POST
	{
		if (!isValid())
			return false;

		const auto containerSize = sizeof(typename ContainerT::value_type) * stdContainer.size();
		const auto result = fillWithData(stdContainer.data(), containerSize, vma);

		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't fill buffer, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	template<core::NotSTDContainer ObjectT>
	bool Buffer::fillWithObject(const ObjectT& object, const VMA& vma) ZINET_API_POST
	{
		if (!isValid())
			return false;

		const auto result = fillWithData(&object, sizeof(ObjectT), vma);

		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't fill buffer, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	template<core::STDContainer ContainerT>
	bool Buffer::getDataToSTDContainer(ContainerT& stdContainer, const VMA& vma) const ZINET_API_POST
	{
		if (!isValid())
			return false;

		const auto containerSize = sizeof(typename ContainerT::value_type) * stdContainer.size();
		const auto result = getData(stdContainer.data(), containerSize, vma);

		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't get buffer data, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	template<core::NotSTDContainer ObjectT>
	bool Buffer::getDataToObject(ObjectT& object, const VMA& vma) const ZINET_API_POST
	{
		if (!isValid())
			return false;

		const auto result = getData(&object, sizeof(object), vma);

		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't get buffer data, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}
}