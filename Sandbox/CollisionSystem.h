#pragma once
#include <xyginext/ecs/System.hpp>
#include <functional>
#include <set>
struct Manifold final {
	sf::Vector2f normal;
	float penetration = 0.f;
};
struct Collider final {
	bool dynamic = false;
	std::function<void(xy::Entity, xy::Entity, Manifold)>callback;
};
class CollisionSystem final : public xy::System
{
public:
	explicit CollisionSystem(xy::MessageBus& msgBus);

	void process(float dT);
	void broadPhase();
	void narrowPhase();

private:
	std::set<std::pair<xy::Entity, xy::Entity>> mCollisions;
};
