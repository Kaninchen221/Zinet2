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
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VulkanRenderer", spdlog::level::debug);

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

		// TODO: Remove this
		struct CameraData
		{
			glm::mat4 view;
			glm::mat4 projection;
		};

		bool createPipeline(DrawInfo& drawInfo, CameraData cameraData = {});
		
		void draw(const DrawInfo& drawInfo);

		bool submitDrawInfo();

		bool displayCurrentImage();

		const auto& getRendererContext() const { return rendererContext; }
		auto& getRendererContext() { return rendererContext; }

		const auto& getGraphicsPipeline() const { return graphicsPipeline; }
		auto& getGraphicsPipeline() { return graphicsPipeline; }

		bool shouldBePaused() const noexcept { return rendererContext.invalidWindowSizeForPresent; }

	protected:

		RendererContext rendererContext;
		GraphicsPipeline graphicsPipeline;
		// TODO: Refactor remove this two lines
		DescriptorInfo pipelineDescriptorInfo;
		Buffer cameraBuffer{ nullptr };

		static void WindowResizedCallback(void* userPointer, const Vector2i& size);

	};
}