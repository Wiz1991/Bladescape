#pragma once
#include <xyginext/core/App.hpp>

class Application final : public  xy::App
{
public:
	Application();
private:

	void handleEvent(const sf::Event&) override;
	void handleMessage(const xy::Message&) override;

	void updateApp(float dT);
	void draw();

	void registerStates() override;

	bool  initialise() override;
	void finalise() override;

private:
	xy::StateStack mStateStack;
};
