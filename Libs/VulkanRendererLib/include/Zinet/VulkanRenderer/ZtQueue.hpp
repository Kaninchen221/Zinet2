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

		Queue() noexcept = delete;
		Queue(const Queue& other) noexcept = delete;
		Queue(Queue&& other) noexcept = default;
		~Queue() noexcept = default;

		Queue& operator = (const Queue& other) noexcept = delete;
		Queue& operator = (Queue&& other) noexcept = default;

		void invalidate() { objectHandle = nullptr; }

		const auto& getQueueFamilyIndex() const noexcept { return queueFamilyIndex; }

		bool submit(const STDContainer auto& contiguousContainerSubmitInfos, const Fence& fence = Fence{ nullptr }) const noexcept;

		bool waitIdle() const noexcept;

	protected:

		std::uint32_t queueFamilyIndex = InvalidIndex;

	};

	inline bool Queue::submit(const STDContainer auto& contiguousContainerSubmitInfos, const Fence& fence) const noexcept
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