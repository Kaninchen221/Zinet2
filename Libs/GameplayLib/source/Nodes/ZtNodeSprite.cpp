#include "Zinet/Gameplay/Nodes/ZtNodeSprite.hpp"

namespace zt::gameplay
{
	void NodeSprite::imGuiNodeInspect() ZINET_API_POST
	{
		texture.show();
	}
}