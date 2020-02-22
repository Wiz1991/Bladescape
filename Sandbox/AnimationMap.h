#pragma once
#pragma once

#include <array>

namespace AnimID
{
	namespace Player
	{
		enum
		{
			Idle, Jump, Run, Die,

			Count
		};
	}
}

template <std::size_t size>
using AnimationMap = std::array<std::size_t, size>;