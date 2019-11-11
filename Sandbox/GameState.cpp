#include "GameState.hpp"
#include "States.hpp"
#include "CommandIDs.hpp"
#include "ResourceIDs.hpp"
#include "CollisionSystem.hpp"
#include "MessageIDs.hpp"
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
	//look for mouse move events and send them to the paddle

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

	auto entity = mGameScene.createEntity();
	entity.addComponent<xy::Transform>();
	entity.addComponent<xy::Drawable>();
	entity.addComponent<xy::Sprite>(layer);

	//create player
	entity = mGameScene.createEntity();
	entity.addComponent<xy::Drawable>();
	Shape::setRectangle(entity.getComponent<xy::Drawable>(), sf::Vector2f(16, 16), sf::Color::Red);
	entity.addComponent<xy::Camera>();
	entity.addComponent<xy::Transform>().setPosition(32, 400);
	mGameScene.setActiveCamera(entity);
	mGameScene.getActiveCamera().getComponent<xy::Camera>().setBounds(sf::FloatRect(0, 0, 1920, 416));
	mGameScene.getActiveCamera().getComponent<xy::Camera>().setZoom(3.2);
}

void GameState::loadResources()
{
	xy::SpriteSheet spriteSheet;
}

void GameState::initScene()
{
	auto& mb = getContext().appInstance.getMessageBus();

	mGameScene.addSystem<xy::RenderSystem>(mb);
	mGameScene.addSystem<xy::CameraSystem>(mb);
	mGameScene.addSystem<xy::SpriteSystem>(mb);
}