#include "Zinet/Gameplay/Systems/ZtSystemTickable.hpp"

namespace zt::gameplay::system
{
	void Tickable::Init(core::ecs::WorldCommands commands)
	{
		commands.addResource(DeltaTime{ 0 });
		commands.addResource(TickableData{});
	}

	void Tickable::Update(DeltaTimeRes deltaTimeRes, TickableDataRes tickableDataRes)
	{
		deltaTimeRes->value = tickableDataRes->clock.restart().getAsSeconds();
	}
}