#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::vulkan_renderer
{
	class RendererContext;

	class ZINET_VULKAN_RENDERER_API Camera
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRCamera");

	public:
		using MatrixT = glm::mat4;

		Camera() noexcept = default;
		Camera(const Camera& other) noexcept = default;
		Camera(Camera&& other) noexcept = default;
		~Camera() noexcept = default;

		Camera& operator = (const Camera& other) noexcept = default;
		Camera& operator = (Camera&& other) noexcept = default;

		MatrixT getViewMatrix() const noexcept;

		void setPosition(const Vector3f& newPosition) noexcept { data.position = newPosition; }
		const auto& getPosition() const noexcept { return data.position; }

		void setLookingAt(const Vector3f& newLookingAt) noexcept { data.lookingAt = newLookingAt; }
		const auto& getLookingAt() const noexcept { return data.lookingAt; }

		void setUpVector(const Vector3f& newUpVector) noexcept { data.upVector = newUpVector; }
		const auto& getUpVector() const noexcept { return data.upVector; }

		MatrixT getPerspectiveMatrix() const noexcept;

		void setFieldOfView(const float& newFieldOfView) noexcept { data.fieldOfView = newFieldOfView; }
		const auto& getFieldOfView() const noexcept { return data.fieldOfView; }

		void setAspectRatio(const float& newAspectRatio) noexcept { data.aspectRatio = newAspectRatio; }
		const auto& getAspectRatio() const noexcept { return data.aspectRatio; }

		void setClipping(const Vector2f& newClipping) noexcept { data.clipping = newClipping; }
		const auto& getClipping() const noexcept { return data.clipping; }

		void imGui();

	protected:

		struct {
			Vector3f position{ 0.00001, 0, 1 };
			Vector3f lookingAt{ 0, 0, 0 };
			Vector3f upVector{ 0, 0, 1 };

			float fieldOfView{ 90.f };
			float aspectRatio{ 2.f };
			Vector2f clipping{ 0.01f, 100.f };
		} data;

	};
}