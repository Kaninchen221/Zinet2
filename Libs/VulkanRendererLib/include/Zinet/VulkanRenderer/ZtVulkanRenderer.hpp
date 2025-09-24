#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::wd
{
	class Window;
}

namespace zt::vulkan_renderer
{
	struct DrawInfo;

	class  VulkanRenderer : public core::Object
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::vulkan_renderer::VulkanRenderer", spdlog::level::info);

	public:

		VulkanRenderer() = default;
		VulkanRenderer(const VulkanRenderer& other) = default;
		VulkanRenderer(VulkanRenderer&& other) = default;
		~VulkanRenderer() noexcept = default;

		VulkanRenderer& operator = (const VulkanRenderer& other) noexcept = default;
		VulkanRenderer& operator = (VulkanRenderer&& other) noexcept = default;

		bool init(wd::Window& window);

		void deinit();

		bool nextImage();
		
		void draw(GraphicsPipeline& graphicsPipeline, const DrawInfo& drawInfo);

		bool submitCurrentDisplayImage();

		bool displayCurrentImage();

		const auto& getRendererContext() const { return rendererContext; }
		auto& getRendererContext() { return rendererContext; }

		bool shouldBePaused() const noexcept { return rendererContext.invalidWindowSizeForPresent; }

	protected:

		RendererContext rendererContext;

		static void WindowResizedCallback(void* userPointer, const Vector2i& size);

	};
}