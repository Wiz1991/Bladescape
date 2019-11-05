#pragma once
#include <xyginext/core/State.hpp>
#include <xyginext/ecs/Scene.hpp>
#include <xyginext/resources/ResourceHandler.hpp>
class GameState final : public xy::State
{
public:
	GameState(xy::StateStack&, xy::State::Context);

	bool handleEvent(const sf::Event&) override;
	void handleMessage(const xy::Message&) override;

	bool update(float dT) override;
	void draw();

	void initialiseScene();
	void loadResources();

	xy::StateID stateID()const override;

private:
	void spawnBall();

private:
	xy::Scene mGameScene;
	xy::ResourceHandler mResources;
};
