#include "PlayerController.h"
#include "PlayerSystem.h"
#include "KeyMapping.h"

PlayerController::PlayerController(const InputBinding& binds)
	: mInputBinding(binds),
	mCurrentInput(0)

{
}

void PlayerController::handleEvent(const sf::Event& evt)
{
	if (evt.type == sf::Event::KeyPressed) {
		if (evt.key.code == sf::Keyboard::A) {
			mCurrentInput |= InputBinding::Left;
		}
		if (evt.key.code == sf::Keyboard::D) {
			mCurrentInput |= InputBinding::Right;
		}
	}
	if (evt.type == sf::Event::KeyReleased) {
		if (evt.key.code == sf::Keyboard::A) {
			mCurrentInput &= ~InputBinding::Left;
		}
		if (evt.key.code == sf::Keyboard::D) {
			mCurrentInput &= ~InputBinding::Right;
		}
	}
}

void PlayerController::update()
{
	if (mPlayer.isValid()) {
		auto& player = mPlayer.getComponent<PlayerComponent>();

		player.mInput = mCurrentInput;
	}
}