#pragma once
#include <xyginext/ecs/System.hpp>
class PlayerComponent {
public:
	PlayerComponent();

private:
};

class PlayerSystem final : public xy::System
{
public:
	PlayerSystem(xy::MessageBus& mb);

	void process(float dT);

	void resolveCollisions(xy::Entity entity);

private:
};
