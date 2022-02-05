#pragma once
#include <string>
#include <map>
#include <vector>
#include <random>
#include <time.h>
#include "GMLObject.h"
#include "GMLEvent.h"
#include "Globals.h"
#include <easyhook.h>
#include "EasyHookUtils.h"
#include "DebugTools.h"

#pragma warning(disable: 4996)

#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#define ExclusiveHook(b) MakeHookExclusive(g_ACLEntries, 1, b)

typedef void(__cdecl* fn_game_save)(YYGStringObject*);
extern fn_game_save game_save;
extern unsigned long g_ACLEntries[1];
extern HOOK_TRACE_INFO EventLoopHookHandle;
extern HOOK_TRACE_INFO YoYoUpdateHookHandle;
extern HOOK_TRACE_INFO WindowMouseGetXHookHandle;
extern HOOK_TRACE_INFO WindowMouseGetYHookHandle;
extern HOOK_TRACE_INFO GMLCallFunctionHookHandle;


extern std::map<std::string, EventCallbackHook*> g_CallbackMap;
extern std::vector<std::string> g_GenericObjectEventNames;


extern EventCallbackHook* g_pEventCallback_PlayerStep0;
extern EventCallbackHook* g_pEventCallback_InputStep2;
extern EventCallbackHook* g_pEventCallback_oCameraStep1;
extern EventCallbackHook* g_pEventCallback_oIOStep1;
extern EventCallbackHook* g_pEventCallback_oGameManagerDraw0;
extern EventCallbackHook* g_pEventCallback_oGameManagerDraw75;
extern EventCallbackHook* g_pEventCallback_oInstructionPromptDraw75;
extern EventCallbackHook* g_pEventCallback_oPlayerAlarm2;
extern EventCallbackHook* g_pEventCallback_oFragileBranchCreate0;
extern EventCallbackHook* g_pEventCallback_oRoomTransitionCreate0;
extern EventCallbackHook* g_pEventCallback_oPlayerCleanup0;

extern EventCallbackHook* g_pCurrentEvent;
extern EventCallbackHook* g_pPrevEvent;

extern void SetDrawEnabled(bool);
extern void PatchDeltaTime();
extern unsigned long GetCurrentRoomID();


extern PhysicsSystem* GetPhysicsSystem();
extern void UpdatePhysicsSpeed();
extern void GameSetSpeed(double FPS);
extern void SetDelta(unsigned long);
extern void SetIterations(unsigned long);