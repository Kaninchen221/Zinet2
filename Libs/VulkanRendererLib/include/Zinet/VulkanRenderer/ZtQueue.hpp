#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtFence.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtConcepts.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class ZINET_VULKAN_RENDERER_API Queue : public VulkanObject<VkQueue>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRQueue");

	public:

		Queue(HandleType newObjectHandle, std::uint32_t newQueueFamilyIndex = InvalidIndex)
			: VulkanObject(newObjectHandle),
			  queueFamilyIndex{ newQueueFamilyIndex }
		{}

		Queue() ZINET_API_POST = delete;
		Queue(const Queue& other) ZINET_API_POST = delete;
		Queue(Queue&& other) ZINET_API_POST = default;
		~Queue() ZINET_API_POST = default;

		Queue& operator = (const Queue& other) ZINET_API_POST = delete;
		Queue& operator = (Queue&& other) ZINET_API_POST = default;

		void invalidate() { objectHandle = nullptr; }

		const auto& getQueueFamilyIndex() const ZINET_API_POST { return queueFamilyIndex; }

		bool submit(const core::STDContainer auto& contiguousContainerSubmitInfos, const Fence& fence = Fence{ nullptr }) const ZINET_API_POST;

		bool waitIdle() const ZINET_API_POST;

	protected:

		std::uint32_t queueFamilyIndex = InvalidIndex;

	};

	inline bool Queue::submit(const core::STDContainer auto& contiguousContainerSubmitInfos, const Fence& fence) const ZINET_API_POST
	{
		const auto result = vkQueueSubmit(get(), static_cast<uint32_t>(contiguousContainerSubmitInfos.size()), contiguousContainerSubmitInfos.data(), fence.get());
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("vkQueueSubmit returned false, value: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}
}