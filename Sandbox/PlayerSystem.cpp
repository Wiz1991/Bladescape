#include "PlayerSystem.h"

PlayerSystem::PlayerSystem(xy::MessageBus& mb)
	: xy::System(mb, typeid(PlayerSystem))
{
}

void PlayerSystem::process(float dT)
{
	auto& entities = getEntities();
	for (auto entity : entities) {
	}
}

void PlayerSystem::resolveCollisions(xy::Entity entity)
{
}

PlayerComponent::PlayerComponent()
{
}