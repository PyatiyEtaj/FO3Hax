#pragma once

#include "framework.hpp"

namespace Types {
	using PCritterCl = unsigned char*;
	using PFOClient = unsigned char*;
	using PHexManager = unsigned char*;
}

namespace FO3 {
	class Mouse {
	public:
		int* X = (int*)Addresses::MouseX;
		int* Y = (int*)Addresses::MouseY;
	};

	enum class MouseType
	{
		ContextMenu = 0,
		InvCursor = 6,
		Move = 1,
		Attack = 3,
	};
}