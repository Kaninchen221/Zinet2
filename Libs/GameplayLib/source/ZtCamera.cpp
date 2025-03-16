#include "Zinet/GameplayLib/ZtCamera.hpp"

namespace zt::gameplay_lib
{

	void Camera::create(const Vector2i& size)
	{
		viewportRenderTarget.createEmpty(size, sf::ColorFormat::R8G8B8A8_SRGB);
	}

}