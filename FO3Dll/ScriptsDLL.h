#pragma once
#include "PathFinder.h"

namespace Scripts {
	void EnableOneHex(int* shift);
	HANDLE EnablePathFinding(PathFinder* pathFinder, bool* terminate);
	void UnsetHooks();
};