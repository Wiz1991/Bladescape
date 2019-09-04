#pragma once
#include <xyginext/core/State.hpp>

class GameState final : public xy::State
{
public:
	GameState(xy::StateStack&, xy::State::Context);

	bool handleEvent(const sf::Event&) override;
	void handleMessage(const xy::Message&) override;

	bool update(float dT) override;
	void draw();

	void initialiseScene();

	xy::StateID stateID()const override;

private:
};
