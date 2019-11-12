#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <map>
#include <string>

static const std::map<sf::Keyboard::Key, std::string> KeyMapping =
{
	std::make_pair(sf::Keyboard::Unknown, "Invalid"),
	std::make_pair(sf::Keyboard::A, "A"),
	std::make_pair(sf::Keyboard::B, "B"),
	std::make_pair(sf::Keyboard::C, "C"),
	std::make_pair(sf::Keyboard::D, "D"),
	std::make_pair(sf::Keyboard::E, "E"),
	std::make_pair(sf::Keyboard::F, "F"),
	std::make_pair(sf::Keyboard::G, "G"),
	std::make_pair(sf::Keyboard::H, "H"),
	std::make_pair(sf::Keyboard::I, "I"),
	std::make_pair(sf::Keyboard::J, "J"),
	std::make_pair(sf::Keyboard::K, "K"),
	std::make_pair(sf::Keyboard::L, "L"),
	std::make_pair(sf::Keyboard::M, "M"),
	std::make_pair(sf::Keyboard::N, "N"),
	std::make_pair(sf::Keyboard::O, "O"),
	std::make_pair(sf::Keyboard::P, "P"),
	std::make_pair(sf::Keyboard::Q, "Q"),
	std::make_pair(sf::Keyboard::R, "R"),
	std::make_pair(sf::Keyboard::S, "S"),
	std::make_pair(sf::Keyboard::T, "T"),
	std::make_pair(sf::Keyboard::U, "U"),
	std::make_pair(sf::Keyboard::V, "V"),
	std::make_pair(sf::Keyboard::W, "W"),
	std::make_pair(sf::Keyboard::X, "X"),
	std::make_pair(sf::Keyboard::Y, "Y"),
	std::make_pair(sf::Keyboard::Z, "Z"),
	std::make_pair(sf::Keyboard::Num0, "0"),
	std::make_pair(sf::Keyboard::Num1, "1"),
	std::make_pair(sf::Keyboard::Num2, "2"),
	std::make_pair(sf::Keyboard::Num3, "3"),
	std::make_pair(sf::Keyboard::Num4, "4"),
	std::make_pair(sf::Keyboard::Num5, "5"),
	std::make_pair(sf::Keyboard::Num6, "6"),
	std::make_pair(sf::Keyboard::Num7, "7"),
	std::make_pair(sf::Keyboard::Num8, "8"),
	std::make_pair(sf::Keyboard::Num9, "9"),
	std::make_pair(sf::Keyboard::Escape, "Esc"),
	std::make_pair(sf::Keyboard::LControl, "LCtrl"),
	std::make_pair(sf::Keyboard::LShift, "LShift"),
	std::make_pair(sf::Keyboard::LAlt, "LAlt"),
	std::make_pair(sf::Keyboard::LSystem, "Sys"),
	std::make_pair(sf::Keyboard::RControl, "RCtrl"),
	std::make_pair(sf::Keyboard::RShift, "RShift"),
	std::make_pair(sf::Keyboard::RAlt, "RAlt"),
	std::make_pair(sf::Keyboard::RSystem, "Sys"),
	std::make_pair(sf::Keyboard::Menu, "Menu"),
	std::make_pair(sf::Keyboard::LBracket, "{"),
	std::make_pair(sf::Keyboard::RBracket, "}"),
	std::make_pair(sf::Keyboard::SemiColon, ";"),
	std::make_pair(sf::Keyboard::Comma, ","),
	std::make_pair(sf::Keyboard::Period, "."),
	std::make_pair(sf::Keyboard::Quote, "\""),
	std::make_pair(sf::Keyboard::Slash, "/"),
	std::make_pair(sf::Keyboard::BackSlash, "\\"),
	std::make_pair(sf::Keyboard::Tilde, "~"),
	std::make_pair(sf::Keyboard::Equal, "="),
	std::make_pair(sf::Keyboard::Dash, "-"),
	std::make_pair(sf::Keyboard::Space, "Space"),
	std::make_pair(sf::Keyboard::Return, "Return"),
	std::make_pair(sf::Keyboard::BackSpace, "Backspace"),
	std::make_pair(sf::Keyboard::Tab, "Tab"),
	std::make_pair(sf::Keyboard::PageUp, "PgUp"),
	std::make_pair(sf::Keyboard::PageDown, "PgDn"),
	std::make_pair(sf::Keyboard::End, "End"),
	std::make_pair(sf::Keyboard::Home, "Home"),
	std::make_pair(sf::Keyboard::Insert, "Ins"),
	std::make_pair(sf::Keyboard::Delete, "Del"),
	std::make_pair(sf::Keyboard::Add, "+"),
	std::make_pair(sf::Keyboard::Subtract, "-"),
	std::make_pair(sf::Keyboard::Multiply, "*"),
	std::make_pair(sf::Keyboard::Divide, "/"),
	std::make_pair(sf::Keyboard::Left, "Left"),
	std::make_pair(sf::Keyboard::Right, "Right"),
	std::make_pair(sf::Keyboard::Up, "Up"),
	std::make_pair(sf::Keyboard::Down, "Down"),
	std::make_pair(sf::Keyboard::Numpad0, "Keypad 0"),
	std::make_pair(sf::Keyboard::Numpad1, "Keypad 1"),
	std::make_pair(sf::Keyboard::Numpad2, "Keypad 2"),
	std::make_pair(sf::Keyboard::Numpad3, "Keypad 3"),
	std::make_pair(sf::Keyboard::Numpad4, "Keypad 4"),
	std::make_pair(sf::Keyboard::Numpad5, "Keypad 5"),
	std::make_pair(sf::Keyboard::Numpad6, "Keypad 6"),
	std::make_pair(sf::Keyboard::Numpad7, "Keypad 7"),
	std::make_pair(sf::Keyboard::Numpad8, "Keypad 8"),
	std::make_pair(sf::Keyboard::Numpad9, "Keypad 9"),
	std::make_pair(sf::Keyboard::F1, "F1"),
	std::make_pair(sf::Keyboard::F2, "F2"),
	std::make_pair(sf::Keyboard::F3, "F3"),
	std::make_pair(sf::Keyboard::F4, "F4"),
	std::make_pair(sf::Keyboard::F5, "F5"),
	std::make_pair(sf::Keyboard::F6, "F6"),
	std::make_pair(sf::Keyboard::F7, "F7"),
	std::make_pair(sf::Keyboard::F8, "F8"),
	std::make_pair(sf::Keyboard::F9, "F9"),
	std::make_pair(sf::Keyboard::F10, "F10"),
	std::make_pair(sf::Keyboard::F11, "F11"),
	std::make_pair(sf::Keyboard::F12, "F12"),
	std::make_pair(sf::Keyboard::F13, "F13"),
	std::make_pair(sf::Keyboard::F14, "F14"),
	std::make_pair(sf::Keyboard::F15, "F15"),
	std::make_pair(sf::Keyboard::Pause, "Pause")
};
std::string toString(sf::Keyboard::Key key)
{
#define BOOK_KEYTOSTRING_CASE(KEY) case sf::Keyboard::KEY: return #KEY;

	switch (key)
	{
		BOOK_KEYTOSTRING_CASE(Unknown)
			BOOK_KEYTOSTRING_CASE(A)
			BOOK_KEYTOSTRING_CASE(B)
			BOOK_KEYTOSTRING_CASE(C)
			BOOK_KEYTOSTRING_CASE(D)
			BOOK_KEYTOSTRING_CASE(E)
			BOOK_KEYTOSTRING_CASE(F)
			BOOK_KEYTOSTRING_CASE(G)
			BOOK_KEYTOSTRING_CASE(H)
			BOOK_KEYTOSTRING_CASE(I)
			BOOK_KEYTOSTRING_CASE(J)
			BOOK_KEYTOSTRING_CASE(K)
			BOOK_KEYTOSTRING_CASE(L)
			BOOK_KEYTOSTRING_CASE(M)
			BOOK_KEYTOSTRING_CASE(N)
			BOOK_KEYTOSTRING_CASE(O)
			BOOK_KEYTOSTRING_CASE(P)
			BOOK_KEYTOSTRING_CASE(Q)
			BOOK_KEYTOSTRING_CASE(R)
			BOOK_KEYTOSTRING_CASE(S)
			BOOK_KEYTOSTRING_CASE(T)
			BOOK_KEYTOSTRING_CASE(U)
			BOOK_KEYTOSTRING_CASE(V)
			BOOK_KEYTOSTRING_CASE(W)
			BOOK_KEYTOSTRING_CASE(X)
			BOOK_KEYTOSTRING_CASE(Y)
			BOOK_KEYTOSTRING_CASE(Z)
			BOOK_KEYTOSTRING_CASE(Num0)
			BOOK_KEYTOSTRING_CASE(Num1)
			BOOK_KEYTOSTRING_CASE(Num2)
			BOOK_KEYTOSTRING_CASE(Num3)
			BOOK_KEYTOSTRING_CASE(Num4)
			BOOK_KEYTOSTRING_CASE(Num5)
			BOOK_KEYTOSTRING_CASE(Num6)
			BOOK_KEYTOSTRING_CASE(Num7)
			BOOK_KEYTOSTRING_CASE(Num8)
			BOOK_KEYTOSTRING_CASE(Num9)
			BOOK_KEYTOSTRING_CASE(Escape)
			BOOK_KEYTOSTRING_CASE(LControl)
			BOOK_KEYTOSTRING_CASE(LShift)
			BOOK_KEYTOSTRING_CASE(LAlt)
			BOOK_KEYTOSTRING_CASE(LSystem)
			BOOK_KEYTOSTRING_CASE(RControl)
			BOOK_KEYTOSTRING_CASE(RShift)
			BOOK_KEYTOSTRING_CASE(RAlt)
			BOOK_KEYTOSTRING_CASE(RSystem)
			BOOK_KEYTOSTRING_CASE(Menu)
			BOOK_KEYTOSTRING_CASE(LBracket)
			BOOK_KEYTOSTRING_CASE(RBracket)
			BOOK_KEYTOSTRING_CASE(SemiColon)
			BOOK_KEYTOSTRING_CASE(Comma)
			BOOK_KEYTOSTRING_CASE(Period)
			BOOK_KEYTOSTRING_CASE(Quote)
			BOOK_KEYTOSTRING_CASE(Slash)
			BOOK_KEYTOSTRING_CASE(BackSlash)
			BOOK_KEYTOSTRING_CASE(Tilde)
			BOOK_KEYTOSTRING_CASE(Equal)
			BOOK_KEYTOSTRING_CASE(Dash)
			BOOK_KEYTOSTRING_CASE(Space)
			BOOK_KEYTOSTRING_CASE(Return)
			BOOK_KEYTOSTRING_CASE(BackSpace)
			BOOK_KEYTOSTRING_CASE(Tab)
			BOOK_KEYTOSTRING_CASE(PageUp)
			BOOK_KEYTOSTRING_CASE(PageDown)
			BOOK_KEYTOSTRING_CASE(End)
			BOOK_KEYTOSTRING_CASE(Home)
			BOOK_KEYTOSTRING_CASE(Insert)
			BOOK_KEYTOSTRING_CASE(Delete)
			BOOK_KEYTOSTRING_CASE(Add)
			BOOK_KEYTOSTRING_CASE(Subtract)
			BOOK_KEYTOSTRING_CASE(Multiply)
			BOOK_KEYTOSTRING_CASE(Divide)
			BOOK_KEYTOSTRING_CASE(Left)
			BOOK_KEYTOSTRING_CASE(Right)
			BOOK_KEYTOSTRING_CASE(Up)
			BOOK_KEYTOSTRING_CASE(Down)
			BOOK_KEYTOSTRING_CASE(Numpad0)
			BOOK_KEYTOSTRING_CASE(Numpad1)
			BOOK_KEYTOSTRING_CASE(Numpad2)
			BOOK_KEYTOSTRING_CASE(Numpad3)
			BOOK_KEYTOSTRING_CASE(Numpad4)
			BOOK_KEYTOSTRING_CASE(Numpad5)
			BOOK_KEYTOSTRING_CASE(Numpad6)
			BOOK_KEYTOSTRING_CASE(Numpad7)
			BOOK_KEYTOSTRING_CASE(Numpad8)
			BOOK_KEYTOSTRING_CASE(Numpad9)
			BOOK_KEYTOSTRING_CASE(F1)
			BOOK_KEYTOSTRING_CASE(F2)
			BOOK_KEYTOSTRING_CASE(F3)
			BOOK_KEYTOSTRING_CASE(F4)
			BOOK_KEYTOSTRING_CASE(F5)
			BOOK_KEYTOSTRING_CASE(F6)
			BOOK_KEYTOSTRING_CASE(F7)
			BOOK_KEYTOSTRING_CASE(F8)
			BOOK_KEYTOSTRING_CASE(F9)
			BOOK_KEYTOSTRING_CASE(F10)
			BOOK_KEYTOSTRING_CASE(F11)
			BOOK_KEYTOSTRING_CASE(F12)
			BOOK_KEYTOSTRING_CASE(F13)
			BOOK_KEYTOSTRING_CASE(F14)
			BOOK_KEYTOSTRING_CASE(F15)
			BOOK_KEYTOSTRING_CASE(Pause)
	}

	return "";
}