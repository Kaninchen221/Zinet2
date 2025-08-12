#include "Zinet/VulkanRenderer/ZtTransform.hpp"

#include <imgui.h>

namespace zt::vulkan_renderer
{

	Transform::MatrixT Transform::getMatrix() const noexcept
	{
		MatrixT matrix = glm::mat4(1.0f);
		matrix *= glm::translate(data.position);
		matrix *= glm::rotate(glm::radians(data.rotation), Vector3f{ 0.f, 0.f, 1.f });
		matrix *= glm::scale(data.scale);

		return matrix;
	}

	void Transform::imGui()
	{
#		if ZINET_USE_IMGUI
		ImGui::PushID(this);

		ImGui::Indent(16.0f);
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::SliderFloat3("Position", reinterpret_cast<float*>(&data.position), -1000, 1000);
			ImGui::SliderFloat("Rotation", &data.rotation, 0, 360);
			ImGui::SliderFloat3("Scale", reinterpret_cast<float*>(&data.scale), 0, 1000);
		}
		ImGui::Unindent(16.0f);

		ImGui::PopID();
#		endif 
	}

}