#include "CollisionSystem.h"
#include <xyginext/ecs/components/Transform.hpp>
#include <xyginext/ecs/components/BroadPhaseComponent.hpp>
#include <xyginext/ecs/Scene.hpp>
#include <xyginext/ecs/systems/DynamicTreeSystem.hpp>

CollisionSystem::CollisionSystem(xy::MessageBus& bus) :
	xy::System(bus, typeid(CollisionSystem))
{
	requireComponent<xy::Transform>();
	requireComponent<xy::BroadphaseComponent>();
	requireComponent<Collider>();
}

void CollisionSystem::process(float)
{
	mCollisions.clear();

	broadPhase();
	narrowPhase();
}

void CollisionSystem::broadPhase()
{
	auto& entities = getEntities();
	for (auto entity : entities) {
		auto& collider = entity.getComponent<Collider>();
		if (collider.dynamic) {
			auto bounds = entity.getComponent<xy::Transform>().getTransform().transformRect(entity.getComponent<xy::BroadphaseComponent>().getArea());
			auto others = getScene()->getSystem<xy::DynamicTreeSystem>().query(bounds);
			for (auto other : others) {
				if (other != entity) {
					auto otherBounds = entity.getComponent<xy::Transform>().getTransform().transformRect(
						other.getComponent<xy::BroadphaseComponent>().getArea()
					);
					if (bounds.intersects(otherBounds)) {
						mCollisions.insert(std::minmax(entity, other));
					}
				}
			}
		}
	}
}

void CollisionSystem::narrowPhase()
{
	for (auto& c : mCollisions)
	{
		auto itemA = c.first;
		auto itemB = c.second;

		auto& txA = itemA.getComponent<xy::Transform>();
		auto& txB = itemB.getComponent<xy::Transform>();

		const auto& collisionA = itemA.getComponent<Collider>();
		const auto& collisionB = itemB.getComponent<Collider>();

		//manifold calculation
		auto boundsA = txA.getTransform().transformRect(itemA.getComponent<xy::BroadphaseComponent>().getArea());
		auto boundsB = txB.getTransform().transformRect(itemB.getComponent<xy::BroadphaseComponent>().getArea());

		sf::FloatRect intersection;
		boundsA.intersects(boundsB, intersection);

		sf::Vector2f normal = txB.getPosition() - txA.getPosition();

		Manifold manifold;
		if (intersection.width < intersection.height)
		{
			manifold.normal.x = (normal.x < 0) ? 1.f : -1.f;
			manifold.penetration = intersection.width;
		}
		else
		{
			manifold.normal.y = (normal.y < 0) ? 1.f : -1.f;
			manifold.penetration = intersection.height;
		}

		//only correct position if dynamic objects
		if (collisionA.dynamic)
		{
			txA.move(manifold.normal * manifold.penetration);
		}

		if (collisionA.callback)
		{
			collisionA.callback(itemA, itemB, manifold);
		}

		//flip the normal for the other item
		manifold.normal = -manifold.normal;

		if (collisionB.dynamic)
		{
			txB.move(manifold.normal * manifold.penetration);
		}

		if (collisionB.callback)
		{
			collisionB.callback(itemB, itemA, manifold);
		}
	}
}