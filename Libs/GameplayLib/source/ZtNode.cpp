#include "Zinet/GameplayLib/ZtNode.hpp"

namespace zt::gameplay_lib
{

	Vector2f Node::getAbsolutePosition() const
	{
		if (!parentNode.expired())
		{
			auto parentNodeAsShared = parentNode.lock();
			return position + parentNodeAsShared->getAbsolutePosition();
		}
		else
		{
			return position;
		}
	}

}