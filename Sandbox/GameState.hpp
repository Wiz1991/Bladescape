#pragma once

#include <xyginext/core/State.hpp>
#include <xyginext/ecs/Scene.hpp>
#include <xyginext/resources/ResourceHandler.hpp>
#include "Level.h"
#include <SFML/Graphics/Texture.hpp>
#include <xyginext/resources/ShaderResource.hpp>
#include <xyginext/ecs/components/Sprite.hpp>
#include "PlayerController.h"
#include "AnimationMap.h"
#include "ResourceIDs.hpp"
#include <xyginext/resources/Resource.hpp>

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
	void loadCollisions();

private:
	xy::Scene mGameScene;
	Level mLevel;
	xy::Entity player;
	PlayerController mPlayerController;
	std::array<xy::Sprite, SpriteID::Count> mSprites;
	AnimationMap<AnimID::Player::Count> mPlayerAnimations;
	xy::TextureResource mTextureResource;
};