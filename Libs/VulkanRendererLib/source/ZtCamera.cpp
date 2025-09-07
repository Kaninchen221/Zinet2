﻿#include "Zinet/VulkanRenderer/ZtCamera.hpp"

#include <imgui.h>

namespace zt::vulkan_renderer
{
	Camera::MatrixT Camera::getViewMatrix() const noexcept
	{
		return glm::lookAt(position, lookingAt, upVector);
	}

	Camera::MatrixT Camera::getPerspectiveMatrix() const noexcept
	{
		auto perspective = glm::perspective(glm::radians(fieldOfView), aspectRatio, clipping.x, clipping.y);
		perspective[1][1] *= -1;
		return perspective;
	}

}
