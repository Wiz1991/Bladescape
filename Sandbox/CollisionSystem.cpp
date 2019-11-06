#include "CollisionSystem.h"
#include <xyginext/ecs/components/Transform.hpp>
#include <xyginext/ecs/components/BroadPhaseComponent.hpp>
#include <xyginext/ecs/Scene.hpp>
#include <xyginext/ecs/systems/DynamicTreeSystem.hpp>

CollisionSystem::CollisionSystem(xy::MessageBus& bus) :
	xy::System(bus,typeid(CollisionSystem))
{
	requireComponent<xy::Transform>();
	requireComponent<xy::BroadphaseComponent>();
	requireComponent<Collider>();
}

void CollisionSystem::process(float dT)
{
	broadPhase();

}

void CollisionSystem::broadPhase()
{
	auto& entities = getEntities();
	for (auto entity : entities) {
		auto& collider = entity.getComponent<Collider>();
		if (collider.dynamic) {
			auto& broadphase = entity.getComponent<xy::BroadphaseComponent>();
			auto bounds = broadphase.getArea();
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
	for (auto& c : mCollisions) {
		auto& entA = c.first;
		auto& entB = c.second;

		auto& txA = entA.getComponent<xy::Transform>();
		auto& txB = entB.getComponent<xy::Transform>();

		auto boundsA = txA.getTransform().transformRect(
		entA.getComponent<xy::BroadphaseComponent>().getArea()
		);
		auto boundsB = txB.getTransform().transformRect(
			entB.getComponent<xy::BroadphaseComponent>().getArea()
		);
		sf::FloatRect intersection;
		boundsA.intersects(boundsB, intersection);

		sf::Vector2f normal = txB.getPosition() - txB.getPosition();
		Manifold manifold;
		if (intersection.width < intersection.height) {
			manifold.normal.x = (normal.x < 0) ? 1 : -1;
			manifold.penetration = intersection.width;
		}
		else {
			manifold.normal.y = (normal.y < 0) ? 1 : -1;
			manifold.penetration = intersection.height;
		}
		auto& collidableA = entA.getComponent<Collider>();
		auto& collidableB = entB.getComponent<Collider>();
		
		if (collidableA.dynamic) {
			txA.move(manifold.normal.x * manifold.penetration,manifold.normal.y * manifold.penetration);
		}


	}



}

