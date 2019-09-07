#pragma once
#include <xyginext/core/State.hpp>
#include <xyginext/ecs/Scene.hpp>
#include <xyginext/resources/ResourceHandler.hpp>

#include <SFML/Graphics/Font.hpp>
class MenuState final : public xy::State
{
public:
	MenuState(xy::StateStack&, xy::State::Context);

	bool handleEvent(const sf::Event&) override;
	void handleMessage(const xy::Message&) override;

	bool update(float dT) override;
	void draw();

	void initialiseScene();
	void loadResources();

	xy::StateID stateID()const override;

private:
	xy::Scene mScene;
	xy::ResourceHandler mResources;
};
