#pragma once
#include <string>

namespace StringUtils {
	template <typename T>
	std::string toString(const T& value)
	{
		std::stringstream stream;
		stream << value;
		return stream.str();
	}
	std::string vectorToString(sf::Vector2f vector) {
		std::stringstream stream;
		stream << vector.x;
		std::string x = stream.str();
		stream << vector.y;
		std::string y = stream.str();
		return x + " , " + y;
	}
}