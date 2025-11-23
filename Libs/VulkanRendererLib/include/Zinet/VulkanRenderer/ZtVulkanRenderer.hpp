#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

#include "Zinet/Window/ZtWindow.hpp"

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

		using Command = std::function<void(CommandBuffer&)>;

		VulkanRenderer() = default;
		VulkanRenderer(const VulkanRenderer& other) = default;
		VulkanRenderer(VulkanRenderer&& other) 
		{  
			*this = std::move(other);
		};
		~VulkanRenderer() noexcept = default;

		VulkanRenderer& operator = (const VulkanRenderer& other) noexcept = default;
		VulkanRenderer& operator = (VulkanRenderer&& other) noexcept
		{
			rendererContext = std::move(other.rendererContext);
			initialized = other.initialized;

			other.initialized = false;

			wd::Window::SetWindowResizedCallback(this, WindowResizedCallback);

			return *this;
		}

		bool init(wd::Window& window);

		void deinit();

		bool nextImage();

		void startRecordingDrawCommands();

		void beginRenderPass(RenderPass& renderPass);

		void draw(const Command& command);

		void endRenderPass();

		void endRecordingDrawCommands();

		bool submitCurrentDisplayImage();

		bool displayCurrentImage();

		const auto& getRendererContext() const { return rendererContext; }
		auto& getRendererContext() { return rendererContext; }

		bool shouldBePaused() const noexcept { return rendererContext.invalidWindowSizeForPresent; }

		bool isInitialized() const noexcept { return initialized; }

	protected:

		RendererContext rendererContext;

		bool initialized = false;

		static void WindowResizedCallback(void* userPointer, const Vector2i& size);

	};
}