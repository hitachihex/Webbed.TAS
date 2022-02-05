#include "pch.h"
#include "Addresses.h"
#include "Webbed.TAS.h"
#include "Hooks.h"
#include "Callbacks.h"
#include "PlaybackManager.h"

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "EasyHook32.lib")

fn_game_save game_save = (fn_game_save)(WEBBED_GAMESAVE_ADDR);
extern "C" void __declspec(dllexport) __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO * inRemoteInfo);
std::map<std::string, EventCallbackHook*> g_CallbackMap;
std::vector<std::string> g_GenericObjectEventNames;

unsigned long g_ACLEntries[1] = { NULL };

HOOK_TRACE_INFO EventLoopHookHandle = { NULL };
HOOK_TRACE_INFO YoYoUpdateHookHandle = { NULL };
HOOK_TRACE_INFO WindowMouseGetXHookHandle = { NULL };
HOOK_TRACE_INFO WindowMouseGetYHookHandle = { NULL };
HOOK_TRACE_INFO GMLCallFunctionHookHandle = { NULL };

EventCallbackHook* g_pEventCallback_PlayerStep0 = new EventCallbackHook();
EventCallbackHook* g_pEventCallback_InputStep2 = new EventCallbackHook();
EventCallbackHook* g_pEventCallback_oCameraStep1 = new EventCallbackHook();
EventCallbackHook* g_pEventCallback_oIOStep1 = new EventCallbackHook();
EventCallbackHook* g_pEventCallback_oGameManagerDraw0 = new EventCallbackHook();
EventCallbackHook* g_pEventCallback_oGameManagerDraw75 = new EventCallbackHook();
EventCallbackHook* g_pEventCallback_oInstructionPromptDraw75 = new EventCallbackHook();
EventCallbackHook* g_pEventCallback_oPlayerAlarm2 = new EventCallbackHook();
EventCallbackHook* g_pEventCallback_oFragileBranchCreate0 = new EventCallbackHook();
EventCallbackHook* g_pEventCallback_oRoomTransitionCreate0 = new EventCallbackHook();
EventCallbackHook* g_pEventCallback_oPlayerCleanup0 = new EventCallbackHook();
EventCallbackHook* g_pCurrentEvent = nullptr;
EventCallbackHook* g_pPrevEvent = nullptr;
void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo)
{
	unsigned long dwOldProt = 0;
	NTSTATUS result = AddHook((void*)WEBBED_YOYOUPDATE_ADDR, YoYoUpdate_Hook, NULL, &YoYoUpdateHookHandle);

	if (FAILED(result))
	{
		std::wstring err(RtlGetLastErrorString());
		DebugOutputW(err.c_str());
	}
	else
	{
		DebugOutput("YoyoUpdate hook installed.");
		PatchDeltaTime();
		DebugOutput("Delta time spoof patch set.");
		ExclusiveHook(&YoYoUpdateHookHandle);
	}

	result = AddHook((void*)WEBBED_EVENTLOOP_ADDR, EventLoop_Hook, NULL, &EventLoopHookHandle);

	if (FAILED(result))
	{
		std::wstring err(RtlGetLastErrorString());
		DebugOutputW(err.c_str());
	}
	else
	{
		SetupEventCallbacks();
		DebugOutput("EventLoop hook installed.");
		ExclusiveHook(&EventLoopHookHandle);
	}


	result = AddHook((void*)WEBBED_WINDOWMOUSEGETX_ADDR, window_mouse_get_x_Hook, NULL, &WindowMouseGetXHookHandle);

	if (FAILED(result))
	{
		std::wstring err(RtlGetLastErrorString());
		DebugOutputW(err.c_str());
	}
	else
	{
		DebugOutput("WindowMouseGetX hook installed.");
		ExclusiveHook(&WindowMouseGetXHookHandle);
	}


	result = AddHook((void*)WEBBED_WINDOWMOUSEGETY_ADDR, window_mouse_get_y_Hook, NULL, &WindowMouseGetYHookHandle);

	if (FAILED(result))
	{
		std::wstring err(RtlGetLastErrorString());
		DebugOutputW(err.c_str());
	}
	else
	{
		DebugOutput("WindowMouseGetY hook installed.");
		ExclusiveHook(&WindowMouseGetYHookHandle);
	}

	/*
	result = AddHook((void*)WEBBED_GMLCALLFUNCTION_ADDR, GMLCallFunction_Hook, NULL, &GMLCallFunctionHookHandle);

	if (FAILED(result))
	{
		std::wstring err(RtlGetLastErrorString());
		DebugOutputW(err.c_str());
	}
	else
	{
		DebugOutput("GMLCallFunction hook installed.");
		ExclusiveHook(&GMLCallFunctionHookHandle);
	}*/

	unsigned long temp = 0x0118E42F;
	
	VirtualProtect((LPVOID)temp, 4, PAGE_EXECUTE_READWRITE, &dwOldProt);

	g_pPlaybackMgr = new PlaybackManager("Webbed.rec");

	/*
	result = AddHook((void*)SHEEPO_GETGAMEPAD_AXIS_VALUE_ADDRESS, gamepad_get_axis_value_hook, NULL, &GamePadGetAxisValueHookHandle);

	if (FAILED(result))
	{
		std::wstring err(RtlGetLastErrorString());
		DebugOutputW(err.c_str());
	}
	else
	{
		DebugOutput("GamePadGetAxisValueHook installed.");
		ExclusiveHook(&GamePadGetAxisValueHookHandle);
	}*/

	/*
	unsigned long current_time_table_addr = 0xBE0 + 0xDD91F4;
	unsigned long dwEax = *(unsigned long*)(current_time_table_addr);
	VirtualProtect((LPVOID)dwEax, 0x4, PAGE_EXECUTE_READWRITE, &dwOldProt);
	*(unsigned long*)(current_time_table_addr) = (unsigned long)current_time_hook;*/


	//g_pPlaybackMgr = new PlaybackManager("Sheepo.rec");

	//DumpPointersForExternalOSD();
}

void SetDrawEnabled(bool enabled) {

	//011C663E         | A2 BF996601                        | mov byte ptr ds:[0x16699BF],al                                          |
	*(bool*)(WEBBED_DRAWENABLED_PTR_ADDR) = enabled;
}

void PatchDeltaTime() {
	//0121F023
	unsigned long patch_addr = 0x0121F023;
	unsigned long dwOldProt;
	VirtualProtect((LPVOID)patch_addr, 0x10, PAGE_EXECUTE_READWRITE, &dwOldProt);

	//0121F023         | 8935 08996601                      | mov dword ptr ds:[0x1669908],esi                                        |
	*(unsigned char*)(patch_addr + 0) = 0x90;
	*(unsigned char*)(patch_addr + 1) = 0x90;
	*(unsigned char*)(patch_addr + 2) = 0x90;
	*(unsigned char*)(patch_addr + 3) = 0x90;
	*(unsigned char*)(patch_addr + 4) = 0x90;
	*(unsigned char*)(patch_addr + 5) = 0x90;

	*(unsigned long*)(0x1669908) = 02;


}

void UnpatchDeltaTime() {

	unsigned long patch_addr = 0x0121F023;
	*(unsigned char*)(patch_addr + 0) = 0x89;
	*(unsigned char*)(patch_addr + 1) = 0x35;
	*(unsigned char*)(patch_addr + 2) = 0x08;
	*(unsigned char*)(patch_addr + 3) = 0x99;
	*(unsigned char*)(patch_addr + 4) = 0x66;
	*(unsigned char*)(patch_addr + 5) = 0x01;

	// set it once for consistenc
	*(unsigned long*)(0x1669908) = 16667;
}

void SetDelta(unsigned long newDelta) {
	//1000000
	*(unsigned long*)(0x1669908) = (1 / newDelta) * 1000000;
}

unsigned long GetCurrentRoomID() {
	return *(unsigned long*)(WEBBED_CURRENTROOMID_ADDR);
}

PhysicsSystem* GetPhysicsSystem() {
	return (PhysicsSystem*)(*(unsigned long*)WEBBED_PHYSICS_SYSTEM_PTR_ADDR);
}

void UpdatePhysicsSpeed() {
	PhysicsSystem* pSystem = GetPhysicsSystem();
	if (pSystem) {
		if (pSystem->m_pPhysRoom) {
			pSystem->m_pPhysRoom->m_dwUpdateSpeed = 60 * g_GameSpeedPlayerStep;
		}
	}
}

void GameSetSpeed(double FPS) {
	*(double*)(WEBBED_GAMESPEED_ADDR) = FPS;
}

void SetIterations(unsigned long nIterations) {
	auto pPhys = GetPhysicsSystem();

	if (pPhys) {
		if (pPhys->m_pPhysRoom) {
			pPhys->m_pPhysRoom->m_dwIterations = nIterations;
		}
	}
}