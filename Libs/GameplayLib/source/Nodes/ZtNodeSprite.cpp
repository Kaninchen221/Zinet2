#include "Zinet/Gameplay/Nodes/ZtNodeSprite.hpp"

namespace zt::gameplay
{
	void NodeSprite::imGui() ZINET_API_POST
	{
		texture.show();
	}
}