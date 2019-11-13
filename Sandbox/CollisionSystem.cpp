#include <xyginext/ecs/components/Transform.hpp>
#include <xyginext/ecs/Scene.hpp>
#include <xyginext/ecs/systems/DynamicTreeSystem.hpp>
#include <xyginext/core/App.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "CollisionSystem.hpp"

CollisionSystem::CollisionSystem(xy::MessageBus& mb, bool server)
	: xy::System(mb, typeid(CollisionSystem)),
	m_isServer(server)
#ifdef DDRAW
	, m_drawDebug(true)
#endif
{
	requireComponent<CollisionComponent>();
	requireComponent<xy::Transform>();
}

//public
void CollisionSystem::process(float)
{
#ifdef DDRAW
	m_vertices.clear();
#endif
	m_collisions.clear();

	auto& entities = getEntities();
	for (auto& entity : entities)
	{
		broadPhase(entity);
	}

	narrowPhase();
}

void CollisionSystem::queryState(xy::Entity entity)
{
	m_collisions.clear();
	broadPhase(entity);
	narrowPhaseQuery(entity);
}

//private
void CollisionSystem::broadPhase(xy::Entity entity)
{
	XY_ASSERT(entity.hasComponent<CollisionComponent>(), "Requires collision component!");

	const auto& xForm = entity.getComponent<xy::Transform>();
	auto& collisionComponent = entity.getComponent<CollisionComponent>();

	//bool isMapGeom = false;

	for (auto i = 0u; i < collisionComponent.m_hitboxCount; ++i)
	{
		auto& hitbox = collisionComponent.m_hitboxes[i];
#ifdef DDRAW
		auto rect = hitbox.getCollisionRect();
		rect = entity.getComponent<xy::Transform>().getTransform().transformRect(rect);

		//draw some debug to make sure the entities are being created in the correct place
		sf::Color colour = (hitbox.m_collisionCount > 0) ? sf::Color::Red : sf::Color::Green;
		//repeat the first and last points with transparent verts
		m_vertices.emplace_back(sf::Vector2f(rect.left, rect.top), sf::Color::Transparent);
		m_vertices.emplace_back(sf::Vector2f(rect.left, rect.top), colour); //TODO use a colour based on collision type?
		m_vertices.emplace_back(sf::Vector2f(rect.left + rect.width, rect.top), colour);
		m_vertices.emplace_back(sf::Vector2f(rect.left + rect.width, rect.top + rect.height), colour);
		m_vertices.emplace_back(sf::Vector2f(rect.left, rect.top + rect.height), colour);
		m_vertices.emplace_back(sf::Vector2f(rect.left, rect.top), colour);
		m_vertices.emplace_back(sf::Vector2f(rect.left, rect.top), sf::Color::Transparent);
#endif
		hitbox.m_collisionCount = 0;
		//auto type = hitbox.getType();
		//isMapGeom = (type == CollisionType::Platform || type == CollisionType::Solid || type == CollisionType::Teleport);
	}

	//if (isMapGeom) return;

	//actual collision testing...
	auto globalBounds = xForm.getTransform().transformRect(collisionComponent.getLocalBounds());
	auto others = getScene()->getSystem<xy::DynamicTreeSystem>().query(globalBounds);

	for (const auto& other : others)
	{
		if (entity != other && passesFilter(entity, other))
		{
			//only test for AABB collisions first, so we make sure each pair is processed only once
			auto otherBounds = other.getComponent<xy::Transform>().getTransform().transformRect(other.getComponent<CollisionComponent>().getLocalBounds());

			if (globalBounds.intersects(otherBounds))
			{
				m_collisions.insert(std::minmax(entity, other));
			}
		}
	}
}

void CollisionSystem::narrowPhase()
{
	//calc manifolds for any collisions and enter into component info
	//if(m_isServer) DPRINT("Broadphase count", std::to_string(m_collisions.size()));
	for (auto c : m_collisions)
	{
		const auto& txA = c.first.getComponent<xy::Transform>();
		const auto& txB = c.second.getComponent<xy::Transform>();

		auto& ccA = c.first.getComponent<CollisionComponent>();
		auto& ccB = c.second.getComponent<CollisionComponent>();

		//such nested...
		for (auto i = 0u; i < ccA.m_hitboxCount; ++i)
		{
			auto& boxA = ccA.m_hitboxes[i];
			auto rectA = txA.getTransform().transformRect(boxA.getCollisionRect());

			for (auto j = 0u; j < ccB.m_hitboxCount; ++j)
			{
				auto& boxB = ccB.m_hitboxes[j];

				sf::FloatRect overlap;
				if (rectA.intersects(txB.getTransform().transformRect(boxB.getCollisionRect()), overlap))
				{
					sf::Vector2f normal = (txB.getPosition() - txB.getOrigin()) - (txA.getPosition() - txA.getOrigin());

					Manifold manifold;
					if (overlap.width < overlap.height)
					{
						manifold.normal.x = (normal.x < 0) ? 1.f : -1.f;
						manifold.penetration = overlap.width;
					}
					else
					{
						manifold.normal.y = (normal.y < 0) ? 1.f : -1.f;
						manifold.penetration = overlap.height;
					}
					manifold.otherType = boxB.getType();
					manifold.otherEntity = c.second;

					if (boxA.m_collisionCount < Hitbox::MaxCollisions)
					{
						boxA.m_manifolds[boxA.m_collisionCount++] = manifold;
					}

					manifold.normal = -manifold.normal;
					manifold.otherType = boxA.getType();
					manifold.otherEntity = c.first;
					if (boxB.m_collisionCount < Hitbox::MaxCollisions)
					{
						boxB.m_manifolds[boxB.m_collisionCount++] = manifold;
					}
				}
			}
		}
	}
}

void CollisionSystem::narrowPhaseQuery(xy::Entity entity)
{
	for (auto c : m_collisions)
	{
		xy::Entity first;
		xy::Entity second;
		if (entity == c.first)
		{
			first = c.first;
			second = c.second;
		}
		else
		{
			first = c.second;
			second = c.first;
		}

		const auto& txA = first.getComponent<xy::Transform>();
		const auto& txB = second.getComponent<xy::Transform>();

		auto& ccA = first.getComponent<CollisionComponent>();
		auto& ccB = second.getComponent<CollisionComponent>();

		for (auto i = 0u; i < ccA.m_hitboxCount; ++i)
		{
			auto& boxA = ccA.m_hitboxes[i];
			auto rectA = txA.getTransform().transformRect(boxA.getCollisionRect());

			for (auto j = 0u; j < ccB.m_hitboxCount; ++j)
			{
				auto& boxB = ccB.m_hitboxes[j];

				sf::FloatRect overlap;
				if (rectA.intersects(txB.getTransform().transformRect(boxB.getCollisionRect()), overlap))
				{
					sf::Vector2f normal = (txB.getPosition() - txB.getOrigin()) - (txA.getPosition() - txA.getOrigin());

					Manifold manifold;
					if (overlap.width < overlap.height)
					{
						manifold.normal.x = (normal.x < 0) ? 1.f : -1.f;
						manifold.penetration = overlap.width;
					}
					else
					{
						manifold.normal.y = (normal.y < 0) ? 1.f : -1.f;
						manifold.penetration = overlap.height;
					}
					manifold.otherType = boxB.getType();
					manifold.otherEntity = second;

					if (boxA.m_collisionCount < Hitbox::MaxCollisions)
					{
						boxA.m_manifolds[boxA.m_collisionCount++] = manifold;
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

	return ((collisionA.m_maskBits & collisionB.m_categoryBits) != 0 && (collisionA.m_categoryBits & collisionB.m_maskBits) != 0);
}

#ifdef DDRAW
void CollisionSystem::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
	if (m_drawDebug) rt.draw(m_vertices.data(), m_vertices.size(), sf::LinesStrip, states);
}
#endif

#include <cstring>

namespace
{
	constexpr std::size_t ManSize = sizeof(Manifold::penetration) + sizeof(Manifold::normal) + sizeof(Manifold::otherType);
	constexpr std::size_t HitboxSize = sizeof(CollisionType::ID) + sizeof(std::size_t);
	constexpr std::size_t ComponentSize = sizeof(std::size_t) + sizeof(sf::Uint32) + sizeof(sf::Uint32);
}

CollisionComponent::CollisionComponent()
{
	float fMax = std::numeric_limits<float>::max();
	m_localBounds.left = fMax;
	m_localBounds.top = fMax;
	m_localBounds.width = -fMax;
	m_localBounds.height = -fMax;
}

//public
void CollisionComponent::addHitbox(sf::FloatRect rect, CollisionType::ID type)
{
	XY_ASSERT(m_hitboxCount < MaxHitBoxes, "No more hitboxes may be added");
	m_hitboxes[m_hitboxCount].setCollisionRect(rect);
	m_hitboxes[m_hitboxCount].setType(type);
	m_hitboxCount++;
	//merge boxes for broad phase culling
	if (m_localBounds.left > rect.left)
	{
		m_localBounds.left = rect.left;
	}
	if (m_localBounds.top > rect.top)
	{
		m_localBounds.top = rect.top;
	}
	if (m_localBounds.width < (rect.left + rect.width))
	{
		m_localBounds.width = (rect.left + rect.width);
	}
	if (m_localBounds.height < (rect.top + rect.height))
	{
		m_localBounds.height = (rect.top + rect.height);
	}
}

std::vector<sf::Uint8> CollisionComponent::serialise() const
{
	std::size_t size = 0;
	for (auto i = 0u; i < m_hitboxCount; ++i)
	{
		size += (m_hitboxes[i].getCollisionCount() * ManSize) + HitboxSize;
	}
	size += ComponentSize;

	std::vector<sf::Uint8> data(size);
	auto ptr = data.data();

	//catbits, maskbits, hitbox count
	std::memcpy(ptr, &m_categoryBits, sizeof(m_categoryBits));
	ptr += sizeof(m_categoryBits);
	std::memcpy(ptr, &m_maskBits, sizeof(m_maskBits));
	ptr += sizeof(m_maskBits);
	std::memcpy(ptr, &m_hitboxCount, sizeof(m_hitboxCount));
	ptr += sizeof(m_hitboxCount);

	//each hitbox
	for (auto i = 0u; i < m_hitboxCount; ++i)
	{
		//collision type, manifold count
		const auto& hitbox = m_hitboxes[i];

		auto type = hitbox.getType();
		std::memcpy(ptr, &type, sizeof(type));
		ptr += sizeof(type);
		auto count = hitbox.getCollisionCount();
		std::memcpy(ptr, &count, sizeof(count));
		ptr += sizeof(count);

		//each manifold in current hitbox
		for (auto j = 0u; j < count; ++j)
		{
			const auto& man = hitbox.getManifolds()[j];
			//penetration, normal, other type
			std::memcpy(ptr, &man.penetration, sizeof(man.penetration));
			ptr += sizeof(man.penetration);
			std::memcpy(ptr, &man.normal, sizeof(man.normal));
			ptr += sizeof(man.normal);
			std::memcpy(ptr, &man.otherType, sizeof(man.otherType));
			ptr += sizeof(man.otherType);
		}
	}

	return data;
}

void CollisionComponent::deserialise(const std::vector<sf::Uint8>& data)
{
	auto ptr = data.data();

	//catbits, maskbits, hitbox count
	std::memcpy(&m_categoryBits, ptr, sizeof(m_categoryBits));
	ptr += sizeof(m_categoryBits);
	std::memcpy(&m_maskBits, ptr, sizeof(m_maskBits));
	ptr += sizeof(m_maskBits);
	std::memcpy(&m_hitboxCount, ptr, sizeof(m_hitboxCount));
	ptr += sizeof(m_hitboxCount);

	//hitboxes
	for (auto i = 0u; i < m_hitboxCount; ++i)
	{
		//collision type, manifold count
		auto& hitbox = m_hitboxes[i];

		CollisionType::ID type;
		std::memcpy(&type, ptr, sizeof(type));
		ptr += sizeof(type);
		hitbox.setType(type);

		std::size_t count;
		std::memcpy(&count, ptr, sizeof(count));
		ptr += sizeof(count);
		hitbox.m_collisionCount = count;

		//each manifold in hitbox
		for (auto j = 0u; j < count; ++j)
		{
			auto& man = hitbox.m_manifolds[j];
			//penetration, normal, other type
			std::memcpy(&man.penetration, ptr, sizeof(man.penetration));
			ptr += sizeof(man.penetration);
			std::memcpy(&man.normal, ptr, sizeof(man.normal));
			ptr += sizeof(man.normal);
			std::memcpy(&man.otherType, ptr, sizeof(man.otherType));
			ptr += sizeof(man.otherType);
		}
	}
}