#pragma once

#include <xyginext/core/State.hpp>
#include <xyginext/ecs/Scene.hpp>
#include <xyginext/resources/ResourceHandler.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <xyginext/resources/Resource.hpp>
#include "ResourceIDs.hpp"
#include "AnimationData.h"
#include "PlayerController.h"

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
	xy::TextureResource mTextureResource;
	PlayerController mPlayerController;
	std::array<xy::Sprite, SpriteID::Count> mSprites;
	AnimationMap<AnimID::Player::Count> m_playerAnimations;
};