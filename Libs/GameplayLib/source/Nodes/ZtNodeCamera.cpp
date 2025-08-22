#include "Zinet/Gameplay/Nodes/ZtNodeCamera.hpp"

#include "Zinet/Core/ZtImgui.hpp"

namespace zt::gameplay
{

	void NodeCamera::imGui()
	{
		ImGui::TextCStr("Node Camera");
		camera.imGui();
	}

}