#pragma once
#include <xyginext/core/State.hpp>
#include <xyginext/ecs/Scene.hpp>
#include <xyginext/resources/ResourceHandler.hpp>

#include <SFML/Graphics/Font.hpp>

class PauseState final : public xy::State
{
public:
	PauseState(xy::StateStack&, xy::State::Context);

	bool handleEvent(const sf::Event&) override;
	void handleMessage(const xy::Message&) override;

	bool update(float dT) override;
	void draw();

	void initialiseScene();
	void loadResources();

	xy::StateID stateID()const override;

private:
	xy::Scene mPausescene;
	xy::ResourceHandler mResources;
};
