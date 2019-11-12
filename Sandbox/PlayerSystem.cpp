#include "PlayerSystem.h"
#include <xyginext/ecs/components/Transform.hpp>
#include "CollisionSystem.hpp"
#include "InputBinding.h"
PlayerSystem::PlayerSystem(xy::MessageBus& bus) :
	xy::System(bus, typeid(PlayerSystem))
{
	requireComponent<PlayerComponent>();
	requireComponent<CollisionComponent>();
	requireComponent<xy::Transform>();
}

void PlayerSystem::process(float dt)
{
	auto& entities = getEntities();
	for (auto& entity : entities) {
		auto& player = entity.getComponent<PlayerComponent>();

		if (player.mInput & InputBinding::Left) {
			player.velocity.x -= player.acceleration;
		}
		if (player.mInput & InputBinding::Right) {
			player.velocity.x += player.acceleration;
		}

		player.mPrevInput = player.mInput;
		applyVelocity(entity, dt);
	}
}

void PlayerSystem::applyVelocity(xy::Entity e, float dt)
{
	auto& tx = e.getComponent<xy::Transform>();
	auto& player = e.getComponent<PlayerComponent>();

	tx.move(player.velocity * dt);

	player.velocity.x *= player.xDrag;
}