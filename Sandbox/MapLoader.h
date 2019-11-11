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

	const sf::Texture& getLayer() const { return mLayer; }

private:
	sf::Texture mLayer;
};