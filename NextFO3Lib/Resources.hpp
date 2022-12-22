#pragma once
#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include "GlobalState.hpp"
#include "resource.h"

extern GlobalState::GlobalObjects GObjects;

namespace Resources 
{
	void Notification()
	{
		PlaySoundA(MAKEINTRESOURCEA(IDR_SOUND1), GObjects.Dll, SND_RESOURCE | SND_ASYNC);
	}
}