#include "PlayerSystem.h"
#include <xyginext/ecs/components/Transform.hpp>
#include "CollisionSystem.hpp"
#include <xyginext/ecs/components/SpriteAnimation.hpp>
#include "InputBinding.h"
#include "AnimationData.h"
#include <xyginext/util/Vector.hpp>
PlayerSystem::PlayerSystem(xy::MessageBus& mb)
	: xy::System(mb, typeid(PlayerSystem))
{
	requireComponent<PlayerComponent>();
}

void PlayerSystem::process(float dT)
{
	auto& entities = getEntities();
	for (auto entity : entities) {
		processInput(entity);
		applyVelocity(entity, dT);
		resolveCollisions(entity);
		applyPhysics(entity, dT);
	}
}

void PlayerSystem::applyPhysics(xy::Entity entity, float dt)
{
	auto& player = entity.getComponent<PlayerComponent>();

	if ((player.collisionFlags & CollidingFlags::None) == 0) {
		if ((player.inputFlags & InputFlag::Jump) && (player.prevInputFlags & InputFlag::Jump)) {
			player.velocity.y *= 0.3;
		}
		float multiplier = (player.velocity.y < 0) ? 0.7f : 1.2f;
		player.velocity.y = std::min(player.velocity.y + PhysicsConsts::gravity * dt, PhysicsConsts::terminalVelocity);
	}
}
void PlayerSystem::resolveCollisions(xy::Entity entity)
{
	const auto& hitboxes = entity.getComponent<CollisionComponent>().getHitboxes();
	auto hitboxCount = entity.getComponent<CollisionComponent>().getHitboxCount();

	auto& player = entity.getComponent<PlayerComponent>();
	auto& tX = entity.getComponent<xy::Transform>();

	for (int i = 0; i < hitboxCount; i++) {
		const auto& hitbox = hitboxes[i];
		if (hitbox.getType() == CollisionType::Player) {
			auto manifolds = hitbox.getManifolds();

			auto collisionCount = hitbox.getCollisionCount();
			for (auto j = 0u; j < collisionCount; j++) {
				const auto& manifold = manifolds[j];
				switch (manifold.otherType) {
				default:
					break;
				case CollisionType::Platform:
				case CollisionType::Solid:
					tX.move(manifold.normal * manifold.penetration);
					if (manifold.normal.y != 0)
					{
						player.velocity = {};
					}
					else
					{
						player.velocity = xy::Util::Vector::reflect(player.velocity, manifold.normal);
					}
					break;
				}
			}
		}
		else if (hitbox.getType() == CollisionType::Foot) {
			auto collisionCount = hitbox.getCollisionCount();
			auto loopCount = collisionCount;
			auto& manifolds = hitbox.getManifolds();
			for (auto i = 0; i < loopCount; i++) {
				const auto& manifold = manifolds[i];
				switch (manifold.otherType) {
				default:
					break;
				}
			}
			if (collisionCount == 0) {
				player.collisionFlags &= CollidingFlags::Foot;
			}
			else if (collisionCount == 1) {
				player.collisionFlags |= CollisionFlags::Foot;
				player.velocity.y = 0;
			}
		}
		else if (hitbox.getType() == CollisionType::RightHand) {
			auto collisionCount = hitbox.getCollisionCount();

			if (collisionCount == 0) {
				player.collisionFlags &= CollisionFlags::RightHand;
			}
			else if (collisionCount == 1) {
				player.collisionFlags |= CollisionFlags::RightHand;
				player.velocity.x = 0;
			}
		}
	}
}

void PlayerSystem::processInput(xy::Entity entity)
{
	auto& player = entity.getComponent<PlayerComponent>();
	auto& tx = entity.getComponent<xy::Transform>();
	auto scale = tx.getScale();
	if (player.inputFlags & InputFlag::Left) {
		player.velocity.x -= PhysicsConsts::acceleration;
		scale.x = -scale.y;
	}
	if ((player.inputFlags & InputFlag::Right) && (player.collisionFlags & CollidingFlags::RightHand) == 0) {
		player.velocity.x += PhysicsConsts::acceleration;
		scale.x = scale.y;
	}
	tx.setScale(scale);

	if ((player.inputFlags & InputFlag::Jump)) {
		player.velocity.y -= PhysicsConsts::jumpImpulse;
	}

	if (player.velocity.x > 1 || player.velocity.x < -1) {
		entity.getComponent<xy::SpriteAnimation>().play(AnimID::Player::Run);
	}
	else {
		entity.getComponent<xy::SpriteAnimation>().play(AnimID::Player::Idle);
	}

	player.prevInputFlags = player.inputFlags;
}
void PlayerSystem::applyVelocity(xy::Entity entity, float dt)
{
	auto& tx = entity.getComponent<xy::Transform>();
	auto& player = entity.getComponent<PlayerComponent>();

	tx.move(player.velocity * dt);
	player.velocity.x *= PhysicsConsts::entityDrag;
}

PlayerComponent::PlayerComponent()
	: inputFlags(0),
	prevInputFlags(0),
	mAccelerationMultipler(0)

{
}