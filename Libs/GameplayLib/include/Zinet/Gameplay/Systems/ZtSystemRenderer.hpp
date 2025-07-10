#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/ZtNode2D.hpp"
#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"

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

		SystemRenderer() noexcept = default;
		SystemRenderer(const SystemRenderer& other) noexcept = default;
		SystemRenderer(SystemRenderer&& other) noexcept = default;
		~SystemRenderer() noexcept = default;

		SystemRenderer& operator = (const SystemRenderer& other) noexcept = default;
		SystemRenderer& operator = (SystemRenderer&& other) noexcept = default;

		bool init() noexcept override;

		bool deinit() noexcept override;

		void addNode(const NodeWeakHandle<NodeT>& node) noexcept;

		void update() noexcept;

	protected:

		vr::VulkanRenderer renderer;
		wd::Window window;

		vr::ShaderModule vertexShaderModule{ nullptr };
		vr::ShaderModule fragmentShaderModule{ nullptr };
		vr::Buffer vertexBuffer{ nullptr };
		vr::Buffer indexBuffer{ nullptr };
		vr::DrawInfo::Indices indices;
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