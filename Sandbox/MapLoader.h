#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <vector>
#include <memory>
#include <unordered_map>
#include <SFML/Graphics/RenderTexture.hpp>
#include <string>

class MapLoader final
{
public:
	MapLoader();

	bool load(const std::string& map);

	const std::vector<sf::Texture>& getLayer() const { return mLayer; }
	const float getTileSize() const { return tileSize; }
	const sf::Vector2f getWorldSize() const { return mWorldSize; }
	std::vector<sf::FloatRect>& getCollisionShapes() { return mCollisionShapes; }

private:
	std::vector<sf::Texture> mLayer;
	float tileSize;
	sf::Vector2f mWorldSize;
	std::vector<sf::FloatRect> mCollisionShapes;
};