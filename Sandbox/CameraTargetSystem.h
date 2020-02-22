#pragma once
#include <xyginext/ecs/System.hpp>

class CameraTargetSystem final : public xy::System
{
public:
	explicit CameraTargetSystem(xy::MessageBus& mb);

	void process(float) override;

	void setBounds(sf::FloatRect bounds) { m_bounds = bounds; }

private:

	sf::FloatRect m_bounds;
	std::vector<float> m_shakeTable;
	std::size_t m_shakeIndex;
};
struct CameraTarget final
{
	xy::Entity lastTarget;
	xy::Entity target;

	//when switching targets we can smooth between those too...
	sf::Vector2f targetPosition;
	sf::Vector2f targetVelocity;

	sf::Vector2f velocity;

	float shakeAmount = 0.f;
};