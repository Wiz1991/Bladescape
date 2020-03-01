#pragma once
#include <SFML/Window/Event.hpp>
#include <xyginext/ecs/Entity.hpp>
#include "InputBinding.h"

class PlayerController final
{
public:
	PlayerController(InputBinding& binds);
	void handleEvent(const sf::Event& evt);
	void update();
	void setPlayerEntity(xy::Entity ent) { playerEntity = ent; }
	xy::Entity getPlayerEntity() const { return playerEntity; }
	void setEnabled(bool flag) { mEnabled = flag; }
private:
	InputBinding& mInputBinding;
	xy::Entity playerEntity;
	sf::Uint32 mCurrentInput;
	float mRunningMultipler;
	bool mEnabled;
};
