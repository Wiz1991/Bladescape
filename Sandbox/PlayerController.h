#pragma once
#include <xyginext/ecs/Entity.hpp>
#include <SFML/Window/Event.hpp>
#include "InputBinding.h"
class PlayerController final
{
public:
	PlayerController(const InputBinding&);
	void handleEvent(const sf::Event&);
	void update();
	void setPlayerEntity(xy::Entity p) { mPlayer = p; }
	xy::Entity getPlayerEntity()const { return mPlayer; }

private:
	xy::Entity mPlayer;
	const InputBinding& mInputBinding;
	std::uint16_t mCurrentInput;
};
