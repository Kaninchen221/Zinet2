#include "Zinet/VulkanRenderer/ZtTransform.hpp"

#include "Zinet/Core/ZtImgui.hpp"

namespace zt::vulkan_renderer
{

	Transform::MatrixT Transform::getMatrix() const noexcept
	{
		MatrixT matrix = MatrixT(1.0f);
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
			
			ImGui::TextCStr("Position");
			ImGui::SliderFloat("X", &position.x, -1000, 1000);
			ImGui::SliderFloat("Y", &position.y, -1000, 1000);
			ImGui::SliderFloat("Z", &position.z, -1000, 1000);
			ImGui::Separator();

			float rad = glm::radians(rotation);
			ImGui::SliderAngle("Rotation", &rad, -180, 180);
			rotation = glm::degrees(rad);
			ImGui::Separator();

			ImGui::TextCStr("Scale");
			ImGui::SliderFloat("X##1", &scale.x, 0, 1000);
			ImGui::SliderFloat("Y##1", &scale.y, 0, 1000);
			ImGui::SliderFloat("Z##1", &scale.z, 0, 1000);
			ImGui::Separator();

			ImGui::Unindent(16.0f);
		}

		ImGui::PopID();
#		endif 
	}

}