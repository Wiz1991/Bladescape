#pragma once

#include <xyginext/core/Message.hpp>

namespace MessageID
{
	enum
	{
		Collision = xy::Message::Count, //IMPORTANT!! First ID must always start at xy::Message::Count
	};
}