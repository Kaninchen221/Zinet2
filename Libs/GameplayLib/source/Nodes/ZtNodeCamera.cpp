#include "Zinet/Gameplay/Nodes/ZtNodeCamera.hpp"

#include "Zinet/Core/ZtImgui.hpp"

namespace zt::gameplay
{

	void NodeCamera::show()
	{
		// TODO: Move it to the NodeCamera class
		ImGui::TextCStr("Node Camera");
		camera.show();
	}

}