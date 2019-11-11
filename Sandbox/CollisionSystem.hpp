#pragma once

#include <xyginext/ecs/System.hpp>

#include <functional>
#include <set>

struct Manifold final
{
	sf::Vector2f normal;
	float penetration = 0.f;
};

struct CollisionComponent final
{
	bool dynamic = false;
	std::function<void(xy::Entity, xy::Entity, Manifold)> callback;
};

class CollisionSystem final : public xy::System
{
public:
	explicit CollisionSystem(xy::MessageBus&);

	void process(float) override;

private:

	std::set<std::pair<xy::Entity, xy::Entity>> mCollisions;
};