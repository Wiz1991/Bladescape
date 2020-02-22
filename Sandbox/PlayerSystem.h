#pragma once
#include <xyginext/ecs/System.hpp>
struct Player final {
	std::uint16_t mInput;
	std::uint16_t mPrevInput;
	enum class Direction :sf::Uint8
	{
		Left, Right
	};
	enum
	{
		JumpFlag = 0x1,
	};
	enum class State : sf::Uint8
	{
		Walking, Jumping, Dying, Dead, Disabled //disable the player during map transitions
	}state = State::Jumping;
	sf::Vector2f velocity;
	sf::Uint8 canLand;
	sf::Uint8 flags;

	float speed = 300.f;
	float airSpeed = 432.f;
};
class PlayerSystem final : public xy::System
{
public:
	PlayerSystem(xy::MessageBus& bus);

	void process(float dt);
	sf::Vector2f parseInput(std::uint16_t mask);
	void resolveCollision(xy::Entity);

	void collisionWalking(xy::Entity);
	void collisionJumping(xy::Entity);
	void collisionDying(xy::Entity);
	void applyVelocity(xy::Entity e, float dt);
};
