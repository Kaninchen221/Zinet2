#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

#include "Zinet/Math/ZtVecTypes.hpp"
#include "Zinet/Math/ZtMath.hpp"

namespace zt::vulkan_renderer
{
	class RendererContext;

	// TODO: Test if the Transform class has a size of its members
	class  Transform
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRTransform");

	public:
		using MatrixT = Matrix;

		Transform(const Vector3f& position, float rotation, const Vector3f& scale) noexcept :
			position{ position },
			rotation{ rotation },
			scale{ scale }
		{
		}

		Transform() noexcept = default;
		Transform(const Transform& other) noexcept = default;
		Transform(Transform&& other) noexcept = default;
		~Transform() noexcept = default;

		Transform& operator = (const Transform& other) noexcept = default;
		Transform& operator = (Transform&& other) noexcept = default;

		MatrixT getMatrix() const noexcept;

		void setPosition(const Vector3f& newPosition) noexcept { position = newPosition; }
		const auto& getPosition() const noexcept { return position; }
		auto& getPosition() noexcept { return position; }

		/// Degrees
		void setRotation(float newRotation) noexcept { rotation = newRotation; }
		const auto& getRotation() const noexcept { return rotation; }
		auto& getRotation() noexcept { return rotation; }

		void setScale(const Vector3f& newScale) noexcept { scale = newScale; }
		const auto& getScale() const noexcept { return scale; }
		auto& getScale() noexcept { return scale; }

		void show();

	protected:

		Vector3f position{ 0, 0, 0 };
		float rotation{ 0 };
		Vector3f scale{ 1, 1, 1 };

	};
}