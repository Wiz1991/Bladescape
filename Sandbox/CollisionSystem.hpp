#pragma once

#include <xyginext/ecs/System.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <vector>
#include <set>

namespace CollisionType {
	enum ID {
		None = 0,
		Player = 0x1,
		Foot = 0x2,
		Solid = 0x4,
		Platform = 0x8,
	};
};
namespace CollisionFlags {
	enum {
		Solid = 0x1,
		Player = 0x2,
		Platform = 0x8,

		PlayerMask = Solid | Platform,
	};
}

#define DDRAW
class CollisionSystem final : public xy::System
#ifdef  DDRAW
	, sf::Drawable
#endif //  DDRAW
{
public:
	CollisionSystem(xy::MessageBus&);

	void process(float dT);
	void broadPhase(xy::Entity entity);
	void narrowPhase();

	bool passesFilter(xy::Entity a, xy::Entity b);
#ifdef DDRAW
	bool mDebugLines;
	std::vector<sf::Vertex> mVertices;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
#endif // DDRAW
private:
	std::set<std::pair<xy::Entity, xy::Entity>> mCollisions;
};

struct Manifold final {
	float penetration = 0.f;
	sf::Vector2f normal;
	CollisionType::ID otherType = CollisionType::None;
	xy::Entity otherEntity;
};
class Hitbox final {
public:
	static constexpr sf::Uint32 maxCollisions = 16;

	CollisionType::ID getType() const { return mCollisionType; }
	void setType(CollisionType::ID type) { mCollisionType = type; }

	const std::array<Manifold, maxCollisions>& getManifolds() const { return mManifolds; }
	std::size_t getCollisionCount() const { return mCollisionCount; }

	sf::FloatRect getBoundingRect() const { return mBoundingRect; }
	void setBoundingRect(sf::FloatRect rect) { mBoundingRect = rect; }
private:
	CollisionType::ID mCollisionType = CollisionType::None;
	std::array<Manifold, maxCollisions> mManifolds{};
	std::size_t mCollisionCount = 0;
	sf::FloatRect mBoundingRect;
	friend class CollisionSystem;
	friend class CollisionComponent;
};
class CollisionComponent final {
public:

	static constexpr sf::Uint32 maxHitBoxes = 2;
	CollisionComponent();

	void addHitbox(sf::FloatRect rect, CollisionType::ID type);
	const std::array<Hitbox, maxHitBoxes>& getHitboxes() const { return mHitboxes; }

	std::size_t getHitboxCount() const { return mHitboxCount; }
	sf::FloatRect getLocalBounds() const { return mLocalBounds; }

	void setCollisionCategoryBits(sf::Uint32 bits) { mCategoryBits = bits; }
	void setCollisionMaskBits(sf::Uint32 bits) { mMaskBits = bits; }

	sf::Uint32 getCollisionCategoryBits() const { return mCategoryBits; }
	sf::Uint32 getCollisionMaskBits() const { return mMaskBits; }

private:
	std::size_t mHitboxCount = 0;
	std::array<Hitbox, maxHitBoxes> mHitboxes{ };
	sf::FloatRect mLocalBounds{};

	sf::Uint32 mCategoryBits = 1;
	sf::Uint32 mMaskBits = std::numeric_limits<sf::Uint32>::max();
	friend class CollisionSystem;
};