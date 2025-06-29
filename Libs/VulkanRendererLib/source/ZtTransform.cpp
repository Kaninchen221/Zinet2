#include "Zinet/VulkanRenderer/ZtTransform.hpp"

namespace zt::vulkan_renderer
{

	Transform::MatrixT Transform::getMatrix() const noexcept
	{
		MatrixT matrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(0, 0, 1));
		matrix *= glm::translate(position);
		matrix *= glm::rotate(glm::radians(rotation), Vector3f{ 0.f, 0.f, 1.f });
		matrix *= glm::scale(scale);

		return matrix;
	}

}