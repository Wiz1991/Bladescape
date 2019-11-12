#pragma once
#include <xyginext/ecs/System.hpp>
struct PlayerComponent final {
	std::uint16_t mInput;
	std::uint16_t mPrevInput;
	enum {
		Falling, Running, Jumping, Dying
	}state = Falling;

	sf::Vector2f velocity = { 0,0 };
	static constexpr float xDrag = 0.81;
	float jumpImpulse = -1500;
	static constexpr float acceleration = 32.f;
};
class PlayerSystem final : public xy::System
{
public:
	PlayerSystem(xy::MessageBus& bus);

	void process(float dt);
	void applyVelocity(xy::Entity e, float dt);
};
