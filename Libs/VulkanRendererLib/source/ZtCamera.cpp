#include "Zinet/VulkanRenderer/ZtCamera.hpp"

namespace zt::vulkan_renderer
{
	Camera::MatrixT Camera::getViewMatrix() const noexcept
	{
		return glm::lookAt(position, lookingAt, upVector);
	}

	Camera::MatrixT Camera::getPerspectiveMatrix() const noexcept
	{
		return glm::perspective(glm::radians(fieldOfView), aspectRatio, clipping.x, clipping.y);
	}

}
