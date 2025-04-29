#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSwapChain.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtFramebuffer.hpp"
#include "Zinet/VulkanRenderer/ZtRenderPass.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"
#include "Zinet/VulkanRenderer/ZtSemaphore.hpp"
#include "Zinet/VulkanRenderer/ZtFence.hpp"
#include "Zinet/VulkanRenderer/ZtCommandPool.hpp"
#include "Zinet/VulkanRenderer/ZtCommandBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtPipelineLayout.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::wd
{
	class Window;
}

namespace zt::vulkan_renderer
{
	class ShaderModule;

	struct DrawInfo 
	{
		const ShaderModule& vertexShaderModule;
		const ShaderModule& fragmentShaderModule;
	};

	ZT_REFLECT_CLASS()
	class ZINET_VULKAN_RENDERER_API VulkanRenderer : public core::Object
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VulkanRenderer");

	public:

		bool start(wd::Window& window) noexcept;

		void shutdown() noexcept;

		void draw(const DrawInfo& drawInfo) noexcept;

		const RendererContext& getRendererContext() const noexcept { return rendererContext; }
		RendererContext& getRendererContext() noexcept { return rendererContext; }

	protected:

		RendererContext rendererContext;

		std::vector<VkImage> images;
		std::vector<ImageView> imageViews;
		std::vector<Framebuffer> framebuffers;

		CommandBuffer commandBuffer{ nullptr };
		Semaphore imageAvailableSemaphore{ nullptr };
		Semaphore renderFinishedSemaphore{ nullptr };
		Fence fence{ nullptr };

		RenderPass renderPass{ nullptr };
		PipelineLayout pipelineLayout{ nullptr };
		Pipeline pipeline{ nullptr };

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<VulkanRenderer>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<VulkanRenderer>(); *result = *this; return result; }
		
		VulkanRenderer() = default;
		VulkanRenderer(const VulkanRenderer& other) = default;
		VulkanRenderer(VulkanRenderer&& other) = default;
		~VulkanRenderer() noexcept = default;
		
		VulkanRenderer& operator = (const VulkanRenderer& other) = default;
		VulkanRenderer& operator = (VulkanRenderer&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "VulkanRenderer"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
/*GENERATED_CODE_END*/
	};
}