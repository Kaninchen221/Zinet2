#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorPool.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::wd
{
	class Window;
}

namespace zt::vulkan_renderer
{
	class RendererContext;
	class CommandBuffer;

	/// For examples of using ImGui see ImGui::ShowDemoWindow();
	class  ImGuiIntegration
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRImGui");

	public:

		ImGuiIntegration() noexcept = default;
		ImGuiIntegration(const ImGuiIntegration& other) noexcept = delete;
		ImGuiIntegration(ImGuiIntegration&& other) noexcept = default;
		~ImGuiIntegration() noexcept = default;

		ImGuiIntegration& operator = (const ImGuiIntegration& other) noexcept = delete;
		ImGuiIntegration& operator = (ImGuiIntegration&& other) noexcept = default;

		bool init(const RendererContext& rendererContext, wd::Window& window, bool multiViewport = true);

		void deinit(const RendererContext& rendererContext);

		bool isInitialized() const noexcept { return initialized; }

		static void ImplSpecificNewFrame();

		void prepareRenderData() const;

		static void DrawCommand(CommandBuffer& commandBuffer);

		static void SetStyle_Dark();

	protected:

		DescriptorPool descriptorPool{ nullptr };

		bool initialized = false;

	};
}