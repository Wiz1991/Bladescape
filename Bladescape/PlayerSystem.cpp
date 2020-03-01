#include "PlayerSystem.h"
#include <xyginext/ecs/components/Transform.hpp>
#include "CollisionSystem.hpp"
#include "StringUtils.h"
#include <xyginext/ecs/Scene.hpp>
#include "InputBinding.h"
#include <xyginext/ecs/components/SpriteAnimation.hpp>
#include "AnimationMap.h"
namespace
{
	const float initialJumpVelocity = 1060.f;
	const float minJumpVelocity = -initialJumpVelocity * 0.39;

	static constexpr float Gravity = 2300.f;
	static constexpr float MaxVelocity = 1000.f;
}
PlayerSystem::PlayerSystem(xy::MessageBus& bus) :
	xy::System(bus, typeid(PlayerSystem))
{
	requireComponent<Player>();
	requireComponent<CollisionComponent>();
	requireComponent<xy::Transform>();
}

void PlayerSystem::process(float dt)
{
	auto& entities = getEntities();
	for (auto& entity : entities) {
		auto& player = entity.getComponent<Player>();

		auto& tx = entity.getComponent<xy::Transform>();
		resolveCollision(entity);
		if ((player.mInput & InputFlag::Jump) == 0) {
			player.flags |= player.JumpFlag;
		}

		if (player.state == Player::State::Walking && player.mPrevInput != InputFlag::Jump && player.flags & player.JumpFlag)
		{
			if (player.mInput & InputFlag::Jump)
			{
				player.state = Player::State::Jumping;
				player.velocity.y = -initialJumpVelocity;
				player.flags &= ~player.JumpFlag;
			}
		}
		else if (player.state == Player::State::Jumping)
		{
			//variable jump height
			if ((player.mInput & InputFlag::Jump) == 0
				&& player.velocity.y < minJumpVelocity)
			{
				player.velocity.y = minJumpVelocity;
			}

			tx.move({ 0.f, player.velocity.y * dt });
			player.velocity.y += Gravity * dt;
			player.velocity.y = std::min(player.velocity.y, MaxVelocity);
		}
		if (player.state == Player::State::Walking)
		{
			auto motion = parseInput(player.mInput);
			tx.move(player.speed * motion * dt);
			player.velocity.x = player.speed * motion.x;
		}
		if (player.state == Player::State::Jumping) {
			auto motion = parseInput(player.mInput);
			tx.move(player.airSpeed * motion * dt);
			player.velocity.x = player.speed * motion.x;
		}

		if (player.state == Player::State::Jumping) {
			entity.getComponent<xy::SpriteAnimation>().play(AnimID::Player::Jump);
			auto scale = tx.getScale();
			if (player.velocity.x > 0) {
				scale.x = scale.y;
			}
			else if (player.velocity.x < 0) {
				scale.x = -scale.y;
			}
			tx.setScale(scale);
		}
		else if (player.state == Player::State::Walking) {
			entity.getComponent<xy::SpriteAnimation>().play(AnimID::Player::Run);
			auto scale = tx.getScale();
			if (player.velocity.x > 0) {
				scale.x = scale.y;
			}
			else if (player.velocity.x < 0) {
				scale.x = -scale.y;
			}
			else {
				entity.getComponent<xy::SpriteAnimation>().play(AnimID::Player::Idle);
			}

			tx.setScale(scale);
		}

		player.mPrevInput = player.mInput;
	}
}

sf::Vector2f PlayerSystem::parseInput(std::uint16_t mask)
{
	sf::Vector2f motion;
	if (mask & InputFlag::Left)
		motion.x = -1;
	if (mask & InputFlag::Right)
		motion.x = 1;
	return motion;
}

void PlayerSystem::resolveCollision(xy::Entity entity)
{
	auto& player = entity.getComponent<Player>();
	switch (player.state)
	{
	default:break;
	case Player::State::Disabled:
		return;
	case Player::State::Dying:
		collisionDying(entity);
		return;
	case Player::State::Jumping:
		collisionJumping(entity);
		return;
	case Player::State::Walking:
		collisionWalking(entity);
		return;
	}
}

void PlayerSystem::collisionWalking(xy::Entity entity)
{
	const auto& hitboxes = entity.getComponent<CollisionComponent>().getHitboxes();
	auto hitboxCount = entity.getComponent<CollisionComponent>().getHitboxCount();

	auto& tx = entity.getComponent<xy::Transform>();
	auto& player = entity.getComponent<Player>();

	//check for collision and resolve
	for (auto i = 0u; i < hitboxCount; ++i)
	{
		const auto& hitbox = hitboxes[i];
		if (hitbox.getType() == CollisionType::Player)
		{
			auto& manifolds = hitbox.getManifolds();
			auto collisionCount = hitbox.getCollisionCount();
			for (auto j = 0u; j < collisionCount; ++j)
			{
				const auto& man = manifolds[j];
				switch (man.otherType)
				{
				default: break;

				case CollisionType::Solid:
					tx.move(man.normal * man.penetration);
					break;
				}
			}
		}
		else if (hitbox.getType() == CollisionType::Foot)
		{
			//remove any collisions which should still
			//let the player enter freefall
			auto collisionCount = hitbox.getCollisionCount();

			if (collisionCount == 0)
			{
				entity.getComponent<Player>().state = Player::State::Jumping; //enter freefall
				return;
			}
		}
	}
}

void PlayerSystem::collisionJumping(xy::Entity entity)
{
	const auto& hitboxes = entity.getComponent<CollisionComponent>().getHitboxes();
	auto hitboxCount = entity.getComponent<CollisionComponent>().getHitboxCount();

	auto& player = entity.getComponent<Player>();
	auto& tx = entity.getComponent<xy::Transform>();

	//check for collision and resolve
	for (auto i = 0u; i < hitboxCount; ++i)
	{
		const auto& hitbox = hitboxes[i];
		if (hitbox.getType() == CollisionType::Player)
		{
			auto manifolds = hitbox.getManifolds();
			auto collisionCount = hitbox.getCollisionCount();

			for (auto j = 0u; j < collisionCount; ++j)
			{
				const auto& man = manifolds[j];
				switch (man.otherType)
				{
				default:
					break;
				case CollisionType::Solid:
					if (man.normal.y < 0 && player.velocity.y > 200)
					{
						player.state = Player::State::Walking;
						player.velocity.y = 0.f;
						return;
					}
					else if (man.normal.y > 0) //bonk head and fall
					{
						player.velocity = -player.velocity * 0.1f;
					}

					tx.move(man.normal * man.penetration);
					break;
				}
			}
		}
		else if (hitbox.getType() == CollisionType::Foot)
		{
			auto manifolds = hitbox.getManifolds();
			auto collisionCount = hitbox.getCollisionCount();

			for (auto j = 0u; j < collisionCount; ++j)
			{
				auto man = manifolds[j];
			}
		}
	}
}

void PlayerSystem::collisionDying(xy::Entity)
{
}

void PlayerSystem::applyVelocity(xy::Entity e, float dt)
{
	auto& tx = e.getComponent<xy::Transform>();
	auto& player = e.getComponent<Player>();

	tx.move(player.velocity * dt);
}