#include "GameState.hpp"
#include "States.hpp"
#include "CommandIDs.hpp"
#include "ResourceIDs.hpp"
#include "CollisionSystem.hpp"
#include "MessageIDs.hpp"
#include "GameConsts.h"
#include "ShapeUtils.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <xyginext/ecs/components/Transform.hpp>
#include <xyginext/ecs/components/Drawable.hpp>
#include <xyginext/ecs/components/Camera.hpp>
#include <xyginext/ecs/components/Sprite.hpp>
#include <xyginext/ecs/components/CommandTarget.hpp>
#include <xyginext/ecs/components/BroadPhaseComponent.hpp>
#include <xyginext/ecs/components/Text.hpp>
#include <xyginext/ecs/components/SpriteAnimation.hpp>

#include <xyginext/ecs/systems/DynamicTreeSystem.hpp>
#include <xyginext/ecs/systems/SpriteSystem.hpp>
#include <xyginext/ecs/systems/CommandSystem.hpp>
#include <xyginext/ecs/systems/RenderSystem.hpp>
#include <xyginext/ecs/systems/TextSystem.hpp>
#include <xyginext/ecs/systems/CallbackSystem.hpp>
#include <xyginext/ecs/systems/CameraSystem.hpp>

#include "CameraTargetSystem.h"
#include "StringUtils.h"
#include <xyginext/ecs/systems/SpriteAnimator.hpp>

#include <xyginext/core/App.hpp>
#include <xyginext/util/Vector.hpp>
#include <xyginext/graphics/SpriteSheet.hpp>

#include <SFML/Window/Event.hpp>

#include <SFML/Graphics/Font.hpp>

GameState::GameState(xy::StateStack& ss, xy::State::Context ctx)
	: xy::State(ss, ctx),
	mGameScene(ctx.appInstance.getMessageBus())

{
	initScene();
	loadResources();
	buildWorld();

	mGameScene.getActiveCamera().getComponent<xy::Camera>().setView(ctx.defaultView.getSize());
	mGameScene.getActiveCamera().getComponent<xy::Camera>().setViewport(ctx.defaultView.getViewport());

	ctx.appInstance.setMouseCursorVisible(true);
}

bool GameState::handleEvent(const sf::Event& evt)
{
	mGameScene.forwardEvent(evt);

	return true;
}

void GameState::handleMessage(const xy::Message& msg)
{
	mGameScene.forwardMessage(msg);
}

bool GameState::update(float dt)
{
	mGameScene.update(dt);

	return true;
}

void GameState::draw()
{
	auto rw = getContext().appInstance.getRenderWindow();
	rw->clear(sf::Color::White);
	rw->draw(mGameScene);
}

xy::StateID GameState::stateID() const
{
	return States::GameState;
}

void GameState::buildWorld()
{
	auto entity = mGameScene.createEntity();
	entity.addComponent<xy::Sprite>() = mSprites[SpriteID::Player];
	entity.addComponent<xy::SpriteAnimation>().play(m_playerAnimations[AnimID::Player::Run]);
	entity.addComponent<xy::Drawable>();

	entity.addComponent<xy::Transform>().setPosition(500, 200);
	entity.getComponent<xy::Transform>().setScale(3, 3);
}

void GameState::loadResources()
{
	xy::SpriteSheet spriteSheet;
	spriteSheet.loadFromFile("assets/sprites/player.spt", mTextureResource);
	mSprites[SpriteID::Player] = spriteSheet.getSprite("player");
	m_playerAnimations[AnimID::Player::Idle] = spriteSheet.getAnimationIndex("idle", "player");
	m_playerAnimations[AnimID::Player::Jump] = spriteSheet.getAnimationIndex("jump", "player");
	m_playerAnimations[AnimID::Player::Run] = spriteSheet.getAnimationIndex("run", "player");
	m_playerAnimations[AnimID::Player::Die] = spriteSheet.getAnimationIndex("die", "player");
}
void GameState::initScene()
{
	auto& mb = getContext().appInstance.getMessageBus();
	mGameScene.addSystem<xy::SpriteSystem>(mb);
	mGameScene.addSystem<xy::SpriteAnimator>(mb);
	mGameScene.addSystem<xy::RenderSystem>(mb);
}

void GameState::loadCollisions()
{
}