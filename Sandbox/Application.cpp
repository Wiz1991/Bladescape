#include "Application.h"
#include "States.h"
#include "MenuState.h"
#include "PauseState.h"
#include "GameState.h"
#include <SFML/Window/Event.hpp>

Application::Application()
	: xy::App(sf::ContextSettings(0, 0, 0, 1, 1, 0))
	, mStateStack(xy::State::Context(*getRenderWindow(), *this))
{
}

void Application::handleEvent(const sf::Event& evt)
{
	mStateStack.handleEvent(evt);
}

void Application::handleMessage(const xy::Message& msg)
{
	mStateStack.handleMessage(msg);
}

void Application::updateApp(float dT)
{
	mStateStack.update(dT);
}

void Application::draw()
{
	mStateStack.draw();
}

void Application::registerStates()
{
	mStateStack.registerState<MenuState>(States::MENU);
	mStateStack.registerState<GameState>(States::GAME);
	mStateStack.registerState<PauseState>(States::PAUSE);
#ifdef XY_DEBUG
	//mStateStack.pushState(States::MENU);
#endif // XY_DEBUG
	mStateStack.pushState(States::MENU);  //Change with INTRO  STATE and enable
											   //direct entry to menu in debug mode
}

bool Application::initialise()
{
	registerStates();

	getRenderWindow()->setView(mStateStack.updateView());

	setWindowTitle("Bladescape");

	return true;
}

void Application::finalise()
{
	mStateStack.clearStates();
}