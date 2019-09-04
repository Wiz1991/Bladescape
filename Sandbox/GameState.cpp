#include "GameState.h"
#include "States.h"

GameState::GameState(xy::StateStack& stack, xy::State::Context context)
	: xy::State(stack, context)
{
}

bool GameState::handleEvent(const sf::Event&)
{
	return false;
}

void GameState::handleMessage(const xy::Message&)
{
}

bool GameState::update(float dT)
{
	return false;
}

void GameState::draw()
{
}

void GameState::initialiseScene()
{
}

xy::StateID GameState::stateID() const
{
	return States::GAME;
}