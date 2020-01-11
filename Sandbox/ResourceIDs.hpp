#pragma once

#include <array>

#include <xyginext/ecs/components/Sprite.hpp>

namespace SpriteID
{
	enum
	{
		Player = 0,
		Block,
		Count,
	};

	static std::array<xy::Sprite, Count> sprites = {};
}

namespace FontID
{
	enum
	{
		Main = 0,
		Count
	};

	static std::array<std::size_t, Count> fonts = {};
}
namespace ShaderID {
	enum {
	};
}