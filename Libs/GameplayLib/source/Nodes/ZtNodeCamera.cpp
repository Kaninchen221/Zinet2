#include "Zinet/Gameplay/Nodes/ZtNodeCamera.hpp"

#include <imgui.h>

namespace zt::gameplay
{

	void NodeCamera::imGui() ZINET_API_POST
	{
		ImGui::Text("Node Camera");
		camera.imGui();
	}

}