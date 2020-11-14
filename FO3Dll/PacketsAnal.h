#pragma once
#include "framework.h"
#include "HookSetter.h"
#include "utils.h"

namespace PacketsAnal {	
	void SetInlineHook(HaxSettings* haxSettings);
	void Unset();
}
