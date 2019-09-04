#include "MenuState.h"

#include "States.h"

#include <xyginext/ecs/components/Transform.hpp>
#include <xyginext/ecs/components/Drawable.hpp>
#include <xyginext/ecs/components/Text.hpp>
#include <xyginext/ecs/components/UIHitBox.hpp>
#include <xyginext/ecs/systems/RenderSystem.hpp>
#include <xyginext/ecs/systems/TextSystem.hpp>
#include <xyginext/ecs/systems/UISystem.hpp>

MenuState::MenuState(xy::StateStack& stack, xy::State::Context context)
	: xy::State(stack, context)
	, mScene(context.appInstance.getMessageBus())
{
	if (!mFont.loadFromFile("ProggyClean.ttf")) xy::Logger::log("Failed to open ProggyClean.ttf");
	initialiseScene();
}

bool MenuState::handleEvent(const sf::Event& evt)
{
	mScene.getSystem<xy::UISystem>().handleEvent(evt);

	mScene.forwardEvent(evt);

	return true;
}

void MenuState::handleMessage(const xy::Message& msg)
{
	mScene.forwardMessage(msg);
}

bool MenuState::update(float dT)
{
	mScene.update(dT);
	return true;
}

void MenuState::draw()
{
	auto& window = *getContext().appInstance.getRenderWindow();

	window.draw(mScene);
}

void MenuState::initialiseScene()
{
	auto& bus = getContext().appInstance.getMessageBus();

	mScene.addSystem<xy::TextSystem>(bus);
	mScene.addSystem<xy::UISystem>(bus);
	mScene.addSystem<xy::RenderSystem>(bus);

	auto entity = mScene.createEntity();
	entity.addComponent<xy::Transform>().setPosition(100, 100);
	entity.addComponent<xy::Text>(mFont).setString("Bladescape");
	entity.getComponent<xy::Text>().setCharacterSize(95);
	entity.addComponent<xy::Drawable>();

	entity = mScene.createEntity();
	entity.addComponent<xy::Transform>().setPosition(100, 220);
	entity.addComponent<xy::Text>(mFont).setString("Play");
	entity.getComponent<xy::Text>().setCharacterSize(64);
	entity.addComponent<xy::Drawable>();
	entity.addComponent<xy::UIHitBox>().area = xy::Text::getLocalBounds(entity);

	entity = mScene.createEntity();
	entity.addComponent<xy::Transform>().setPosition(100, 285);
	entity.addComponent<xy::Text>(mFont).setString("Quit");
	entity.getComponent<xy::Text>().setCharacterSize(64);
	entity.addComponent<xy::Drawable>();
	entity.addComponent<xy::UIHitBox>().area = xy::Text::getLocalBounds(entity);

	auto callbackID = mScene.getSystem<xy::UISystem>().addMouseButtonCallback(
		[&](xy::Entity, sf::Uint64 flags) {
			if (flags & xy::UISystem::Flags::LeftMouse) {
				getContext().appInstance.quit();
			}
		});

	entity.getComponent<xy::UIHitBox>().callbacks[xy::UIHitBox::MouseUp] = callbackID;
}

xy::StateID MenuState::stateID() const
{
	return States::MENU;
}