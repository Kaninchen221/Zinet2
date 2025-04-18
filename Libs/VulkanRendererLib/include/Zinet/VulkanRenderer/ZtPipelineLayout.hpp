#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;

	class ZINET_VULKAN_RENDERER_API PipelineLayout : public VulkanObject<VkPipelineLayout>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRPipelineLayout");

	public:

		PipelineLayout(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		PipelineLayout() noexcept = delete;
		PipelineLayout(const PipelineLayout& other) noexcept = delete;
		PipelineLayout(PipelineLayout&& other) noexcept = default;
		~PipelineLayout() noexcept = default;

		PipelineLayout& operator = (const PipelineLayout& other) noexcept = delete;
		PipelineLayout& operator = (PipelineLayout&& other) noexcept = default;
	
		bool create(const Device& device) noexcept;

		void destroy(const Device& device) noexcept;

	};
}