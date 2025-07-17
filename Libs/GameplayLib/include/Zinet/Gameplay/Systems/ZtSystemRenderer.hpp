#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode2D.hpp"
#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

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
	class ZINET_GAMEPLAY_API SystemRenderer : public System<Node2D>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::SystemRenderer");

	public:

		using ClassBaseT = System<Node2D>;

		SystemRenderer() ZINET_API_POST = default;
		SystemRenderer(const SystemRenderer& other) ZINET_API_POST = default;
		SystemRenderer(SystemRenderer&& other) ZINET_API_POST = default;
		~SystemRenderer() ZINET_API_POST = default;

		SystemRenderer& operator = (const SystemRenderer& other) ZINET_API_POST = default;
		SystemRenderer& operator = (SystemRenderer&& other) ZINET_API_POST = default;

		bool init() ZINET_API_POST override;

		void deinit() ZINET_API_POST override;

		void addNode(const NodeWeakHandle<NodeT>& node) ZINET_API_POST;

		void update() ZINET_API_POST;

		auto& getRenderer() ZINET_API_POST { return renderer; }
		const auto& getRenderer() const ZINET_API_POST { return renderer; }

	protected:

		vr::ShaderModule createShaderModule(std::string_view sourceCodeFileName, vr::ShaderType shaderType);

		vr::ImGuiIntegration imGuiIntegration;
		vr::VulkanRenderer renderer;

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
			.instances = 0,
			//.pipelineDescriptorInfo = { {}, {} },
			//.drawCallDescriptorInfo =
			//{
			//	.uniformBuffers = { { &uniformBuffers[0] }, { &uniformBuffers[1] } },
			//	.texturesInfos{}
			//}//,
			//.additionalCommands = { ImGuiIntegration::DrawCommand }
		};
	};

}