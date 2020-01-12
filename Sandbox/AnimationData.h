#pragma once

#include <array>

namespace AnimID
{
	namespace Player
	{
		//MUST BE IN THE SAME ORDER AS IN THE .SPT FILE
		enum
		{
			Run,
			Idle,
			Hit,

			Count
		};
	}
}

template <std::size_t size>
using AnimationMap = std::array<std::size_t, size>;