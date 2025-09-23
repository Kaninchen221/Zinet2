#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/VulkanRenderer/ZtCamera.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorInfo.hpp"

namespace zt::gameplay
{
	class  NodeCamera : public Node
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::NodeCamera");

	public:

		using CameraT = vulkan_renderer::Camera;

		NodeCamera() = default;
		NodeCamera(const NodeCamera& other) = default;
		NodeCamera(NodeCamera&& other) noexcept = default;
		~NodeCamera() noexcept = default;

		NodeCamera& operator = (const NodeCamera& other) = default;
		NodeCamera& operator = (NodeCamera&& other) noexcept = default;

		void onCreate(ObjectWeakHandle<Object> newSelf) override;

		void onDestroy() override;

		void show() override;

		virtual void update(float deltaTime) override;

		void setCamera(const CameraT& newCamera) { camera = newCamera; }
		const auto& getCamera() const noexcept { return camera; }
		auto& getCamera() noexcept { return camera; }

		vulkan_renderer::DescriptorInfo getDescriptorInfo();

	protected:

		CameraT camera;
		vulkan_renderer::Buffer buffer{ nullptr };

		struct BufferData
		{
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 perspective;
		};
	};

}