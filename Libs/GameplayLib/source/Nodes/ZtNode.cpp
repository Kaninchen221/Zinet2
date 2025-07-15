#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include <imgui.h>

namespace zt::gameplay
{

	void Node::imGuiNodeInspect() ZINET_API_POST
	{
		ImGui::Text("imGui Node Inspect");
	}

}