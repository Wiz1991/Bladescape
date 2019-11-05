#include "BallSystem.h"
#include <xyginext/ecs/components/Transform.hpp>
#include <xyginext/ecs/Scene.hpp>
BallSystem::BallSystem(xy::MessageBus& msgBus) :
	xy::System(msgBus, typeid(BallSystem))
{
	requireComponent<Ball>();
	requireComponent<xy::Transform>();
}

void BallSystem::process(float dT)
{
	auto& entities = getEntities();
	for (auto entity : entities) {
		auto& ball = entity.getComponent<Ball>();
		switch (ball.state)
		{
		default:
		case Ball::State::Waiting:
			break;
		case Ball::State::Active:
		{
			auto& tX = entity.getComponent<xy::Transform>();
			tX.move(ball.velocity * Ball::speed * dT);

			auto bounds = sf::FloatRect(sf::Vector2f(0, 0), xy::DefaultSceneSize);
			if (!bounds.contains(tX.getPosition())) {
				getScene()->destroyEntity(entity)
			}
		}
		}
	}
}