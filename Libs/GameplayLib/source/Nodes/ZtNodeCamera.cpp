#include "Zinet/Gameplay/Nodes/ZtNodeCamera.hpp"

#include "Zinet/Core/ZtImgui.hpp"

namespace zt::gameplay
{

	void NodeCamera::show()
	{
		// TODO: Write ThreadQueueSystem
		// Something like that:
		// class GameplayThreadQueueSystem : public ThreadQueueSystem
		//auto gameplayThreadQueueSystem = EngineContext::Get()->GetSystem<GameplayThreadQueueSystem>();
		//gameplayThreadQueueSystem.addCommand([position = tempPosition](ObjectHandle<NodeCamera> objectHandle) { objectHandle->setCameraPosition(position); })

		ImGui::TextCStr("Node Camera");
		camera.show();
	}

}