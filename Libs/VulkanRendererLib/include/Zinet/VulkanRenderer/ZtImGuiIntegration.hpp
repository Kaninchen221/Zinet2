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

		ImGuiIntegration() ZINET_API_POST = default;
		ImGuiIntegration(const ImGuiIntegration& other) ZINET_API_POST = delete;
		ImGuiIntegration(ImGuiIntegration&& other) ZINET_API_POST = default;
		~ImGuiIntegration() ZINET_API_POST = default;

		ImGuiIntegration& operator = (const ImGuiIntegration& other) ZINET_API_POST = delete;
		ImGuiIntegration& operator = (ImGuiIntegration&& other) ZINET_API_POST = default;

		bool init(const RendererContext& rendererContext, wd::Window& window) ZINET_API_POST;

		void deinit(const RendererContext& rendererContext) ZINET_API_POST;

		static void ImplSpecificNewFrame() ZINET_API_POST;

		void prepareRenderData() const ZINET_API_POST;

		static void DrawCommand(const CommandBuffer& commandBuffer) ZINET_API_POST;

		static void SetStyle_Dark() ZINET_API_POST;

	protected:

		DescriptorPool descriptorPool{ nullptr };

	};
}