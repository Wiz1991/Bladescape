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

#include <xyginext/ecs/systems/DynamicTreeSystem.hpp>
#include <xyginext/ecs/systems/SpriteSystem.hpp>
#include <xyginext/ecs/systems/CommandSystem.hpp>
#include <xyginext/ecs/systems/RenderSystem.hpp>
#include <xyginext/ecs/systems/TextSystem.hpp>
#include <xyginext/ecs/systems/CallbackSystem.hpp>
#include <xyginext/ecs/systems/CameraSystem.hpp>

#include <xyginext/ecs/systems/SpriteAnimator.hpp>

#include <xyginext/core/App.hpp>
#include <xyginext/util/Vector.hpp>
#include <xyginext/graphics/SpriteSheet.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>

GameState::GameState(xy::StateStack& ss, xy::State::Context ctx)
	: xy::State(ss, ctx),
	mGameScene(ctx.appInstance.getMessageBus()),
	mMapLoader()

{
	initScene();
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		mGameScene.getEntity(player).getComponent<xy::Transform>().move(0, 2);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		mGameScene.getEntity(player).getComponent<xy::Transform>().move(0, -2);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		mGameScene.getEntity(player).getComponent<xy::Transform>().move(2, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		mGameScene.getEntity(player).getComponent<xy::Transform>().move(-2, 0);
	}
	mGameScene.update(dt);
	return true;
}

void GameState::draw()
{
	auto rw = getContext().appInstance.getRenderWindow();

	rw->draw(mGameScene);
}

xy::StateID GameState::stateID() const
{
	return States::GameState;
}

void GameState::buildWorld()
{
	mMapLoader.load("map1.tmx");
	auto& layer = mMapLoader.getLayer();
	auto scale = GameConsts::PixelsPerTile / mMapLoader.getTileSize();
	//load map
	std::int32_t depth = -50 + 2;
	for (auto& l : layer) {
		auto entity = mGameScene.createEntity();
		entity.addComponent<xy::Transform>();
		entity.addComponent<xy::Drawable>().setDepth(depth);
		entity.addComponent<xy::Sprite>(l);
		//entity.getComponent<xy::Transform>().setScale(scale, scale);
		depth += 4;
	}
	loadCollisions();
	//create player
	auto entity = mGameScene.createEntity();
	entity.addComponent<xy::Drawable>();
	Shape::setRectangle(entity.getComponent<xy::Drawable>(), sf::Vector2f(16, 16), sf::Color::White);
	entity.addComponent<xy::Camera>();
	entity.addComponent<xy::Transform>().setPosition(40, 304);
	entity.addComponent<xy::BroadphaseComponent>().setArea(entity.getComponent<xy::Drawable>().getLocalBounds());
	entity.addComponent<CollisionComponent>().dynamic = true;
	mGameScene.setActiveCamera(entity);
	mGameScene.getActiveCamera().getComponent<xy::Camera>().setBounds(sf::FloatRect(sf::Vector2f(), mMapLoader.getWorldSize()));
	player = entity.getIndex();
	mGameScene.getActiveCamera().getComponent<xy::Camera>().setZoom(scale);
}

void GameState::loadResources()
{
	xy::SpriteSheet spriteSheet;
}

void GameState::initScene()
{
	auto& mb = getContext().appInstance.getMessageBus();
	mGameScene.addSystem<xy::DynamicTreeSystem>(mb);
	mGameScene.addSystem<CollisionSystem>(mb);
	mGameScene.addSystem<xy::CameraSystem>(mb);
	mGameScene.addSystem<xy::SpriteSystem>(mb);
	mGameScene.addSystem<xy::RenderSystem>(mb);
}

void GameState::loadCollisions()
{
	auto& collisions = mMapLoader.getCollisionShapes();
	for (auto& col : collisions) {
		auto entity = mGameScene.createEntity();
		entity.addComponent<xy::Transform>().setPosition(col.left, col.top);
		col.left = 0.f; //reset to local transform
		col.top = 0.f;
		entity.addComponent<xy::BroadphaseComponent>().setArea(col);
		entity.addComponent<CollisionComponent>();
	}
}