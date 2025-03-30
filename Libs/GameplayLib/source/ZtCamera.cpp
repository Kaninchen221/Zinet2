#include "Zinet/GameplayLib/ZtCamera.hpp"

namespace zt::gameplay_lib
{

	void Camera::create(const Vector2i& newSize)
	{
		size = newSize;
		viewportRenderTarget.createEmpty(size, sf::ColorFormat::R8G8B8A8_SRGB);
	}

	Vector2f Camera::mousePositionNormToWorld(const Vector2f& mousePositionNorm) const
	{
		const auto viewportSize = getSize();
		const auto lookAt = getLookAt();

		const Vector2f mousePosInWorld = { (mousePositionNorm.x * viewportSize.x) - (viewportSize.x / 2.f) + lookAt.x, (mousePositionNorm.y * viewportSize.y) - (viewportSize.y / 2.f) + lookAt.y };
		return mousePosInWorld;
	}

}