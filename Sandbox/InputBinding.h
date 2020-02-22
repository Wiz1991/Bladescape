#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <map>
namespace InputFlag {
	enum {
		Left = 0x1,
		Right = 0x2,
		Jump = 0x4,
	};
}
class InputBinding {
public:
	enum  Action {
		Jump, Left, Right, Count
	};
	InputBinding() {
		mKeyBinds[Action::Jump] = sf::Keyboard::Key::W;
		mKeyBinds[Action::Left] = sf::Keyboard::Key::A;
		mKeyBinds[Action::Right] = sf::Keyboard::Key::D;
	}
	std::map<Action, sf::Keyboard::Key> mKeyBinds;
};