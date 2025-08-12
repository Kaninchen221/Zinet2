#include "Zinet/Gameplay/Nodes/ZtNodeCamera.hpp"

#include <imgui.h>

namespace zt::gameplay
{

	void NodeCamera::imGui()
	{
		ImGui::Text("Node Camera");
		camera.imGui();
	}

}