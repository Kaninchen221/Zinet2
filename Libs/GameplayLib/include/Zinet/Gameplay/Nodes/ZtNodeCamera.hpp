#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/VulkanRenderer/ZtCamera.hpp"

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API NodeCamera : public Node
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::NodeCamera");

	public:

		using CameraT = vulkan_renderer::Camera;

		NodeCamera() = default;
		NodeCamera(const NodeCamera& other) = default;
		NodeCamera(NodeCamera&& other) noexcept = default;
		~NodeCamera() noexcept = default;

		NodeCamera& operator = (const NodeCamera& other) = default;
		NodeCamera& operator = (NodeCamera&& other) noexcept = default;

		void imGui() override;

		void setCamera(const CameraT& newCamera) { camera = newCamera; }
		const auto& getCamera() const noexcept { return camera; }
		auto& getCamera() noexcept { return camera; }

	protected:

		CameraT camera;

	};

}