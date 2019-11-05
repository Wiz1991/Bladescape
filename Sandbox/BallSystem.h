#pragma once
#include <xyginext/ecs/System.hpp>
#include <SFML/System/Vector2.hpp>

struct Ball final {
	enum class State {
		Waiting,
		Active
	}state = State::Waiting;
	static constexpr float speed = 800.f;
	sf::Vector2f velocity = { 0.f,-1.f };
};
struct Paddle final {
	xy::Entity ball;
};
class BallSystem final : public xy::System
{
public:
	explicit BallSystem(xy::MessageBus& msgBus);
	void process(float dT);
};
