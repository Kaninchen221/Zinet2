#include "Zinet/VulkanRenderer/ZtCamera.hpp"

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

	void Camera::show()
	{
#		if ZINET_USE_IMGUI

		if (ImGui::CollapsingHeader("Properties"))
		{
			ImGui::Indent();
			if (ImGui::Button("Reset camera"))
			{
				position = Vector3f{ 0, 0, 150 };
				lookingAt = Vector3f{};
			}

			ImGui::SliderFloat3("Position", reinterpret_cast<float*>(&position), -1000, 1000);
			ImGui::SliderFloat3("Looking At", reinterpret_cast<float*>(&lookingAt), -1000, 1000);

			if (ImGui::Button(">"))
			{
				position.x += 10;
				lookingAt.x += 10;
			}
			ImGui::SameLine();

			if (ImGui::Button("<"))
			{
				position.x -= 10;
				lookingAt.x -= 10;
			}
			ImGui::SameLine();

			if (ImGui::Button("^"))
			{
				position.y += 10;
				lookingAt.y += 10;
			}
			ImGui::SameLine();

			if (ImGui::Button("v"))
			{
				position.y -= 10;
				lookingAt.y -= 10;
			}
			ImGui::Unindent();
		}

#		endif 
	}

}
