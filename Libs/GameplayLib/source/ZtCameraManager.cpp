#include "Zinet/Gameplay/ZtCameraManager.hpp"

namespace zt::gameplay
{
	CameraManager::CameraManager() noexcept
	{
		camera.setPosition(Vector3f(0.00001, 0, 150));
		camera.setLookingAt(Vector3f(0.0f, 0.0f, 0.0f));
		camera.setUpVector(Vector3f(0, 1, 0));

		camera.setFieldOfView(45.f);
		camera.setAspectRatio(1);
		camera.setClipping(Vector2f{ 0.0000001f, 10000000.0f });
	}

	void CameraManager::blendTo(const Camera& target, const core::Time& time)
	{
		blending = true;
		blendTarget = target;
		blendTime = time;
		blendElapsedTime = core::Time{};

		positionDiff = blendTarget.getPosition() - camera.getPosition();
		lookingAtDiff = blendTarget.getLookingAt() - camera.getLookingAt();
		fovDiff = blendTarget.getFieldOfView() - camera.getFieldOfView();
	}

	void CameraManager::update(const core::Time& time)
	{
		if (!blending)
			return;

		auto shouldEndBlend = [this]()
		{
			if (blendElapsedTime >= blendTime)
			{
				camera.setPosition(blendTarget.getPosition());
				camera.setLookingAt(blendTarget.getLookingAt());
				camera.setFieldOfView(blendTarget.getFieldOfView());

				blending = false;
				blendElapsedTime = {};
				return true;
			}

			return false;
		};

		if (shouldEndBlend())
			return;

		auto timeRatio = time.getAsSeconds() / blendTime.getAsSeconds();

		camera.setPosition(camera.getPosition() + positionDiff * timeRatio);
		camera.setLookingAt(camera.getLookingAt() + lookingAtDiff * timeRatio);
		camera.setFieldOfView(camera.getFieldOfView() + fovDiff * timeRatio);

		blendElapsedTime += time;

		if (shouldEndBlend())
			return;
	}
}