#include "PlayerController.h"
#include "PlayerSystem.h"

PlayerController::PlayerController(InputBinding binds)
	: mInputBinding(binds)
	, mEnabled(true)
	, mCurrentInput(0)
	, mRunningMultipler(0)
{
}

void PlayerController::handleEvent(const sf::Event& evt)
{
	if (evt.type == sf::Event::KeyPressed)
	{
		if (evt.key.code == mInputBinding.mKeyBinds[InputBinding::Action::Jump])
		{
			mCurrentInput |= InputFlag::Jump;
		}

		else if (evt.key.code == mInputBinding.mKeyBinds[InputBinding::Action::Left])
		{
			mCurrentInput |= InputFlag::Left;
		}
		else if (evt.key.code == mInputBinding.mKeyBinds[InputBinding::Action::Right])
		{
			mCurrentInput |= InputFlag::Right;
		}
	}
	else if (evt.type == sf::Event::KeyReleased)
	{
		if (evt.key.code == mInputBinding.mKeyBinds[InputBinding::Action::Jump])
		{
			mCurrentInput &= ~InputFlag::Jump;
		}
		else if (evt.key.code == mInputBinding.mKeyBinds[InputBinding::Action::Left])
		{
			mCurrentInput &= ~InputFlag::Left;
		}
		else if (evt.key.code == mInputBinding.mKeyBinds[InputBinding::Action::Right])
		{
			mCurrentInput &= ~InputFlag::Right;
		}
	}
}

void PlayerController::update()
{
	if (!mEnabled) {
		mCurrentInput = 0;
	}
	if (playerEntity.isValid()) {
		auto& player = playerEntity.getComponent<PlayerComponent>();
		player.inputFlags = mCurrentInput;
		player.mAccelerationMultipler = mRunningMultipler;
	}
	if ((mCurrentInput & (InputFlag::Left | InputFlag::Right)) == 0) mRunningMultipler = 1.f;
}