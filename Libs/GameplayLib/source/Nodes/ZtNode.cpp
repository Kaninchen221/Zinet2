#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtArchive.hpp"
#include "Zinet/Core/ZtImgui.hpp"

namespace zt::gameplay
{

	void Node::show()
	{
		ImGui::TextFMT("Node: {}", getDisplayName());
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