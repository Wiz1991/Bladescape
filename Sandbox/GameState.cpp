#include "GameState.h"
#include "States.h"
#include "CommandIDs.h"
#include "ResourceIDs.h"
//components
#include <xyginext/ecs/components/Camera.hpp>
#include <xyginext/ecs/components/Drawable.hpp>
#include <xyginext/ecs/components/Sprite.hpp>
#include <xyginext/ecs/components/Transform.hpp>
#include <xyginext/ecs/components/CommandTarget.hpp>

//systems
#include <xyginext/ecs/systems/RenderSystem.hpp>
#include <xyginext/ecs/systems/SpriteSystem.hpp>
#include <xyginext/ecs/systems/CommandSystem.hpp>

//SFML
#include <SFML/Window/Event.hpp>

//REST

GameState::GameState(xy::StateStack& stack, xy::State::Context context)
	: xy::State(stack, context)
	, mGameScene(context.appInstance.getMessageBus())
{
	loadResources();
	initialiseScene();

	mGameScene.getActiveCamera().getComponent<xy::Camera>().setView(context.defaultView.getSize());
	mGameScene.getActiveCamera().getComponent<xy::Camera>().setViewport(context.defaultView.getViewport());

	context.appInstance.setMouseCursorVisible(false);
}

bool GameState::handleEvent(const sf::Event& evt)
{
	if (evt.type == sf::Event::MouseMoved) {
		auto mouseWorldPos = xy::App::getRenderWindow()->mapPixelToCoords({ evt.mouseMove.x,evt.mouseMove.y });
		xy::Command cmd;
		cmd.targetFlags = CommandID::Paddle;
		cmd.action = [mouseWorldPos](xy::Entity entity, float) {
			float posX = mouseWorldPos.x;
			auto spriteWidth = entity.getComponent<xy::Sprite>().getTextureBounds().width / 2;
			posX = std::max(spriteWidth, mouseWorldPos.x);
			posX = std::min(posX, xy::DefaultSceneSize.x - spriteWidth);

			auto& tx = entity.getComponent<xy::Transform>();
			auto currentPos = tx.getPosition();
			currentPos.x = posX;
			tx.setPosition(currentPos);
		};
		mGameScene.getSystem<xy::CommandSystem>().sendCommand(cmd);
	}
	mGameScene.forwardEvent(evt);
	return true;
}

void GameState::handleMessage(const xy::Message& msg)
{
	mGameScene.forwardMessage(msg);
}

bool GameState::update(float dT)
{
	mGameScene.update(dT);
	return true;
}

void GameState::draw()
{
	auto window = getContext().appInstance.getRenderWindow();

	window->draw(mGameScene);
}

void GameState::initialiseScene()
{
	auto& msgBus = getContext().appInstance.getMessageBus();

	mGameScene.addSystem<xy::CommandSystem>(msgBus);
	mGameScene.addSystem<xy::SpriteSystem>(msgBus);
	mGameScene.addSystem<xy::RenderSystem>(msgBus);

	auto entity = mGameScene.createEntity();

	entity.addComponent<xy::Transform>().setPosition(xy::DefaultSceneSize.x / 2, xy::DefaultSceneSize.y - 100);
	entity.addComponent<xy::Sprite>(mResources.get<sf::Texture>(TextureID::handles[TextureID::Paddle]));
	entity.addComponent<xy::Drawable>();
	auto paddleBounds = entity.getComponent<xy::Sprite>().getTextureBounds();
	entity.getComponent<xy::Transform>().setOrigin(paddleBounds.width / 2, paddleBounds.height / 2);
	entity.addComponent<xy::CommandTarget>().ID = CommandID::Paddle;
}

void GameState::loadResources()
{
	TextureID::handles[TextureID::Paddle] = mResources.load<sf::Texture>("button.png");
}

xy::StateID GameState::stateID() const
{
	return States::GAME;
}