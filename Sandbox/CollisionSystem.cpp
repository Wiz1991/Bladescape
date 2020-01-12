#include <xyginext/ecs/components/Transform.hpp>
#include <xyginext/ecs/Scene.hpp>
#include <xyginext/ecs/systems/QuadTree.hpp>
#include <xyginext/core/App.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "CollisionSystem.hpp"

CollisionSystem::CollisionSystem(xy::MessageBus& mb)
	: xy::System(mb, typeid(CollisionSystem))
	//#ifdef DDRAW
	, mDebugLines(true)
	//#endif // DDRAW
{
	requireComponent<xy::Transform>();
	requireComponent<CollisionComponent>();
}

void CollisionSystem::process(float dT)
{
	//#ifdef DDRAW
	mVertices.clear();
	//#endif // DDRAW
	mCollisions.clear();
	auto& entities = getEntities();
	for (auto& entity : entities) {
		broadPhase(entity);
	}
	narrowPhase();
}
void CollisionSystem::broadPhase(xy::Entity entity)
{
	const auto& eTransform = entity.getComponent<xy::Transform>();
	auto& collisionComponent = entity.getComponent<CollisionComponent>();

	//#ifdef DDRAW
	for (auto i = 0; i < collisionComponent.getHitboxCount(); ++i) {
		auto& hitbox = collisionComponent.mHitboxes[i];
		auto rect = hitbox.getBoundingRect();
		rect = entity.getComponent<xy::Transform>().getTransform().transformRect(rect);

		sf::Color color = (hitbox.getCollisionCount() > 0) ? sf::Color::Red : sf::Color::Green;

		mVertices.emplace_back(sf::Vector2f(rect.left, rect.top), sf::Color::Transparent);
		mVertices.emplace_back(sf::Vector2f(rect.left, rect.top), color); //TODO use a colour based on collision type?
		mVertices.emplace_back(sf::Vector2f(rect.left + rect.width, rect.top), color);
		mVertices.emplace_back(sf::Vector2f(rect.left + rect.width, rect.top + rect.height), color);
		mVertices.emplace_back(sf::Vector2f(rect.left, rect.top + rect.height), color);
		mVertices.emplace_back(sf::Vector2f(rect.left, rect.top), color);
		mVertices.emplace_back(sf::Vector2f(rect.left, rect.top), sf::Color::Transparent);
		hitbox.mCollisionCount = 0;
	}
	//#endif // DDRAW
	auto globalBounds = eTransform.getTransform().transformRect(collisionComponent.getLocalBounds());
	auto others = getScene()->getSystem<xy::QuadTree>().queryArea(globalBounds);

	for (const auto& other : others) {
		if (entity != other && passesFilter(entity, other)) {
			auto otherBounds = other.getComponent<xy::Transform>().getTransform().transformRect(
				other.getComponent<CollisionComponent>().getLocalBounds()
			);
			if (globalBounds.intersects(otherBounds)) {
				mCollisions.insert(std::minmax(entity, other));
			}
		}
	}
}
void CollisionSystem::narrowPhase()
{
	for (auto c : mCollisions) {
		const auto& txA = c.first.getComponent<xy::Transform>();
		const auto& txB = c.second.getComponent<xy::Transform>();

		auto& ccA = c.first.getComponent<CollisionComponent>();
		auto& ccB = c.second.getComponent<CollisionComponent>();

		for (auto i = 0; i < ccA.getHitboxCount(); ++i) {
			auto& boxA = ccA.mHitboxes[i];
			auto rectA = txA.getTransform().transformRect(boxA.getBoundingRect());
			for (auto j = 0; j < ccB.getHitboxCount(); ++j) {
				auto& boxB = ccB.mHitboxes[j];
				auto rectB = txB.getTransform().transformRect(boxB.getBoundingRect());

				sf::FloatRect intersection;
				if (rectA.intersects(rectB, intersection)) {
					sf::Vector2f normal = (txB.getPosition() - txB.getOrigin()) - (txA.getPosition() - txA.getOrigin());

					Manifold manifold;
					if (intersection.width < intersection.height) {
						manifold.penetration = intersection.width;
						manifold.normal.x = (normal.x < 0) ? 1 : -1.f;
					}
					else {
						manifold.normal.y = (normal.y < 0) ? 1 : -1;
						manifold.penetration = intersection.height;
					}
					manifold.otherType = boxB.getType();
					manifold.otherEntity = c.second;
					if (boxA.mCollisionCount < Hitbox::maxCollisions) {
						boxA.mManifolds[boxA.mCollisionCount++] = manifold;
					}

					manifold.normal = -manifold.normal;
					manifold.otherType = boxA.getType();
					manifold.otherEntity = c.first;
					if (boxB.mCollisionCount < Hitbox::maxCollisions) {
						boxB.mManifolds[boxB.mCollisionCount++] = manifold;
					}
				}
			}
		}
	}
}
bool CollisionSystem::passesFilter(xy::Entity a, xy::Entity b)
{
	const auto collisionA = a.getComponent<CollisionComponent>();
	const auto collisionB = b.getComponent<CollisionComponent>();

	bool pass = ((collisionA.mMaskBits & collisionB.mCategoryBits) && (collisionA.mCategoryBits & collisionB.mMaskBits));
	return pass;
}
//#ifdef DDRAW
void CollisionSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mDebugLines) {
		target.draw(mVertices.data(), mVertices.size(), sf::PrimitiveType::LinesStrip, states);
	}
}
//#endif // DDRAW

CollisionComponent::CollisionComponent()
{
}

void CollisionComponent::addHitbox(sf::FloatRect rect, CollisionType::ID type)
{
	if (mHitboxCount > maxHitBoxes) {
		xy::Logger::log("No more hitboxes may be added", xy::Logger::Type::Error, xy::Logger::Output::Console);
		return;
	}
	mHitboxes[mHitboxCount].setBoundingRect(rect);
	mHitboxes[mHitboxCount].setType(type);
	mHitboxCount++;

	if (mLocalBounds.left > rect.left) {
		mLocalBounds.left = rect.left;
	}
	if (mLocalBounds.top > rect.top) {
		mLocalBounds.top = rect.top;
	}
	if (mLocalBounds.width < (rect.left + rect.width)) {
		mLocalBounds.width = rect.left + rect.width;
	}
	if (mLocalBounds.height < rect.top + rect.height) {
		mLocalBounds.height = rect.top + rect.height;
	}
}