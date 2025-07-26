#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include <imgui.h>
#include <fmt/format.h>

namespace zt::gameplay
{

	void Node::imGui() ZINET_API_POST
	{
		ImGui::Text(fmt::format("Node: {}", getDisplayName()).c_str());
	}

}