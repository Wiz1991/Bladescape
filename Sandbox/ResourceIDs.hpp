#pragma once

#include <array>

#include <xyginext/ecs/components/Sprite.hpp>

namespace SpriteID
{
	enum
	{
		Player,
		Count
	};

	static std::array<xy::Sprite, SpriteID::Count> sprites = {};
}

namespace FontID
{
	enum
	{
		Count
	};

	static std::array<std::size_t, Count> fonts = {};
}
namespace ShaderID {
	enum {
		TileMap,
	};
}