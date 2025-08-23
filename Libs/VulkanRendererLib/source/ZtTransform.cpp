#include "Zinet/VulkanRenderer/ZtTransform.hpp"

#include <imgui.h>

namespace zt::vulkan_renderer
{

	Transform::MatrixT Transform::getMatrix() const noexcept
	{
		MatrixT matrix = glm::mat4(1.0f);
		matrix *= glm::translate(position);
		matrix *= glm::rotate(glm::radians(rotation), Vector3f{ 0.f, 0.f, 1.f });
		matrix *= glm::scale(scale);

		return matrix;
	}

	void Transform::show()
	{
#		if ZINET_USE_IMGUI
		ImGui::PushID(this);

		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Indent(16.0f);
			ImGui::SliderFloat3("Position", reinterpret_cast<float*>(&position), -1000, 1000);
			ImGui::SliderFloat("Rotation", &rotation, 0, 360);
			ImGui::SliderFloat3("Scale", reinterpret_cast<float*>(&scale), 0, 1000);
			ImGui::Unindent(16.0f);
		}

		ImGui::PopID();
#		endif 
	}

}