#pragma once
#include <array>
#include <xyginext/ecs/components/Sprite.hpp>
namespace SpriteID {
	enum {
		Paddle,
		Ball,
		Block,
		Count
	};
	static std::array<xy::Sprite, SpriteID::Count> sprites = {};
}
namespace FontID {
	enum {
		Main,
		Count
	};
	static std::array<std::size_t, FontID::Count>handles = {};
}