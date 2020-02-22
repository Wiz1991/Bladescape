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
#include "CameraTargetSystem.h"
#include <xyginext/ecs/systems/SpriteSystem.hpp>
#include <xyginext/ecs/systems/CommandSystem.hpp>
#include <xyginext/ecs/systems/RenderSystem.hpp>
#include <xyginext/ecs/systems/TextSystem.hpp>
#include <xyginext/ecs/systems/CallbackSystem.hpp>
#include <xyginext/ecs/systems/CameraSystem.hpp>
#include "PlayerSystem.h"

#include <xyginext/ecs/systems/SpriteAnimator.hpp>

#include <xyginext/core/App.hpp>
#include <xyginext/util/Vector.hpp>
#include <xyginext/graphics/SpriteSheet.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include "InputBinding.h"

GameState::GameState(xy::StateStack& ss, xy::State::Context ctx)
	: xy::State(ss, ctx),
	mGameScene(ctx.appInstance.getMessageBus()),
	mLevel("assets/maps/map1.tmx"),
	mPlayerController(InputBinding())
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
	rw->draw(mLevel);
	rw->draw(mGameScene);
}

xy::StateID GameState::stateID() const
{
	return States::GameState;
}

void GameState::buildWorld()
{
	auto scale = GameConsts::PixelsPerTile / mLevel.getTileSize();

	loadCollisions();
	//create player
	static const sf::FloatRect ClocksyBounds(0, 0, 16, 16);
	static const sf::FloatRect ClocksyFoot(0, 16, 16, 1);

	static const sf::FloatRect PlayerBounds = ClocksyBounds;
	static const sf::FloatRect PlayerFoot = ClocksyFoot;
	static const sf::Vector2f PlayerOrigin(8, 16);

	auto entity = mGameScene.createEntity();
	entity.addComponent<xy::Sprite>() = mSprites[SpriteID::Player];
	entity.addComponent<xy::SpriteAnimation>().play(AnimID::Player::Idle);
	entity.addComponent<xy::Drawable>();

	entity.addComponent<xy::Camera>();
	entity.addComponent<xy::Transform>().setPosition(40, 304);
	entity.addComponent<CollisionComponent>().addHitbox(PlayerBounds, CollisionType::Player);
	entity.getComponent<CollisionComponent>().addHitbox(PlayerFoot, CollisionType::Foot);
	entity.getComponent<CollisionComponent>().setCollisionCategoryBits(CollisionFlags::Player);
	entity.getComponent<CollisionComponent>().setCollisionMaskBits(CollisionFlags::PlayerMask);
	entity.addComponent<xy::BroadphaseComponent>().setArea(entity.getComponent<xy::Drawable>().getLocalBounds());
	entity.addComponent<Player>();
	mPlayerController.setPlayerEntity(entity);
	player = entity;

	mGameScene.getActiveCamera().addComponent<CameraTarget>().target = entity;
	//mGameScene.getActiveCamera().getComponent<xy::Camera>().setZoom(scale);
	mGameScene.getActiveCamera().getComponent<xy::Camera>().setBounds(sf::FloatRect(sf::Vector2f(), mLevel.getWorldSize()));
	mGameScene.getSystem<CameraTargetSystem>().setBounds({ sf::Vector2f(),mLevel.getWorldSize() });
}

void GameState::loadResources()
{
	xy::SpriteSheet spriteSheet;
	spriteSheet.loadFromFile("assets/sprites/playerold.spt", mTextureResource);
	mSprites[SpriteID::Player] = spriteSheet.getSprite("player");
	mPlayerAnimations[AnimID::Player::Idle] = spriteSheet.getAnimationIndex("idle", "player");
	mPlayerAnimations[AnimID::Player::Run] = spriteSheet.getAnimationIndex("run", "player");
	mPlayerAnimations[AnimID::Player::Jump] = spriteSheet.getAnimationIndex("jump", "player");
}

void GameState::initScene()
{
	auto& mb = getContext().appInstance.getMessageBus();
	mGameScene.addSystem<xy::DynamicTreeSystem>(mb);
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
	auto& collisions = mLevel.getCollisionData();
	for (auto& col : collisions) {
		auto entity = mGameScene.createEntity();
		entity.addComponent<xy::Transform>().setPosition(col.bounds.left, col.bounds.top);
		col.bounds.left = 0.f; //reset to local transform
		col.bounds.top = 0.f;
		entity.addComponent<CollisionComponent>().addHitbox(sf::FloatRect(col.bounds.left, col.bounds.top, col.bounds.width, col.bounds.height), CollisionType::Solid);
		entity.addComponent<xy::BroadphaseComponent>().setArea(col.bounds);
	}
}