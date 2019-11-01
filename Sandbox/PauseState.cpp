#include "PauseState.h"
#include <xyginext/ecs/components/Transform.hpp>
#include <xyginext/ecs/components/Drawable.hpp>
#include <xyginext/ecs/components/Text.hpp>
#include <xyginext/ecs/components/UIHitBox.hpp>

#include <xyginext/ecs/systems/RenderSystem.hpp>
#include <xyginext/ecs/systems/TextSystem.hpp>
#include <xyginext/ecs/systems/UISystem.hpp>

#include "ResourceIDs.h"
#include "States.h"

#include <SFML/Window/Event.hpp>

PauseState::PauseState(xy::StateStack& stack, xy::State::Context context)
	: xy::State(stack, context)
	, mPausescene(context.appInstance.getMessageBus())
{
	loadResources();
	initialiseScene();

	context.appInstance.setMouseCursorVisible(true);
	context.appInstance.getRenderWindow()->setMouseCursorGrabbed(false);
	context.appInstance.getRenderWindow()->setKeyRepeatEnabled(false);
}

bool PauseState::handleEvent(const sf::Event& evt)
{
	if (evt.type == sf::Event::KeyReleased && evt.key.code == sf::Keyboard::Escape)
		requestStackPop();
	mPausescene.getSystem<xy::UISystem>().handleEvent(evt);
	mPausescene.forwardEvent(evt);
	return true;
}

void PauseState::handleMessage(const xy::Message& msg)
{
	mPausescene.forwardMessage(msg);
}

bool PauseState::update(float dT)
{
	mPausescene.update(dT);
	return false;
}

void PauseState::draw()
{
	auto window = getContext().appInstance.getRenderWindow();
	window->draw(mPausescene);
}

void PauseState::initialiseScene()
{
	auto& msgBus = getContext().appInstance.getMessageBus();

	mPausescene.addSystem<xy::TextSystem>(msgBus);
	mPausescene.addSystem<xy::UISystem>(msgBus);
	mPausescene.addSystem<xy::RenderSystem>(msgBus);
	auto entity = mPausescene.createEntity();

	entity.addComponent<xy::Transform>().setPosition(xy::DefaultSceneSize.x / 2 - 40.f, xy::DefaultSceneSize.y / 2 - 50.f);
	entity.addComponent<xy::Text>(mResources.get<sf::Font>(FontID::handles[FontID::Main])).setString("Resume");
	entity.getComponent<xy::Text>().setCharacterSize(60);
	entity.addComponent<xy::Drawable>();
	entity.addComponent<xy::UIHitBox>().area = xy::Text::getLocalBounds(entity);

	auto callbackID = mPausescene.getSystem<xy::UISystem>().addMouseButtonCallback(
		[this](xy::Entity, sf::Uint64 flags) {
			if (flags & xy::UISystem::Flags::LeftMouse) {
				requestStackPop();
			}
		});
	entity.getComponent<xy::UIHitBox>().callbacks[xy::UIHitBox::MouseUp] = callbackID;
}

void PauseState::loadResources()
{
	FontID::handles[FontID::Main] = mResources.load<sf::Font>("main.ttf");
}

xy::StateID PauseState::stateID() const
{
	return States::PAUSE;
}