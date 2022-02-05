#pragma once

#include <easyhook.h>

extern NTSTATUS AddHook(void*, void*, void*, TRACED_HOOK_HANDLE);
extern NTSTATUS MakeHookExclusive(unsigned long*, unsigned long, TRACED_HOOK_HANDLE);