#pragma once
#include <array>
namespace TextureID {
	enum {
		Paddle,
		Ball,
		Count
	};
	static std::array<std::size_t, TextureID::Count> handles = {};
}
namespace FontID {
	enum {
		Main,
		Count
	};
	static std::array<std::size_t, FontID::Count>handles = {};
}