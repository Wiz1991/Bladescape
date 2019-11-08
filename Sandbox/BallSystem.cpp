#include "BallSystem.h"
#include <xyginext/ecs/components/Transform.hpp>
#include <xyginext/ecs/Scene.hpp>
#include "MessageIDs.h"
BallSystem::BallSystem(xy::MessageBus& msgBus) :
	xy::System(msgBus, typeid(BallSystem))
{
	requireComponent<Ball>();
	requireComponent<xy::Transform>();
}

void BallSystem::process(float dT)
{
	auto& entities = getEntities();
	for (auto entity : entities)
	{
		auto& ball = entity.getComponent<Ball>();
		switch (ball.state)
		{
		default:
		case Ball::State::Waiting:
			break;
		case Ball::State::Active:
		{
			auto& tx = entity.getComponent<xy::Transform>();
			tx.move(ball.velocity * Ball::speed * dT);

			sf::FloatRect bounds(sf::Vector2f(), xy::DefaultSceneSize);
			if (!bounds.contains(tx.getPosition()))
			{
				getScene()->destroyEntity(entity);

				auto* msg = postMessage<BallEvent>(MessageID::BallMessage);
				msg->action = BallEvent::Despawned;
				msg->position = tx.getPosition();
			}
		}
		break;
		}
	}
}