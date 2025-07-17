#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::vulkan_renderer
{
	class RendererContext;

	class ZINET_VULKAN_RENDERER_API Transform
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRTransform");

	public:

		Transform() ZINET_API_POST = default;
		Transform(const Transform& other) ZINET_API_POST = default;
		Transform(Transform&& other) ZINET_API_POST = default;
		~Transform() ZINET_API_POST = default;

		Transform& operator = (const Transform& other) ZINET_API_POST = default;
		Transform& operator = (Transform&& other) ZINET_API_POST = default;

		using MatrixT = glm::mat4;
		MatrixT getMatrix() const ZINET_API_POST;

		void setPosition(const Vector3f& newPosition) ZINET_API_POST { position = newPosition; }
		const auto& getPosition() const ZINET_API_POST { return position; }

		/// Degrees
		void setRotation(float newRotation) ZINET_API_POST { rotation = newRotation; }
		const auto& getRotation() const ZINET_API_POST { return rotation; }

		void setScale(const Vector3f& newScale) ZINET_API_POST { scale = newScale; }
		const auto& getScale() const ZINET_API_POST { return scale; }

		void imGui();

	protected:

		Vector3f position{ 0, 0, 0 };
		float rotation{ 0 };
		Vector3f scale{ 1, 1, 1 };

	};
}