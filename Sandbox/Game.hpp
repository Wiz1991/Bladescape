#pragma once

#include <xyginext/core/App.hpp>

class Game final : public xy::App
{
public:
	Game();
	~Game() = default;
	Game(const Game&) = delete;
	Game& operator = (const Game&) = delete;

private:

	xy::StateStack m_stateStack;

	void handleEvent(const sf::Event&) override;
	void handleMessage(const xy::Message&) override;

	void registerStates() override;
	void updateApp(float dt) override;
	void draw() override;

	bool initialise() override;
	void finalise() override;
};