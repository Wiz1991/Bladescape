#include "MenuState.h"
#include "ResourceIDs.h"
#include "States.h"
//COMPONENTS
#include <xyginext/ecs/components/Transform.hpp>
#include <xyginext/ecs/components/Drawable.hpp>
#include <xyginext/ecs/components/Text.hpp>
#include <xyginext/ecs/components/UIHitBox.hpp>
#include <xyginext/ecs/components/Camera.hpp>

//SYSTEMS
#include <xyginext/ecs/systems/RenderSystem.hpp>
#include <xyginext/ecs/systems/TextSystem.hpp>
#include <xyginext/ecs/systems/UISystem.hpp>

#include <SFML/Window/Event.hpp>

MenuState::MenuState(xy::StateStack& stack, xy::State::Context context)
	: xy::State(stack, context)
	, mScene(context.appInstance.getMessageBus())
{
	loadResources();
	initialiseScene();

	mScene.getActiveCamera().getComponent<xy::Camera>().setView(context.defaultView.getSize());
	mScene.getActiveCamera().getComponent<xy::Camera>().setViewport(context.defaultView.getViewport());
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
	//ADDING SYSTEMS
	mScene.addSystem<xy::TextSystem>(bus);
	mScene.addSystem<xy::UISystem>(bus);
	mScene.addSystem<xy::RenderSystem>(bus);

	//TITLE
	auto entity = mScene.createEntity();
	entity.addComponent<xy::Transform>().setPosition(100, 100);
	entity.addComponent<xy::Text>(mResources.get<sf::Font>(FontID::handles[FontID::Main])).setString("Bladescape");
	entity.getComponent<xy::Text>().setCharacterSize(95);
	entity.addComponent<xy::Drawable>();

	//PLAY BUTTON
	entity = mScene.createEntity();
	entity.addComponent<xy::Transform>().setPosition(100, 220);
	entity.addComponent<xy::Text>(mResources.get<sf::Font>(FontID::handles[FontID::Main])).setString("Play");
	entity.getComponent<xy::Text>().setCharacterSize(64);
	entity.addComponent<xy::Drawable>();
	entity.addComponent<xy::UIHitBox>().area = xy::Text::getLocalBounds(entity);
	auto callbackID = mScene.getSystem<xy::UISystem>().addMouseButtonCallback([&](xy::Entity, sf::Uint64 flags) {
		if (flags & xy::UISystem::Flags::LeftMouse) {
			requestStackClear();
			requestStackPush(States::GAME);
		}
		});
	entity.getComponent<xy::UIHitBox>().callbacks[xy::UIHitBox::MouseUp] = callbackID;

	//QUIT BUTTON
	entity = mScene.createEntity();
	entity.addComponent<xy::Transform>().setPosition(100, 285);
	entity.addComponent<xy::Text>(mResources.get<sf::Font>(FontID::handles[FontID::Main])).setString("Quit");
	entity.getComponent<xy::Text>().setCharacterSize(64);
	entity.addComponent<xy::Drawable>();
	entity.addComponent<xy::UIHitBox>().area = xy::Text::getLocalBounds(entity);
	callbackID = mScene.getSystem<xy::UISystem>().addMouseButtonCallback(
		[&](xy::Entity, sf::Uint64 flags) {
			if (flags & xy::UISystem::Flags::LeftMouse) {
				getContext().appInstance.quit();
			}
		});
	entity.getComponent<xy::UIHitBox>().callbacks[xy::UIHitBox::MouseUp] = callbackID;
}

void MenuState::loadResources()
{
	FontID::handles[FontID::Main] = mResources.load<sf::Font>("assets/main.ttf");
}

xy::StateID MenuState::stateID() const
{
	return States::MENU;
}