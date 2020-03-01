#pragma once

#include <xyginext/ecs/System.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <vector>
#include <set>

#ifdef XY_DEBUG
#define DDRAW
#endif

class CollisionSystem final :public xy::System

#ifdef DDRAW
	, public sf::Drawable
#endif
{
public:
	explicit CollisionSystem(xy::MessageBus&, bool = false);

	void process(float) override;

	//allows querying the current world state with a given
	//entity to see if it collides. Entity must have collision
	//component and quad tree component. Used in reconciliation.
	void queryState(xy::Entity);

#ifdef XY_DEBUG
	void toggleDebugDraw() { m_drawDebug = !m_drawDebug; }
#endif // XY_DEBUG

private:

	void broadPhase(xy::Entity);
	void narrowPhase();
	void narrowPhaseQuery(xy::Entity); //as narrow phase but doesn't alter the state of other entities

	bool passesFilter(xy::Entity, xy::Entity);
	std::set<std::pair<xy::Entity, xy::Entity>> m_collisions;

#ifdef DDRAW
	bool m_drawDebug;
	std::vector<sf::Vertex> m_vertices;
	void draw(sf::RenderTarget&, sf::RenderStates) const override;
#endif
};
#include <xyginext/core/Assert.hpp>
#include <xyginext/ecs/Entity.hpp>

#include <SFML/Config.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <array>
#include <limits>
namespace CollisionFlags //used in broad phase culling
{
	enum
	{
		Solid = 0x1,
		Platform = 0x2,
		Player = 0x4,

		PlayerMask = Platform | Solid,
	};
}
namespace CollisionType
{
	enum ID //these are actually in a specific order for collision checks
	{
		None = 0,
		Player = 0x1,
		Foot = 0x2,
		Solid = 0x4,
	};
};

struct Manifold final
{
	float penetration = 0.f;
	sf::Vector2f normal;
	CollisionType::ID otherType = CollisionType::None;
	xy::Entity otherEntity;
};

class Hitbox final
{
public:
	static constexpr sf::Uint32 MaxCollisions = 16;

	CollisionType::ID getType() const { return m_collisionType; }
	void setType(CollisionType::ID type) { m_collisionType = type; }

	const std::array<Manifold, MaxCollisions>& getManifolds() const { return m_manifolds; }
	std::size_t getCollisionCount() const { return m_collisionCount; }

	sf::FloatRect getCollisionRect() const { return m_collisionRect; }
	void setCollisionRect(sf::FloatRect rect) { m_collisionRect = rect; }

private:
	CollisionType::ID m_collisionType = CollisionType::None;
	std::array<Manifold, MaxCollisions> m_manifolds{};
	std::size_t m_collisionCount = 0;
	sf::FloatRect m_collisionRect;

	friend class CollisionSystem;
	friend class CollisionComponent;
};

class CollisionComponent final
{
public:
	static constexpr sf::Uint32 MaxHitBoxes = 2;

	CollisionComponent();

	void addHitbox(sf::FloatRect rect, CollisionType::ID type);

	std::size_t getHitboxCount() const { return m_hitboxCount; }

	const std::array<Hitbox, MaxHitBoxes>& getHitboxes() const { return m_hitboxes; }

	//returns the combined bounds for all hitboxes for a broadphase pass
	sf::FloatRect getLocalBounds() const { return m_localBounds; }

	void setCollisionCategoryBits(sf::Uint32 bits) { m_categoryBits = bits; }

	void setCollisionMaskBits(sf::Uint32 bits) { m_maskBits = bits; }

	sf::Uint32 getCollisionCategoryBits() const { return m_categoryBits; }

	std::vector<sf::Uint8> serialise() const;

	void deserialise(const std::vector<sf::Uint8>&);

private:
	std::size_t m_hitboxCount = 0;
	std::array<Hitbox, MaxHitBoxes> m_hitboxes{};
	sf::FloatRect m_localBounds{};

	sf::Uint32 m_categoryBits = 1;
	sf::Uint32 m_maskBits = std::numeric_limits<sf::Uint32>::max();

	friend class CollisionSystem;
};