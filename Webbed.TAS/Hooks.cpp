#include "pch.h"

#define _USE_MATH_DEFINES 1

#include "Hooks.h"
#include "Globals.h"
#include "GMLEvent.h"
#include "Webbed.TAS.h"
#include "Callbacks.h"
#include "PlaybackManager.h"
#include <cmath>

#define MPI 3.14159265358979323846

#pragma warning(disable : 4996)
oEventLoop original_EventLoop = (oEventLoop)(WEBBED_EVENTLOOP_ADDR);
oYoyoUpdate original_YoyoUpdate = (oYoyoUpdate)(WEBBED_YOYOUPDATE_ADDR);
odraw_text original_draw_text = (odraw_text)(WEBBED_DRAWTEXT_ADDR);
oWindowMouseGetX original_window_mouse_get_x = (oWindowMouseGetX)(WEBBED_WINDOWMOUSEGETX_ADDR);
oWindowMouseGetY original_window_mouse_get_y = (oWindowMouseGetY)(WEBBED_WINDOWMOUSEGETY_ADDR);
oGMLCallFunction original_GMLCallFunction = (oGMLCallFunction)(WEBBED_GMLCALLFUNCTION_ADDR);	


void __cdecl YoYoUpdate_Hook()
{
	static bool bOnce = false;

	// increase g_WaveTimeCounter
	if (!bOnce)
	{
		DebugOutput("YoyoUpdate_Hook, !bOnce");
		bOnce = true;
	}

	g_bPressedFrameStepThisFrame = false;

	if (GetAsyncKeyState(VK_F1) & 1 && !g_bPaused)
	{

		g_bPaused = true;
		DebugOutput("Paused.");

		GameSetSpeed(60.0);
		return;
	}

	if (GetAsyncKeyState(VK_F2) & 1) {
		g_bCursorLocked = !g_bCursorLocked;
	}

	// Start & Stop playback
	if (GetAsyncKeyState(VK_F3) & 1)
	{
		if (!g_pPlaybackMgr->IsPlayingBack()) {
			g_pPlaybackMgr->plugForJournal = false;
		}

		g_pTopBranch = nullptr;
		g_pBottomBranch = nullptr;
		g_pPlaybackMgr->InitPlayback(true);
	}


	// Set gamespeed to normal.
	if (GetAsyncKeyState(VK_DIVIDE) & 1)
	{
		SetDrawEnabled(true);
		g_GameSpeedPlayerStep = 1;
		//GameSetSpeed(60.0);
	}

	// Increase game speed.
	if (GetAsyncKeyState(VK_ADD) & 1)
	{

		// TODO: Make configurable hotkey crap

		if ((g_GameSpeedPlayerStep + 1) > 200)
			g_GameSpeedPlayerStep = 200;
		else {
			g_GameSpeedPlayerStep++;
			//GameSetSpeed(60.0 * g_GameSpeedPlayerStep);
		}

		//DebugOutput("Game speed is now: %d", g_GameSpeedPlayerStep);
	}

	// Decrease game speed.
	if (GetAsyncKeyState(VK_SUBTRACT) & 1)
	{
		// can't go to 0
		if (!(g_GameSpeedPlayerStep - 1))
			g_GameSpeedPlayerStep = 1;
		else {
			g_GameSpeedPlayerStep--;
			//GameSetSpeed(60.0 * g_GameSpeedPlayerStep);
		}

		//DebugOutput("Game speed is now: %d", g_GameSpeedPlayerStep);
	}


	if (GetAsyncKeyState(0x68) & 1)
	{
		//g_pStringObject->Set((char*)"Save.dat");
		//game_save(g_pStringObject);
	}

	if (GetAsyncKeyState(0x69) & 1) {
		//g_pStringObject->Set((char*)"Save.dat");
		//game_load("Save.dat");
	}


	if (!g_bPaused)
	{

		if (g_GameSpeedPlayerStep > 1)
		{

			
			for (unsigned int i = 0; i < g_GameSpeedPlayerStep; i++)
			{
				original_YoyoUpdate();
			}

			
			if (g_pPlaybackMgr)
			{
				g_pPlaybackMgr->FormatManagerString();
			}
			return;
		}
	}

	if (g_bPaused)
	{
		if (GetAsyncKeyState(VK_OEM_6) & 1)
		{
			g_bPressedFrameStepThisFrame = true;
			original_YoyoUpdate();

			
			if (g_pPlaybackMgr)
			{
				g_pPlaybackMgr->FormatManagerString();
			}

			return;
		}

		if (GetAsyncKeyState(VK_OEM_5) & 1)
		{
			g_bPaused = false;
			DebugOutput("Unpausing.");
			goto fuckitleaveYoYoUpdate;
		}

		// just leave
		return;
	}

fuckitleaveYoYoUpdate:
	original_YoyoUpdate();

	
	if (g_pPlaybackMgr)
	{
		g_pPlaybackMgr->FormatManagerString();
	}
}

bool __cdecl EventLoop_Hook(void* arg1, void* arg2, void* arg3, void* arg4, void* arg5)
{

	static bool bOnce = false;

	if (!bOnce)
	{
		bOnce = true;
		DebugOutput("EventLoop_Hook, !bOnce");

	}
	GameMaker_Event* pEvent = (GameMaker_Event*)(arg3);
	GMLObjectSubData* pObjectSubData = pEvent->m_pSubData;
	const char* pcszObjectEventName = pEvent->m_pSubData->m_pszObjectEventName;

	if (pEvent->m_ObjectEventType == EObjectEventType::OET_UNKNOWN_00)
	{
		// no idea
	}

	else if (pEvent->m_ObjectEventType <= EObjectEventType::OET_STEP1)
	{
		
		auto it = g_CallbackMap.find(pcszObjectEventName);

		if (it != g_CallbackMap.end())
		{
			auto mapValue = it->second;
			g_pPrevEvent = g_pCurrentEvent;
			g_pCurrentEvent = mapValue;

			if (!mapValue->m_bHooked)
			{
				mapValue->m_dwOriginal = pEvent->m_pSubData->m_dwAddressToCall;
				mapValue->m_bHooked = true;

				unsigned long dwOldProt;
				VirtualProtect(&pEvent->m_pSubData->m_dwAddressToCall, 0x04, PAGE_EXECUTE_READWRITE, &dwOldProt);
				pEvent->m_pSubData->m_dwAddressToCall = mapValue->m_dwCallback;
			}
		}
		else
		{

			/* Generics are broken, figure out why
			bool hookedGeneric = false;
			for (size_t i = 0; i < g_GenericObjectEventNames.size(); i++)
			{
				if (g_GenericObjectEventNames[i] == pcszObjectEventName)
				{
					hookedGeneric = true;
				}
			}

			if (!hookedGeneric)
			{
				g_GenericObjectEventNames.push_back(std::string(pcszObjectEventName));
				EventCallbackHook* pEventCallbackHook = new EventCallbackHook();
				t_EventCallbackHook::Init(pEventCallbackHook, std::string(pcszObjectEventName), (unsigned long)(&GMLObject_Generic_Callback), pEvent->m_pSubData->m_dwAddressToCall);

				g_CallbackMap[std::string(pcszObjectEventName)] = pEventCallbackHook;

				pEventCallbackHook->m_dwOriginal = pEvent->m_pSubData->m_dwAddressToCall;
				pEventCallbackHook->m_bHooked = true;

				unsigned long dwOldProt;
				VirtualProtect(&pEvent->m_pSubData->m_dwAddressToCall, 0x04, PAGE_EXECUTE_READWRITE, &dwOldProt);
				pEvent->m_pSubData->m_dwAddressToCall = pEventCallbackHook->m_dwCallback;

			}*/
		}
	}
	else if (pEvent->m_ObjectEventType == EObjectEventType::OET_UNKNOWN_03)
	{
		// no idea
	}

	bool retval = original_EventLoop(arg1, arg2, arg3, arg4, arg5);
	return retval;
}

float __stdcall gamepad_get_axis_value_hook(unsigned int axisIndex)
{
	/*
	// can we just return 0,0 unless we're doing something?
	unsigned int deviceIndex = 0;

	__asm
	{
		mov deviceIndex, ecx;
	}
	*/


	static bool bOnce = false;

	if (!bOnce)
	{
		DebugOutput("gamepad_get_axis_value_hook, !bOnce");
		bOnce = true;
	}


	if (g_pPlaybackMgr)
	{
		if (g_pPlaybackMgr->IsPlayingBack())
		{
			auto pCurrentInput = g_pPlaybackMgr->GetCurrentInput();
			if (pCurrentInput)
			{
				double x = 0.0;
				double y = 0.0;

				// checking this bool is faster than constantly calling cos and sin on 0.0 every frame
				if (pCurrentInput->hasAngle)
				{
					double rads = (pCurrentInput->angle * (M_PI / 180.0));
					x = sin(rads);
					y = -cos(rads);
				}

				if (axisIndex == 0) return x;
				else if (axisIndex == 1) return y;
			}
		}
	}
	return 0.0;
}

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080
constexpr double center_x = WINDOW_WIDTH / 2;
constexpr double center_y = WINDOW_HEIGHT / 2;
void __cdecl window_mouse_get_y_Hook(YYRealAgument* arg) {
	original_window_mouse_get_y(arg);
	if (g_pPlaybackMgr) {
		if (g_pPlaybackMgr->IsPlayingBack()) {
			auto curInput = g_pPlaybackMgr->GetCurrentInput();

			/*
			if (curInput->hasAngle) {

				//   this.XAxis = (float)Math.Sin(temp * Math.PI / 180.0);
				  //this.YAxis = (float)Math.Cos(temp * Math.PI / 180.0);
				//double x = cos(curInput->angle * (3.14 / 180.0));
				//double y = sin(curInput->angle * (3.14 / 180.0));
				double x = 250.0;
				double y = 500.0;
				g_pPlaybackMgr->lastMouseXInput = x;
				g_pPlaybackMgr->lastMouseYInput = y;
				arg->value = y;
			}*/

			if (curInput->hasMouse) {
				arg->value = curInput->mouseY;

			}
			else {
				// keep it where it was last
				arg->value = 540;
			}

		}
		else if (g_bCursorLocked) {
			arg->value = 540;
		}
	}
}

void __cdecl window_mouse_get_x_Hook(YYRealAgument* arg) {

	original_window_mouse_get_x(arg);
	if (g_pPlaybackMgr) {
		if (g_pPlaybackMgr->IsPlayingBack()) {
			auto curInput = g_pPlaybackMgr->GetCurrentInput();

			if (curInput->hasMouse) {
				arg->value = curInput->mouseX;
			}
			else {
				// keep it where it was last
				arg->value = 960;
			}


		}
		else if (g_bCursorLocked) {
			arg->value = 960;
		}
	}

}

void __cdecl GMLCallFunction_Hook(void* arg1, void* arg2, void* arg3, void* arg4, unsigned long index, void* arg6) {
	unsigned long new_index = (index * 0x4C);
	new_index += *(unsigned long*)(WEBBED_GMFUNTABLE_PTR_ADDR);

	char* we_callin = (char*)(new_index);


	if (g_pPlaybackMgr) {

		if (g_pPlaybackMgr->IsPlayingBack()) {
			char* p = strstr(we_callin, "draw_");


			if (p) {
				if (g_pPlaybackMgr->m_bRuntoActive)
				{

					// literally just patch the bytes instead, kek
					//0x0118E42F
					*(unsigned char*)(0x0118E42F) = 0x90;
					*(unsigned char*)(0x0118E430) = 0x90;

				}
				else {
					*(unsigned char*)(0x0118E42F) = 0xFF;
					*(unsigned char*)(0x0118E430) = 0xD1;
				}
			}
		}
		// don't call the draw shit while runto is active. just leave
	
	}

	original_GMLCallFunction(arg1, arg2, arg3, arg4, index, arg6);
}
