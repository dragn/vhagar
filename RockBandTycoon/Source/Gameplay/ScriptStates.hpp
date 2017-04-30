#pragma once

#define _STATE(X) static const char* X = #X

namespace ScriptStates
{
_STATE(MOTG);
}

#undef _STATE
