#include "pch.h"
#include "Addresses.h"
#include "Webbed.TAS.h"
#include "Hooks.h"
#include "Callbacks.h"
#include "PlaybackManager.h"

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "EasyHook32.lib")

bool ugly_hack = false;
fn_game_save game_save = (fn_game_save)(WEBBED_GAMESAVE_ADDR);
fn_room_restart room_restart = (fn_room_restart)(WEBBED_ROOM_RESTART_ADDR);
fn_instance_create_depth instance_create_depth = (fn_instance_create_depth)(WEBBED_CREATE_INSTANCE_ADDR);

extern "C" void __declspec(dllexport) __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO * inRemoteInfo);
std::map<std::string, EventCallbackHook*> g_CallbackMap;
std::vector<std::string> g_GenericObjectEventNames;

unsigned long g_ACLEntries[1] = { NULL };

HOOK_TRACE_INFO EventLoopHookHandle = { NULL };
HOOK_TRACE_INFO YoYoUpdateHookHandle = { NULL };
HOOK_TRACE_INFO WindowMouseGetXHookHandle = { NULL };
HOOK_TRACE_INFO WindowMouseGetYHookHandle = { NULL };
HOOK_TRACE_INFO GMLCallFunctionHookHandle = { NULL };
HOOK_TRACE_INFO CheckKeyHeldHookHandle = { NULL };

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
EventCallbackHook* g_pEventCallback_oPlayerCreate0 = new EventCallbackHook();
EventCallbackHook* g_pEventCallback_oDebugSpawnerStep0 = new EventCallbackHook();
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
	result = AddHook((void*)WEBBED_CHECK_KEY_HELD_ADDR, check_key_held_Hook, NULL, &CheckKeyHeldHookHandle);

	if (FAILED(result))
	{
		std::wstring err(RtlGetLastErrorString());
		DebugOutputW(err.c_str());
	}
	else
	{
		DebugOutput("check_key_held hook installed.");
		ExclusiveHook(&CheckKeyHeldHookHandle);
	}*/

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

	*(unsigned long*)(0x1669908) = 16667;


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

class CVariableNameMap {
public:

	// 0x0000 - 0x0003
	char* m_VarNames;
protected:
private:
};

const char* get_varname_from_id(unsigned long id) {
	CVariableNameMap* pVariableNames = (CVariableNameMap*)(*(unsigned long*)(WEBBED_VARIABLE_NAMES_LIST_ADDR));
	unsigned long var_hashmap_ptr = *(unsigned long*)(WEBBED_VARIABLE_NAMES_LIST_ADDR);
	unsigned long index_as_instance_id = (id - 0x186A0);
	if (index_as_instance_id < 0 || index_as_instance_id > 0x19000)  return nullptr;

	return (const char*)(pVariableNames->m_VarNames[index_as_instance_id]);
	//unsigned long dwEax = *(unsigned long*)(var_hashmap_ptr + index_as_instance_id * 4);
	//return (const char*)(dwEax);
}
//#pragma optimize("", off)
void dump_variable_names(GMLObject* pObject, const char * filename) {

	FILE* fp = fopen(filename, "w");
		unsigned long var_count = pObject->m_pInstProps->m_nVariableCount;
		unsigned long var_name_list_ptr = *(unsigned long*)(WEBBED_VARIABLE_NAMES_LIST_ADDR);

		for (unsigned long i = 0; i <= 0x1FF; i++) {
			unsigned long var_id = pObject->m_pInstProps->m_pInternal->variables[i].m_Id;
			//const char* var_name = get_varname_from_id(var_id);
			//if (!var_name) continue;
			
			if (var_id == 0 || var_id > 0x19000) continue;
			unsigned long index_as_instance_id = (var_id - 0x186A0);
			if (index_as_instance_id < 0 || index_as_instance_id > 0x19000) continue;
			//011D991A         | A1 C8D75C01                        | mov eax,dword ptr ds:[0x15CD7C8]                                        |
			unsigned long dwEax = *(unsigned long*)(var_name_list_ptr + index_as_instance_id * 4);
			const char* var_name = (const char*)(dwEax);
			fprintf(fp, "VarName with id %08X and index %d is %s\n", var_id, i, var_name);
		}

		fclose(fp);
}

constexpr unsigned int MATRIX_VIEW       = 0;
constexpr unsigned int MATRIX_PROJECTION = 1;
void dump_mouse_world_pos() {
	unsigned int matrix_view_index = MATRIX_VIEW * 64; // shl ecx, 0x06
	unsigned int matrix_projection_index = MATRIX_PROJECTION * 64; // shl ecx, 0x06

	matrix_view_index += WEBBED_MATRIX_PTR_ADDR;
	matrix_projection_index += WEBBED_MATRIX_PTR_ADDR;

	float* v = (float*)(matrix_view_index);
	float* p = (float*)(matrix_projection_index);

	// in case you don't have the camera position for some reason
    float cam_x = -v[12] * v[0] - v[13] * v[1] - v[14] * v[2];
    float cam_y = -v[12] * v[4] - v[13] * v[5] - v[14] * v[6];
    float cam_z = -v[12] * v[8] - v[13] * v[9] - v[14] * v[10];

	const float port_w = 1920.0;
	const float port_h = 1080.0;

	//012CAF90         | A1 506B8801                        | mov eax,dword ptr ds:[0x1886B50]    window_mouse_x
	//012CAFA0         | A1 786B8801                        | mov eax,dword ptr ds:[0x1886B78]    window_mouse_y
	unsigned long mouse_x = *(unsigned long*)(0x1886B50);
	unsigned long mouse_y = *(unsigned long*)(0x1886B78);
	// webbeds projection is orthographic.
	float mx = (2 * mouse_x / port_w - 1) / p[0];
	float my = (1 - 2 * mouse_y / port_h) / p[5];

	float t = -(cam_z + v[8] * mx + v[9] * my) / v[10];
	float wx = cam_x + v[0] * mx + v[1] * my + v[2] * t;
	float wy = cam_y + v[4] * mx + v[5] * my + v[6] * t;

	DebugOutput("g_mouse_x = %d, g_mouse_y = %d", mouse_x, mouse_y);
	DebugOutput("mouse_world_x = %f, mouse_world_y = %f", mx, my);

}
//#pragma optimize("", on)