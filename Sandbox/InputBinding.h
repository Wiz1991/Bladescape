#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <array>
namespace InputFlag {
	enum {
		Left = 0x1,
		Right = 0x2,
		Jump = 0x4
	};
}
struct InputBinding final {
	enum {
		Jump, Left, Right, Count
	};
	std::array<sf::Keyboard::Key, Count> keys = { sf::Keyboard::Key::W,sf::Keyboard::Key::A,sf::Keyboard::Key::D };
};