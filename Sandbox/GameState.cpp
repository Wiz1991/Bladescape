#include "GameState.hpp"
#include "States.hpp"
#include "CommandIDs.hpp"
#include "ResourceIDs.hpp"
#include "MessageIDs.hpp"
#include "InputBinding.h"
#include "GameConsts.h"

#include <xyginext/ecs/components/Transform.hpp>
#include <xyginext/ecs/components/Drawable.hpp>
#include <xyginext/ecs/components/Camera.hpp>
#include <xyginext/ecs/components/Sprite.hpp>
#include <xyginext/ecs/components/QuadTreeItem.hpp>
#include <xyginext/ecs/components/CommandTarget.hpp>
#include <xyginext/ecs/components/SpriteAnimation.hpp>

#include "CameraTargetSystem.h"
#include <xyginext/ecs/systems/SpriteSystem.hpp>
#include <xyginext/ecs/systems/SpriteAnimator.hpp>
#include "PlayerSystem.h"
#include "CollisionSystem.hpp"
#include <xyginext/ecs/systems/RenderSystem.hpp>
#include <xyginext/ecs/systems/QuadTree.hpp>
#include <xyginext/ecs/systems/CommandSystem.hpp>
#include <xyginext/ecs/systems/CameraSystem.hpp>
#include "CameraTargetSystem.h"

#include "ShapeUtils.hpp"
#include "StringUtils.h"
#include <xyginext/util/Vector.hpp>

#include <xyginext/core/App.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <xyginext/graphics/SpriteSheet.hpp>
#include <SFML/Graphics/Font.hpp>

GameState::GameState(xy::StateStack& ss, xy::State::Context ctx)
	: xy::State(ss, ctx),
	mGameScene(ctx.appInstance.getMessageBus()),
	mPlayerController(InputBinding())
{
	initScene();
	loadResources();
	buildWorld();
	loadCollisions();

	mGameScene.getActiveCamera().getComponent<xy::Camera>().setView(ctx.defaultView.getSize());
	mGameScene.getActiveCamera().getComponent<xy::Camera>().setViewport(ctx.defaultView.getViewport());
	//mGameScene.getActiveCamera().getComponent<xy::Camera>().setZoom(2);

	ctx.appInstance.setMouseCursorVisible(true);
}

bool GameState::handleEvent(const sf::Event& evt)
{
	mGameScene.forwardEvent(evt);

	mPlayerController.handleEvent(evt);
	return true;
}

void GameState::handleMessage(const xy::Message& msg)
{
	mGameScene.forwardMessage(msg);
}

bool GameState::update(float dt)
{
	mGameScene.update(dt);

	mPlayerController.update();
	return true;
}

void GameState::draw()
{
	auto rw = getContext().appInstance.getRenderWindow();
	//rw->clear(sf::Color::White);
	rw->draw(mGameScene);
}

xy::StateID GameState::stateID() const
{
	return States::GameState;
}

void GameState::buildWorld()
{
	//PLAYER
	auto entity = mGameScene.createEntity();
	mPlayerController.setPlayerEntity(entity);
	entity.addComponent<xy::CommandTarget>().ID = CommandID::Player;
	entity.addComponent<xy::Sprite>() = mSprites[SpriteID::Player];
	entity.addComponent<xy::SpriteAnimation>().play(AnimID::Player::Run);
	entity.addComponent<xy::Drawable>();
	entity.addComponent<PlayerComponent>();
	entity.addComponent<CollisionComponent>().addHitbox({ 0,0,32,32 }, CollisionType::Player);
	entity.getComponent<CollisionComponent>().addHitbox({ 0,32,32,5 }, CollisionType::Foot);
	entity.getComponent<CollisionComponent>().addHitbox({ 32,0,5,30 }, CollisionType::RightHand);
	entity.getComponent<CollisionComponent>().addHitbox({ -5,0,5,30 }, CollisionType::LeftHand);
	//entity.getComponent<CollisionComponent>().addHitbox({ 0,-5,32,5 }, CollisionType::Head);
	entity.addComponent<xy::QuadTreeItem>().setArea(sf::FloatRect(0, 0, 32, 32));
	entity.getComponent<CollisionComponent>().setCollisionCategoryBits(CollisionType::Player);
	entity.getComponent<CollisionComponent>().setCollisionMaskBits(CollisionFlags::Solid);
	entity.addComponent<xy::Transform>().setPosition(500, 500);
	entity.getComponent<xy::Transform>().setOrigin(32 / 2, 32 / 2);

	mGameScene.getActiveCamera().addComponent<CameraTarget>().target = entity;
	mGameScene.getActiveCamera().getComponent<xy::Camera>().setZoom(1.7);
	mGameScene.getSystem<CameraTargetSystem>().setBounds({ 0,0,3000,720 });
}

void GameState::loadResources()
{
	xy::SpriteSheet spriteSheet;
	spriteSheet.loadFromFile("assets/sprites/player.spt", mTextureResource);
	mSprites[SpriteID::Player] = spriteSheet.getSprite("player");
	m_playerAnimations[AnimID::Player::Idle] = spriteSheet.getAnimationIndex("idle", "player");
	m_playerAnimations[AnimID::Player::Run] = spriteSheet.getAnimationIndex("run", "player");
	m_playerAnimations[AnimID::Player::Hit] = spriteSheet.getAnimationIndex("hit", "player");
}
void GameState::initScene()
{
	auto& mb = getContext().appInstance.getMessageBus();
	mGameScene.addSystem<xy::QuadTree>(mb, sf::FloatRect(0, 0, 1280, 720));
	mGameScene.addSystem<CollisionSystem>(mb);
	mGameScene.addSystem<PlayerSystem>(mb);
	mGameScene.addSystem<xy::SpriteSystem>(mb);
	mGameScene.addSystem<xy::SpriteAnimator>(mb);
	mGameScene.addSystem<CameraTargetSystem>(mb);
	mGameScene.addSystem<xy::CameraSystem>(mb);
	mGameScene.addSystem<xy::RenderSystem>(mb);
	mGameScene.addSystem<xy::CommandSystem>(mb);
}

void GameState::loadCollisions()
{
	sf::FloatRect bounds(0, 600, 2000, 32);

	auto entity = mGameScene.createEntity();
	entity.addComponent<xy::Transform>().setPosition(bounds.left, bounds.top);
	entity.addComponent<CollisionComponent>().addHitbox({ 0,0,bounds.width,bounds.height }, CollisionType::Solid);
	entity.addComponent<xy::QuadTreeItem>().setArea({ 0,0,bounds.width,bounds.height });
	entity.getComponent<CollisionComponent>().setCollisionMaskBits(CollisionFlags::Player);
	entity.getComponent<CollisionComponent>().setCollisionCategoryBits(CollisionType::Solid);

	sf::FloatRect bounds2(0, 100, 2000, 32);
	entity = mGameScene.createEntity();
	entity.addComponent<xy::Transform>().setPosition(bounds2.left, bounds2.top);
	entity.addComponent<CollisionComponent>().addHitbox({ 0,0,bounds2.width,bounds2.height }, CollisionType::Solid);
	entity.addComponent<xy::QuadTreeItem>().setArea({ 0,0,bounds2.width,bounds2.height });
	entity.getComponent<CollisionComponent>().setCollisionMaskBits(CollisionFlags::Player);
	entity.getComponent<CollisionComponent>().setCollisionCategoryBits(CollisionType::Solid);

	sf::FloatRect bounds3(800, 0, 32, 2000);
	entity = mGameScene.createEntity();
	entity.addComponent<xy::Transform>().setPosition(bounds3.left, bounds3.top);
	entity.addComponent<CollisionComponent>().addHitbox({ 0,0,bounds3.width,bounds3.height }, CollisionType::Solid);
	entity.addComponent<xy::QuadTreeItem>().setArea({ 0,0,bounds3.width,bounds3.height });
	entity.getComponent<CollisionComponent>().setCollisionMaskBits(CollisionFlags::Player);
	entity.getComponent<CollisionComponent>().setCollisionCategoryBits(CollisionType::Solid);
}