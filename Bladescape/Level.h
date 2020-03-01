#pragma once
#include <SFML/System/Vector2.hpp>

#include <SFML/Graphics/RenderTexture.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Object.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include "CollisionSystem.hpp"

#include <xyginext/ecs/Scene.hpp>
enum EnemyTypes final {
};
struct Path {
};
struct CollisionData {
	sf::FloatRect bounds{};
};
struct EnemyData {
	sf::Vector2f spawnPoint;
	EnemyTypes type;
	Path enemyPath;
};

class Level 
{
public:
	Level(std::string mapPath);
	bool loadMap(std::string& mapPath);
	sf::FloatRect getLevelBounds() const { return mLevelBounds; }
	std::vector<CollisionData>& getCollisionData() { return mCollisionData; }
	const sf::Vector2f getSpawnPoint() const { return mSpawnPoint; }
	const float getTileSize() const { return mTileSize; }
	sf::Vector2f getWorldSize() { return mWorldSize; }
	std::vector<std::unique_ptr<sf::RenderTexture>>& getRenderLayers() { return  mLayers; }

	

private:
	sf::FloatRect mLevelBounds{};
	float mTileSize;
	sf::Vector2f mWorldSize;
	std::vector<EnemyData> mEnemyData;
	std::vector<CollisionData> mCollisionData;
	sf::Vector2f mSpawnPoint;
	std::vector<std::unique_ptr<sf::RenderTexture>> mLayers;
	
};
