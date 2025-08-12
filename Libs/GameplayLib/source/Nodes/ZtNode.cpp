#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtArchive.hpp"

#include <imgui.h>
#include <fmt/format.h>

namespace zt::gameplay
{

	void Node::imGui()
	{
		ImGui::Text(fmt::format("Node: {}", getDisplayName()).c_str());
	}

	bool Node::serialize(core::JsonArchive& archive)
	{
		if (!Object::serialize(archive))
			return false;

		archive.serialize("className", getClassName());

		return true;
	}

	bool Node::deserialize(core::JsonArchive& archive)
	{
		if (!Object::deserialize(archive))
			return false;

		return true;

	}

}