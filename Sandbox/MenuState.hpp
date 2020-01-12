#pragma once

#include <xyginext/core/State.hpp>
#include <xyginext/ecs/Scene.hpp>

#include <SFML/Graphics/Font.hpp>

#include <xyginext/core/ConsoleClient.hpp>
#include <xyginext/gui/GuiClient.hpp>

class MenuState final : public xy::State, public xy::GuiClient, public xy::ConsoleClient
{
public:
	MenuState(xy::StateStack&, xy::State::Context);

private:
	bool handleEvent(const sf::Event& evt) override;

	void handleMessage(const xy::Message&) override;

	bool update(float dt) override;

	void draw() override;

	xy::StateID stateID() const override;

	xy::Scene m_scene;

	sf::Font m_font;

	void createScene();
};