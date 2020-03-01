#include "Level.h"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "GameConsts.h"
Level::Level(std::string mapPath)
{
	loadMap(mapPath);
}

bool Level::loadMap(std::string& mapPath)
{
	tmx::Map map;
	if (map.load(mapPath)) {
		float scale = 64 / 16;
		
		std::vector<std::unique_ptr<sf::Texture>> textures;
		const auto& layers = map.getLayers();
		for (const auto& layer : layers) {
			//create a new layer for drawing the tiles on
			auto renderLayer = std::make_unique<sf::RenderTexture>();

			renderLayer->create(map.getTileCount().x * map.getTileSize().x,
								map.getTileCount().y * map.getTileSize().y);


			if (layer->getType() == tmx::Layer::Type::Tile) {

				//supports usage of only one tileset
				const auto& tileSets = map.getTilesets();
				for (const auto& tileset : tileSets) {
					//read textures and store
					std::unique_ptr <sf::Texture> tex = std::make_unique<sf::Texture>();
					if (!tex->loadFromFile(tileset.getImagePath())) {
						xy::Logger::log("Failed to load tileset: " + tileset.getImagePath(), xy::Logger::Type::Error);
						return false;
					}
					else {
						textures.push_back(std::move(tex));
					}
				}

				const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
				const auto& tiles = tileLayer.getTiles();

				//draw to render texture
				sf::IntRect drawRect(0, 0, map.getTileSize().x, map.getTileSize().y);
				sf::Sprite drawSprite;
				mTileSize = map.getTileSize().x;
				mWorldSize = sf::Vector2f(map.getTileCount().x * mTileSize, map.getTileCount().y * mTileSize);

				for (auto y = 0u; y < map.getTileCount().y; ++y) {
					for (auto x = 0u; x < map.getTileCount().x; ++x) {
						auto posX = static_cast<float>(x * map.getTileSize().x);
						auto posY = static_cast<float>(y * map.getTileSize().y);
						sf::Vector2f position(posX, posY);

						drawSprite.setPosition(position);

						auto tileID = tiles[y * map.getTileCount().x + x].ID;
						if (tileID == 0) continue; //empty tile

						auto i = 0;
						while (tileID < tileSets[i].getFirstGID()) ++i;

						auto relativeID = tileID - tileSets[i].getFirstGID();
						auto tileX = relativeID % tileSets[i].getColumnCount();
						auto tileY = relativeID / tileSets[i].getColumnCount();

						drawRect.left = tileX * tileSets[i].getTileSize().x;
						drawRect.top = tileY * tileSets[i].getTileSize().y;

						drawSprite.setTexture(*textures[0]);
						drawSprite.setTextureRect(drawRect);

						renderLayer->draw(drawSprite);
					}
				}

				renderLayer->display();
				mLayers.push_back(std::move(renderLayer));
			}
			else if (layer->getType() == tmx::Layer::Type::Object) {
				auto toFloatRect = [](tmx::FloatRect rect)->sf::FloatRect
				{
					return { rect.left, rect.top, rect.width, rect.height };
				};
				auto toVector2f = [](tmx::Vector2f vector)->sf::Vector2f
				{
					return { vector.x,vector.y };
				};
				const auto& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();
				for (const auto& object : objects) {
					if (object.getType() == "solid") {
						CollisionData data;
						data.bounds = toFloatRect(object.getAABB());
						mCollisionData.emplace_back(data);
					}
					if (object.getType() == "spawnpoint") {
						mSpawnPoint = toVector2f(object.getPosition());
					}
				}
			}
		}
	}
	else {
		xy::Logger::log("Failed to load map '" + mapPath + "'", xy::Logger::Type::Error, xy::Logger::Output::Console);
		return false;
	}
}

