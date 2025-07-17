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

		Camera() ZINET_API_POST = default;
		Camera(const Camera& other) ZINET_API_POST = default;
		Camera(Camera&& other) ZINET_API_POST = default;
		~Camera() ZINET_API_POST = default;

		Camera& operator = (const Camera& other) ZINET_API_POST = default;
		Camera& operator = (Camera&& other) ZINET_API_POST = default;

		using MatrixT = glm::mat4;
		MatrixT getViewMatrix() const ZINET_API_POST;

		void setPosition(const Vector3f& newPosition) ZINET_API_POST { position = newPosition; }
		const auto& getPosition() const ZINET_API_POST { return position; }

		void setLookingAt(const Vector3f& newLookingAt) ZINET_API_POST { lookingAt = newLookingAt; }
		const auto& getLookingAt() const ZINET_API_POST { return lookingAt; }

		void setUpVector(const Vector3f& newUpVector) ZINET_API_POST { upVector = newUpVector; }
		const auto& getUpVector() const ZINET_API_POST { return upVector; }

		MatrixT getPerspectiveMatrix() const ZINET_API_POST;

		void setFieldOfView(const float& newFieldOfView) ZINET_API_POST { fieldOfView = newFieldOfView; }
		const auto& getFieldOfView() const ZINET_API_POST { return fieldOfView; }

		void setAspectRatio(const float& newAspectRatio) ZINET_API_POST { aspectRatio = newAspectRatio; }
		const auto& getAspectRatio() const ZINET_API_POST { return aspectRatio; }

		void setClipping(const Vector2f& newClipping) ZINET_API_POST { clipping = newClipping; }
		const auto& getClipping() const ZINET_API_POST { return clipping; }

		void imGui();

	protected:

		Vector3f position{ 0.00001, 0, 1 };
		Vector3f lookingAt{ 0, 0, 0 };
		Vector3f upVector{ 0, 0, 1 };

		float fieldOfView{ 90.f };
		float aspectRatio{ 2.f };
		Vector2f clipping{ 0.01f, 100.f };
	};
}