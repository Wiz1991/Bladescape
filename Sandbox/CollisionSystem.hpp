#pragma once

#include <xyginext/ecs/System.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <vector>
#include <set>
#define DDRAW
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
		Teleport = 0x4,
		Player = 0x8,
		Bubble = 0x10,
		NPC = 0x20,
		Fruit = 0x40,
		Powerup = 0x80,
		Bonus = 0x100,
		HardBounds = 0x200,
		MagicHat = 0x400,
		Crate = 0x800,
		Explosion = 0x1000,
		Dynamite = 0x2000,

		PlayerMask = Bubble | Platform | Solid | Teleport | NPC | Fruit | Powerup | Bonus | MagicHat | Crate | Explosion,
		NPCMask = Solid | Player | Bubble | Platform | Teleport | Powerup | HardBounds | Crate | Explosion,
		FruitMask = Solid | Platform | Player | Teleport,
		PowerupMask = Platform | Solid | Player | NPC | Crate,
		CrateMask = Platform | Solid | Player | NPC | Powerup | Teleport | Crate,
		ExplosionMask = Player | NPC,
		DynamiteMask = Solid | Platform | Teleport
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
		Teleport = 0x8,
		NPC = 0x10,
		Fruit = 0x20,
		Powerup = 0x40,
		Platform = 0x80,
		Bubble = 0x100,
		HardBounds = 0x200,
		MagicHat = 0x400,
		Crate = 0x800,
		Explosion = 0x1000,
		Dynamite = 0x2000
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