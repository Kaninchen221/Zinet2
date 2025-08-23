#include "Zinet/Gameplay/Nodes/ZtNodeCamera.hpp"

#include "Zinet/Core/ZtImgui.hpp"

namespace zt::gameplay
{

	void NodeCamera::show()
	{
		ImGui::TextCStr("Node Camera");
		camera.show();
	}

}