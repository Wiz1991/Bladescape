#pragma once
#include <xyginext/ecs/System.hpp>
namespace PhysicsConsts {
	static constexpr float entityDrag = 0.4;
	static constexpr float acceleration = 120;
	static constexpr float gravity = 1700;
	static constexpr float terminalVelocity = 2000;
	static constexpr float jumpImpulse = 654;
}
namespace CollidingFlags {
	enum {
		None = 0,
		LeftHand = 0x1,
		RightHand = 0x2,
		Foot = 0x4
	};
}
class PlayerComponent {
public:
	PlayerComponent();
	sf::Uint32 prevInputFlags;
	sf::Uint32 inputFlags;
	float mAccelerationMultipler;
	sf::Vector2f velocity;

	sf::Uint32 collisionFlags = CollidingFlags::None;

private:
};

class PlayerSystem final : public xy::System
{
public:
	PlayerSystem(xy::MessageBus& mb);

	void process(float dT);
private:
	void resolveCollisions(xy::Entity entity);
	void processInput(xy::Entity entity);
	void applyVelocity(xy::Entity entity, float dt);
	void applyPhysics(xy::Entity entity, float dt);
};
