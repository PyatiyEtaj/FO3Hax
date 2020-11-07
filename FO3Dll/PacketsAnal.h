#pragma once
#include "framework.h"
#include "HookSetter.h"
#include "utils.h"


namespace PacketsAnal {	
	void SetInlineHook(bool* input, bool* output);
	void Unset();
}
