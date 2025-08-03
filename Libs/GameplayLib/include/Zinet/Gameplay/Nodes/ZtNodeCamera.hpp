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

		NodeCamera() ZINET_API_POST = default;
		NodeCamera(const NodeCamera& other) ZINET_API_POST = default;
		NodeCamera(NodeCamera&& other) ZINET_API_POST = default;
		~NodeCamera() ZINET_API_POST = default;

		NodeCamera& operator = (const NodeCamera& other) ZINET_API_POST = default;
		NodeCamera& operator = (NodeCamera&& other) ZINET_API_POST = default;

		void imGui() ZINET_API_POST override;

		void setCamera(const CameraT& newCamera) ZINET_API_POST { camera = newCamera; }
		const auto& getCamera() const ZINET_API_POST { return camera; }
		auto& getCamera() ZINET_API_POST { return camera; }

	protected:

		CameraT camera;

	};

}