#pragma once
#include <xyginext/core/Message.hpp>

namespace MessageID {
	enum {
		BallMessage = xy::Message::Count
	};
}
struct BallEvent final {
	enum {
		Spawned, Despawned
	}action;
	sf::Vector2f position;
};