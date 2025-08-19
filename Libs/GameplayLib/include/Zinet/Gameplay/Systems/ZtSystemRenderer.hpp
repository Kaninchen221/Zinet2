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

#include "Zinet/Window/ZtWindow.hpp"

namespace
{
	namespace vr = zt::vulkan_renderer;
}

namespace zt::gameplay
{
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

		bool deinit() override;

		void update() override;

		void imGui() override;

		auto& getRenderer() noexcept { return renderer; }
		const auto& getRenderer() const noexcept { return renderer; }

		void setCameraNode(ObjectHandle<NodeCamera> newCamera) noexcept { camera = newCamera; }
		auto getCameraNode() const noexcept { return camera; }

		AssetProperty<AssetShader> vertexShader{ "Vertex Shader" };
		AssetProperty<AssetShader> fragmentShader{ "Fragment Shader" };

	protected:

		vr::ImGuiIntegration imGuiIntegration;
		vr::VulkanRenderer renderer;
		
		ObjectHandle<NodeCamera> camera;

		vr::ShaderModule vertexShaderModule{ nullptr };
		vr::ShaderModule fragmentShaderModule{ nullptr };
		vr::Buffer vertexBuffer{ nullptr };
		vr::Buffer indexBuffer{ nullptr };
		vr::DrawInfo drawInfo
		{
			.vertexShaderModule = &vertexShaderModule,
			.fragmentShaderModule = &fragmentShaderModule,
			.vertexBuffer = &vertexBuffer,
			.indexBuffer = &indexBuffer,
			.indexCount = static_cast<uint32_t>(6),
			.instances = 0
		};
	};

}