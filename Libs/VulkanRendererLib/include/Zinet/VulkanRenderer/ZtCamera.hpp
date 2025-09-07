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

	class  Camera
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

		void setPosition(const Vector3f& newPosition) noexcept { position = newPosition; }
		const auto& getPosition() const noexcept { return position; }

		void setLookingAt(const Vector3f& newLookingAt) noexcept { lookingAt = newLookingAt; }
		const auto& getLookingAt() const noexcept { return lookingAt; }

		void setUpVector(const Vector3f& newUpVector) noexcept { upVector = newUpVector; }
		const auto& getUpVector() const noexcept { return upVector; }

		MatrixT getPerspectiveMatrix() const noexcept;

		void setFieldOfView(const float& newFieldOfView) noexcept { fieldOfView = newFieldOfView; }
		const auto& getFieldOfView() const noexcept { return fieldOfView; }

		void setAspectRatio(const float& newAspectRatio) noexcept { aspectRatio = newAspectRatio; }
		const auto& getAspectRatio() const noexcept { return aspectRatio; }

		void setClipping(const Vector2f& newClipping) noexcept { clipping = newClipping; }
		const auto& getClipping() const noexcept { return clipping; }

	protected:

		Vector3f position{ 0.00001, 0, 1 };
		Vector3f lookingAt{ 0, 0, 0 };
		Vector3f upVector{ 0, 0, 1 };

		float fieldOfView{ 90.f };
		float aspectRatio{ 2.f };
		Vector2f clipping{ 0.01f, 100.f };

	};
}