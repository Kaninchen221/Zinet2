#include "Zinet/Gameplay/Nodes/ZtNodeCamera.hpp"

#include "Zinet/Core/ZtImgui.hpp"

namespace zt::gameplay
{

	void NodeCamera::show()
	{
#		if ZINET_USE_IMGUI
		Node::show();

		if (ImGui::CollapsingHeader("Properties"))
		{
			ImGui::Indent();
			if (ImGui::Button("Reset camera"))
			{
				camera.setPosition(Vector3f{0, 0, 150});
				camera.setLookingAt(Vector3f{});
			}

			Vector3f position = camera.getPosition();
			ImGui::SliderFloat3("Position", reinterpret_cast<float*>(&position), -1000, 1000);

			Vector3f lookingAt = camera.getLookingAt();
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

			camera.setPosition(position);
			camera.setLookingAt(lookingAt);

			ImGui::Unindent();
		}

#		endif 
	}

}