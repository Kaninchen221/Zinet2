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
	class ZINET_VULKAN_RENDERER_API ImGuiIntegration
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

		bool init(const RendererContext& rendererContext, wd::Window& window) noexcept;

		void deinit(const RendererContext& rendererContext) noexcept;

		static void ImplSpecificNewFrame() noexcept;

		void prepareRenderData() const noexcept;

		static void DrawCommand(const CommandBuffer& commandBuffer) noexcept;

	protected:

		DescriptorPool descriptorPool{ nullptr };

	};
}