#include "MenuState.hpp"
#include "States.hpp"

#include <xyginext/ecs/components/Transform.hpp>
#include <xyginext/ecs/components/Text.hpp>
#include <xyginext/ecs/components/Drawable.hpp>
#include <xyginext/ecs/components/UIHitBox.hpp>
#include <xyginext/ecs/components/Camera.hpp>

#include <xyginext/ecs/systems/TextSystem.hpp>
#include <xyginext/ecs/systems/RenderSystem.hpp>
#include <xyginext/ecs/systems/UISystem.hpp>
#include <xyginext/gui/Gui.hpp>

MenuState::MenuState(xy::StateStack& ss, xy::State::Context ctx) :
	xy::State(ss, ctx),
	m_scene(ctx.appInstance.getMessageBus())
{
	createScene();

	m_scene.getActiveCamera().getComponent<xy::Camera>().setView(ctx.defaultView.getSize());
	m_scene.getActiveCamera().getComponent<xy::Camera>().setViewport(ctx.defaultView.getViewport());

	ctx.appInstance.setMouseCursorVisible(true);
}

bool MenuState::handleEvent(const sf::Event& evt)
{
	m_scene.getSystem<xy::UISystem>().handleEvent(evt);
	m_scene.forwardEvent(evt);
	return true;
}

void MenuState::handleMessage(const xy::Message& msg)
{
	m_scene.forwardMessage(msg);
}

bool MenuState::update(float dt)
{
	m_scene.update(dt);
	return true;
}

void MenuState::draw()
{
	auto rw = getContext().appInstance.getRenderWindow();
	rw->draw(m_scene);
}

xy::StateID MenuState::stateID() const
{
	return States::MenuState;
}

void MenuState::createScene()
{
	//add the systems
	auto& messageBus = getContext().appInstance.getMessageBus();
	m_scene.addSystem<xy::TextSystem>(messageBus);
	m_scene.addSystem<xy::UISystem>(messageBus);
	m_scene.addSystem<xy::RenderSystem>(messageBus);

	m_font.loadFromFile("assets/main.ttf");
}