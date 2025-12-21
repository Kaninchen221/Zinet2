#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/VulkanRenderer/ZtCamera.hpp"

#include "Zinet/Core/ZtTime.hpp"

namespace zt::gameplay
{
	using Camera = vulkan_renderer::Camera;

	class ZINET_GAMEPLAY_API CameraManager
	{

	public:
		CameraManager() noexcept;
		CameraManager(const CameraManager& other) noexcept = default;
		CameraManager(CameraManager&& other) noexcept = default;
		~CameraManager() noexcept = default;

		CameraManager& operator = (const CameraManager& other) noexcept = default;
		CameraManager& operator = (CameraManager&& other) noexcept = default;

		void setCamera(const Camera& newCamera) { camera = newCamera; }
		const auto& getCamera() const noexcept { return camera; }

		void blendTo(const Camera& target, const core::Time& time);

		bool isBlending() const noexcept { return blending; }

		void update(const core::Time& time);

	private:

		Camera camera;

		// Blending
		bool blending = false;
		Camera blendTarget;
		core::Time blendTime;
		core::Time blendElapsedTime;
		Vector3f positionDiff;
		Vector3f lookingAtDiff;
		float fovDiff;
	};
}