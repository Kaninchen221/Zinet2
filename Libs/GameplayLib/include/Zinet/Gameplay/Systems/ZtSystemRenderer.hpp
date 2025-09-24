#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode2D.hpp"
#include "Zinet/Gameplay/Nodes/ZtNodeCamera.hpp"
#include "Zinet/Gameplay/Systems/ZtSystem.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetProperty.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"
#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtGraphicsPipelineCreateInfo.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::gameplay
{
	// TODO: More graphics pipelines
	// Now we have a logic for multiple sprites with dynamic UBOs
	// We need:
	//		- logic for using multiple graphics pipelines
	//		- GP for basic shapes
	//		- GP for multiple sprites with static UBOs (maybe some logic like static/dynamic sprite?)
	//		- Push constants
	class SystemRenderer : public System
	{
	private:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::SystemRenderer");

		inline static bool UseImGui = true;

	public:

		SystemRenderer() = default;
		SystemRenderer(const SystemRenderer& other) = default;
		SystemRenderer(SystemRenderer&& other) = default;
		~SystemRenderer() = default;

		SystemRenderer& operator = (const SystemRenderer& other) = default;
		SystemRenderer& operator = (SystemRenderer&& other) = default;

		static void SetUseImGui(bool use) noexcept { UseImGui = use; }
		static bool GetUseImGui() noexcept { return UseImGui; }

		bool init() override;

		void waitCompleteJobs() override { renderer.getRendererContext().getDevice().waitIdle(); }

		bool deinit() override;

		void addNode(const ObjectWeakHandle<Node>& node) override;

		void update() override;

		void show() override;

		auto& getRenderer() noexcept { return renderer; }
		const auto& getRenderer() const noexcept { return renderer; }

		void setCameraNode(ObjectHandle<NodeCamera> newCamera) noexcept;
		auto getCameraNode() const noexcept { return camera; }

		AssetProperty<AssetShader> vertexShader{ "Vertex Shader" };
		AssetProperty<AssetShader> fragmentShader{ "Fragment Shader" };

	protected:

		vulkan_renderer::ImGuiIntegration imGuiIntegration;
		vulkan_renderer::VulkanRenderer renderer;
		vulkan_renderer::GraphicsPipeline graphicsPipeline;

		ObjectHandle<NodeCamera> camera;

		vulkan_renderer::Buffer vertexBuffer{ nullptr };
		vulkan_renderer::Buffer indexBuffer{ nullptr };
		vulkan_renderer::DrawInfo drawInfo
		{
			.vertexBuffer = &vertexBuffer,
			.indexBuffer = &indexBuffer,
			.indexCount = static_cast<uint32_t>(6),
			.instances = 0
		};

		vulkan_renderer::
		GraphicsPipelineCreateInfo graphicsPipelineCreateInfo
		{
			.rendererContext = renderer.getRendererContext(),
			.shaderModules =
			{
				// Set after the assets are ready to use
				// { vulkan_renderer::ShaderType::Vertex, nullptr },
				// { vulkan_renderer::ShaderType::Fragment, nullptr }
			},
			.descriptorInfos =
			{
				// [0] Pipeline descriptor set info
				// [1] Object descriptor set info
			},
			// .descriptorSetsCount = Set after init renderer
		};
	};

}