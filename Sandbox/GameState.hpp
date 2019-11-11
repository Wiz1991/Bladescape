#pragma once

#include <xyginext/core/State.hpp>
#include <xyginext/ecs/Scene.hpp>
#include <xyginext/resources/ResourceHandler.hpp>
#include "MapLoader.h"
#include <SFML/Graphics/Texture.hpp>
#include <xyginext/resources/ShaderResource.hpp>
class GameState final : public xy::State
{
public:
	GameState(xy::StateStack&, xy::State::Context);

private:
	bool handleEvent(const sf::Event& evt) override;

	void handleMessage(const xy::Message&) override;

	bool update(float dt) override;

	void draw() override;

	xy::StateID stateID() const override;

	void buildWorld();
	void loadResources();
	void initScene();

private:
	xy::Scene mGameScene;
	MapLoader mMapLoader;
};