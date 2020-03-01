#include "Game.hpp"
#include "States.hpp"
#include "MenuState.hpp"
#include "GameState.hpp"

#include <SFML/Window/Event.hpp>

Game::Game()
	: xy::App(/*sf::ContextSettings(0, 0, 0, 3, 2, sf::ContextSettings::Core)*/),
	m_stateStack({ *getRenderWindow(), *this })
{
}

//private
void Game::handleEvent(const sf::Event& evt)
{
	m_stateStack.handleEvent(evt);
}

void Game::handleMessage(const xy::Message& msg)
{
	m_stateStack.handleMessage(msg);
}

void Game::updateApp(float dt)
{
	m_stateStack.update(dt);
}

void Game::draw()
{
	m_stateStack.draw();
}

bool Game::initialise()
{
	registerStates();

	m_stateStack.pushState(States::GameState);

	getRenderWindow()->setKeyRepeatEnabled(false);
	getRenderWindow()->setView(m_stateStack.updateView());

	return true;
}

void Game::finalise()
{
	m_stateStack.clearStates();
	m_stateStack.applyPendingChanges();
}

void Game::registerStates()
{
	m_stateStack.registerState<MenuState>(States::MenuState);
	m_stateStack.registerState<GameState>(States::GameState);
}