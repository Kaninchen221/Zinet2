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
	class ZINET_GAMEPLAY_API SystemRenderer : public System
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::SystemRenderer");

	public:

		SystemRenderer() ZINET_API_POST = default;
		SystemRenderer(const SystemRenderer& other) ZINET_API_POST = default;
		SystemRenderer(SystemRenderer&& other) ZINET_API_POST = default;
		~SystemRenderer() ZINET_API_POST = default;

		SystemRenderer& operator = (const SystemRenderer& other) ZINET_API_POST = default;
		SystemRenderer& operator = (SystemRenderer&& other) ZINET_API_POST = default;

		bool init() ZINET_API_POST override;

		void deinit() ZINET_API_POST override;

		void update() ZINET_API_POST override;

		inline static bool UseImGui = true;

		void imGui() ZINET_API_POST override;

		auto& getRenderer() ZINET_API_POST { return renderer; }
		const auto& getRenderer() const ZINET_API_POST { return renderer; }

		void setCameraNode(ObjectHandle<NodeCamera> newCamera) ZINET_API_POST { camera = newCamera; }
		auto getCameraNode() const ZINET_API_POST { return camera; }

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