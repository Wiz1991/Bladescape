#pragma once

#include <xyginext/ecs/components/Drawable.hpp>
#include <xyginext/util/Const.hpp>

#include <cmath>

/*!
\brief Utility functions for creating drawable shapes.
Usage: pass in a drawable component attached to an entity
along with any extra parameters and the function will update
the component vertices to create the requested shape. This
is useful, for example, for drawing physics debug output
*/

namespace Shape
{
	/*!
	\brief Creates a circle shape around the origin point.
	\param radius Radius of the circle to create
	\param colour Colour to set the created circle
	\param pointCount Number of points to use to create the circle. Setting this
	to a lower number such as 5 or 6 can be useful for creating regulat polygons
	like hexagons or pentagons.
	*/
	static inline void setCircle(xy::Drawable& drawable, float radius, sf::Color colour = sf::Color::Green, int pointCount = 10)
	{
		auto& verts = drawable.getVertices();
		verts.clear();

		float step = xy::Util::Const::TAU / pointCount;
		for (auto i = 0; i < pointCount; ++i)
		{
			sf::Vector2f p(std::sin(i * step), std::cos(i * step));
			p *= radius;
			verts.emplace_back(p, colour);
		}
		verts.push_back(verts.front());
		verts.emplace_back(sf::Vector2f(), colour);

		drawable.setPrimitiveType(sf::LineStrip);
		drawable.updateLocalBounds();
	}

	/*!
	\brief Creates a rectangle shape.
	\param size Size of the rectangle to create
	\param colour Colour to set the rectangle
	*/
	static inline void setRectangle(xy::Drawable& drawable, sf::Vector2f size, sf::Color colour = sf::Color::Green)
	{
		auto& verts = drawable.getVertices();
		verts.clear();

		verts.emplace_back(sf::Vector2f(), colour);
		verts.emplace_back(sf::Vector2f(size.x, 0.f), colour);
		verts.emplace_back(size, colour);
		verts.emplace_back(sf::Vector2f(0.f, size.y), colour);
		verts.push_back(verts.front());

		drawable.setPrimitiveType(sf::LineStrip);
		drawable.updateLocalBounds();
	}

	/*!
	\brief Creates a chain of segments from the given list of points.
	\param points std::vector of sf::Vector2f used as points to create the line
	\param colour Colour to use when drawing the line
	*/
	static inline void setPolyLine(xy::Drawable& drawable, const std::vector<sf::Vector2f>& points, sf::Color colour = sf::Color::Green)
	{
		auto& verts = drawable.getVertices();
		verts.clear();
		for (auto p : points)
		{
			verts.emplace_back(p, colour);
		}
		drawable.setPrimitiveType(sf::LineStrip);
		drawable.updateLocalBounds();
	}
}