/*********************************************************************

Copyright 2019 Matt Marchant

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

*********************************************************************/

#include "MapLoader.h"
#include "ResourceIDs.hpp"

#include <xyginext/core/FileSystem.hpp>
#include <xyginext/core/Log.hpp>

#include <xyginext/util/String.hpp>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

#include <map>

MapLoader::MapLoader()

{
}

//public
bool MapLoader::load(const std::string& file)
{
	tmx::Map map;

	if (map.load("assets/maps/" + file))
	{
		std::vector<std::unique_ptr<sf::Texture>>textures;
		const auto& tileSets = map.getTilesets();
		for (const auto& ts : tileSets) {
			std::unique_ptr<sf::Texture> tex = std::make_unique<sf::Texture>();
			if (!tex->loadFromFile(ts.getImagePath())) {
				xy::Logger::log("Failed to load tile set image:  " + ts.getImagePath(), xy::Logger::Type::Error);
				return false;
			}
			else {
				textures.push_back(std::move(tex));
			}
		}

		sf::IntRect textureRect(0, 0, (map.getTileSize().x), (map.getTileSize().y));
		sf::Sprite drawSprite;

		sf::RenderTexture renderLayer;
		renderLayer.create(map.getTileCount().x * map.getTileSize().x, map.getTileCount().y * map.getTileSize().y);
		mWorldSize = sf::Vector2f(map.getTileCount().x * map.getTileSize().x, map.getTileCount().y * map.getTileSize().y);
		const auto& layers = map.getLayers();
		for (const auto& layer : layers) {
			if (layer->getType() == tmx::Layer::Type::Tile) {
				renderLayer.clear(sf::Color::Transparent);
				const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
				const auto& tiles = tileLayer.getTiles();
				tileSize = map.getTileSize().x;

				for (auto y = 0u; y < map.getTileCount().y; ++y) {
					for (auto x = 0u; x < map.getTileCount().x; ++x) {
						auto posX = static_cast<float>(x * map.getTileSize().x);
						auto posY = static_cast<float>(y * map.getTileSize().y);
						sf::Vector2f position(posX, posY);

						drawSprite.setPosition(position);
						auto tileID = tiles[y * map.getTileCount().x + x].ID;
						if (tileID == 0) {
							continue;
						}
						auto i = 0;
						while (tileID < tileSets[i].getFirstGID()) {
							++i;
						}

						auto relativeID = tileID - tileSets[i].getFirstGID();
						auto tileX = relativeID % tileSets[i].getColumnCount();
						auto tileY = relativeID / tileSets[i].getColumnCount();
						textureRect.left = tileX * tileSets[i].getTileSize().x;
						textureRect.top = tileY * tileSets[i].getTileSize().y;

						drawSprite.setTexture(*textures[0]);
						drawSprite.setTextureRect(textureRect);

						renderLayer.draw(drawSprite);
					}
				}
				renderLayer.display();
				mLayer.push_back(renderLayer.getTexture());
			}
			else if (layer->getType() == tmx::Layer::Type::Object) {
				auto toFloatRect = [](tmx::FloatRect rect)->sf::FloatRect
				{
					return { rect.left, rect.top, rect.width, rect.height };
				};
				const auto& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();
				for (const auto& object : objects) {
					if (object.getType() == "solid") {
						mCollisionShapes.emplace_back(toFloatRect(object.getAABB()));
					}
				}
			}
		}
	}

	return false;
}