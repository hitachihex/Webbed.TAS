#include "pch.h"
#include "Callbacks.h"
#include "GMLEvent.h"
#include "Globals.h"
#include "GMLRoom.h"
#include "Hooks.h"
#include "PlaybackManager.h"

#pragma optimize("", off)
void __cdecl GMLObject_Generic_Callback(GMLObject* pGMLObject, void* arg2)
{

	GameMaker_Event* pGMLObjectEvent = nullptr;
	__asm
	{
		mov pGMLObjectEvent, edi;
	}

	std::string objEventName = std::string(pGMLObjectEvent->m_pSubData->m_pszObjectEventName);

	auto cbEvent = g_CallbackMap[objEventName];

	if (cbEvent == nullptr)
		DebugOutput("we're going to crash!");

	((void(__cdecl*)(void*, void*))cbEvent->m_dwOriginal)(pGMLObject, arg2);

}
#pragma optimize("", on)

void __cdecl GMLObject_oPlayer_Step0_Callback(GMLObject* pObject, void* arg2)
{
	static bool bOnce = false;

	if (!bOnce)
	{
		bOnce = true;
		DebugOutput("oPlayer_Step0_Callback: pObject = %p", pObject);
	}

	g_pPlayerObject = pObject;
	//pObject->set_visible(false);

	gmobj_call_orig(pObject, arg2, g_pEventCallback_PlayerStep0);

}

void __cdecl GMLObject_oInput_Step2_Callback(GMLObject* pObject, void* arg2) {
	static bool bOnce = false;

	if (!bOnce)
	{
		bOnce = true;
		DebugOutput("oInput_Step2_Callback: pObject = %p", pObject);
	}

	gmobj_call_orig(pObject, arg2, g_pEventCallback_InputStep2);
}

void __cdecl GMLObject_oIO_Step1_Callback(GMLObject* pObject, void* arg2) {
	static bool bOnce = false;

	if (!bOnce)
	{
		bOnce = true;
		DebugOutput("oIO_Step1_Callback: pObject = %p", pObject);
	}


	if (g_pPlaybackMgr)
	{
		if (g_pPlaybackMgr->IsPlayingBack())
		{
			g_pPlaybackMgr->DoPlayback(g_bPressedFrameStepThisFrame);
		}
	}

	gmobj_call_orig(pObject, arg2, g_pEventCallback_oIOStep1);
}

void __cdecl GMLObject_oCamera_Step1_Callback(GMLObject* pObject, void* arg2) {
	static bool bOnce = false;

	if (!bOnce)
	{
		bOnce = true;
		DebugOutput("oCamera_Step1_Callback: pObject = %p", pObject);
	}

	g_pCameraObject = pObject;

	gmobj_call_orig(pObject, arg2, g_pEventCallback_oCameraStep1);
}

void __cdecl GMLObject_oGameManager_Draw0_Callback(GMLObject* pObject, void* arg2) {
	static bool bOnce = false;

	if (!bOnce)
	{
		bOnce = true;
		DebugOutput("oGameManager_Draw0_Callback: pObject = %p", pObject);
	}


	if (g_pCameraObject) {
		//DebugOutput("Trying to draw SPOODER at %f, %f, white, and font 1", g_pCameraObject->m_fX - 900.0, g_pCameraObject->m_fY - 500.0);
	}
	gmobj_call_orig(pObject, arg2, g_pEventCallback_oGameManagerDraw0);
}

void __cdecl GMLObject_oGameManager_Draw75_Callback(GMLObject* pObject, void* arg2) {
	static bool bOnce = false;

	if (!bOnce)
	{
		bOnce = true;
		DebugOutput("oGameManager_Draw75_Callback: pObject = %p", pObject);
	}

		/*
		std::string fmt = "\nPos: ";
		fmt += std::to_string(g_pPlayerObject->m_fX) + ", " + std::to_string(g_pPlayerObject->m_fY);
		fmt += "\nSpeed: ";
		fmt += std::to_string(*g_pPlayerObject->getXSpeedPtr()) + ", " + std::to_string(g_pPlayerObject->m_phys_speed_y) + "\n";
		DoDrawText(pObject, 200.0, 200.0, fmt.c_str(), C_WHITE, FONT_TEXT_NEW);
		//DebugOutput("Trying to draw SPOODER at %f, %f, white, and font 1", g_pCameraObject->m_fX - 900.0, g_pCameraObject->m_fY - 500.0);*/
	std::string fmt = g_pPlaybackMgr->m_szCurrentManagerState;
	DoDrawText(pObject, 200.0, 200.0, fmt.c_str(), C_WHITE, FONT_TEXT_NEW);

	gmobj_call_orig(pObject, arg2, g_pEventCallback_oGameManagerDraw75);
}

void __cdecl GMLObject_oInstructionPrompt_Draw75_Callback(GMLObject* pObject, void* arg2) {

	static bool bOnce = false;

	if (!bOnce)
	{
		bOnce = true;
		DebugOutput("oInstructionPrompt_Draw75_Callback: pObject = %p", pObject);
	}


	if (g_pCameraObject) {
		//DoDrawText(pObject, 200.0, 200.0, "S-P-O-O-D-E-R", C_WHITE, FONT_TEXT_NEW);
		//DebugOutput("Trying to draw SPOODER at %f, %f, white, and font 1", g_pCameraObject->m_fX - 900.0, g_pCameraObject->m_fY - 500.0);
	}
	gmobj_call_orig(pObject, arg2, g_pEventCallback_oInstructionPromptDraw75);
}

void __cdecl GMLObject_oPlayer_Alarm2_Callback(GMLObject* pObject, void* arg2) {
	static bool bOnce = false;

	if (!bOnce)
	{
		bOnce = true;
		DebugOutput("oPlayer_Alarm2_Callback: pObject = %p", pObject);
	}


	if (g_pPlaybackMgr) {
		if (g_pPlaybackMgr->IsPlayingBack()) {

			if (g_pPlaybackMgr->lastInputWasEsc) {
				g_pPlaybackMgr->lastInputWasEsc = false;
				// unblock the alarm
				return gmobj_call_orig(pObject, arg2, g_pEventCallback_oPlayerAlarm2);
			}
			else {
				// block the alarm - don't want alt tabbing to pause us.
				return;
			}

		}
	}
	return gmobj_call_orig(pObject, arg2, g_pEventCallback_oPlayerAlarm2);
}

void __cdecl GMLObject_oFragileBranch_Create0_Callback(GMLObject* pObject, void* arg2) {
	static bool bOnce = false;

	if (!bOnce)
	{
		bOnce = true;
		DebugOutput("oFragileBranch_Create0_Callback: pObject = %p", pObject);
	}

	if (pObject->id_1 == 0x19612) {
		g_pBottomBranch = pObject;
		if (g_pBottomBranch->m_pPhysProp) {
			//g_pBottomBranch->m_pPhysProp->set_phy_active(false);
		}

	}
	else if (pObject->id_1 == 0x19613) {
		g_pTopBranch = pObject;
	}

	return gmobj_call_orig(pObject, arg2, g_pEventCallback_oFragileBranchCreate0);
}


void __cdecl GMLObject_oRoomTransition_Create0_Callback(GMLObject* pObject, void* arg2) {
	g_pBottomBranch = nullptr;
	g_pTopBranch = nullptr;
	return gmobj_call_orig(pObject, arg2, g_pEventCallback_oRoomTransitionCreate0);
}

void __cdecl GMLObject_oPlayer_Cleanup0_Callback(GMLObject* pObject, void* arg2) {
	static bool bOnce = false;

	if (!bOnce)
	{
		bOnce = true;
		DebugOutput("oPlayer_Cleanup0_Callback: pObject = %p", pObject);
	}

	// invalidate player object when cleanup happens. it will be set in step0 again.
	g_pPlayerObject = nullptr;
	return gmobj_call_orig(pObject, arg2, g_pEventCallback_oPlayerCleanup0);
}

// 0x19612 bottom branch
// 0x19613 top branch
//0062E7C3         | C746 40 B5AD4201                   | mov dword ptr ds:[esi+0x40],webbed.142ADB5                              | [esi+40]:"gml_Object_oInstructionPrompt_Draw_75", 142ADB5:"gml_Script_draw_button_prompt"
void SetupEventCallbacks() {

	t_EventCallbackHook::Init(g_pEventCallback_PlayerStep0, "gml_Object_oPlayer_Step_0", (unsigned long)(&GMLObject_oPlayer_Step0_Callback), 0xDEADBEEF);

	//0090A743         | C746 68 F0DA4301                   | mov dword ptr ds:[esi+0x68],webbed.143DAF0:"gml_Object_oCamera_Step_1"
	t_EventCallbackHook::Init(g_pEventCallback_oCameraStep1, "gml_Object_oCamera_Step_1", (unsigned long)(&GMLObject_oCamera_Step1_Callback), 0xDEADBEEF);

	//00E54633         | C746 38 196E4401                   | mov dword ptr ds:[esi+0x38],webbed.1446E19:"gml_Object_oInput_Step_2"
	t_EventCallbackHook::Init(g_pEventCallback_InputStep2, "gml_Object_oInput_Step_2", (unsigned long)(&GMLObject_oInput_Step2_Callback), 0xDEADBEEF);

	//00949EF3         | C746 60 90DE4301                   | mov dword ptr ds:[esi+0x60],webbed.143DE90                              | [esi+60]:"gml_Object_oIO_Step_1", 143DE90:"gml_Object_oIO_Step_1"
	t_EventCallbackHook::Init(g_pEventCallback_oIOStep1, "gml_Object_oIO_Step_1", (unsigned long)(&GMLObject_oIO_Step1_Callback), 0xDEADBEEF);

	//008FB19E         | C746 38 B8D94301                   | mov dword ptr ds:[esi+0x38],webbed.143D9B8                              | 143D9B8:"gml_Object_oGameManager_Draw_0"
	t_EventCallbackHook::Init(g_pEventCallback_oGameManagerDraw0, "gml_Object_oGameManager_Draw_0", (unsigned long)(&GMLObject_oGameManager_Draw0_Callback), 0xDEADBEEF);

	//008FD8F3         | C746 58 F7D94301                   | mov dword ptr ds:[esi+0x58],webbed.143D9F7                              | 143D9F7:"gml_Object_oGameManager_Draw_75"
	t_EventCallbackHook::Init(g_pEventCallback_oGameManagerDraw75, "gml_Object_oGameManager_Draw_75", (unsigned long)(&GMLObject_oGameManager_Draw75_Callback), 0xDEADBEEF);

	//008C8753         | C746 40 8CD34301                   | mov dword ptr ds:[esi+0x40],webbed.143D38C                              | [esi+40]:"gml_Object_oInstructionPrompt_Draw_75", 143D38C:"gml_Object_oInstructionPrompt_Draw_75"
	t_EventCallbackHook::Init(g_pEventCallback_oInstructionPromptDraw75, "gml_Object_oInstructionPrompt_Draw_75", (unsigned long)(&GMLObject_oInstructionPrompt_Draw75_Callback), 0xDEADBEEF);

	//00A5EF1D         | C746 20 F6F54301                   | mov dword ptr ds:[esi+0x20],webbed.143F5F6                              | esi+20: "gml_Object_oPlayer_Alarm_2"
	t_EventCallbackHook::Init(g_pEventCallback_oPlayerAlarm2, "gml_Object_oPlayer_Alarm_2", (unsigned long)(&GMLObject_oPlayer_Alarm2_Callback), 0xDEADBEEF);

	t_EventCallbackHook::Init(g_pEventCallback_oFragileBranchCreate0, "gml_Object_oFragileBranch_Create_0", (unsigned long)(&GMLObject_oFragileBranch_Create0_Callback), 0xDEADBEEF);

	t_EventCallbackHook::Init(g_pEventCallback_oRoomTransitionCreate0, "gml_Object_oRoomTransition_Create_0", (unsigned long)(&GMLObject_oRoomTransition_Create0_Callback), 0xDEADBEEF);

	t_EventCallbackHook::Init(g_pEventCallback_oPlayerCleanup0, "gml_Object_oPlayer_CleanUp_0", (unsigned long)(&GMLObject_oPlayer_Cleanup0_Callback), 0xDEADBEEF);

	g_CallbackMap[g_pEventCallback_PlayerStep0->m_ObjectEventName] = g_pEventCallback_PlayerStep0;

	g_CallbackMap[g_pEventCallback_oCameraStep1->m_ObjectEventName] = g_pEventCallback_oCameraStep1;

	g_CallbackMap[g_pEventCallback_InputStep2->m_ObjectEventName] = g_pEventCallback_InputStep2;

	g_CallbackMap[g_pEventCallback_oIOStep1->m_ObjectEventName] = g_pEventCallback_oIOStep1;

	g_CallbackMap[g_pEventCallback_oGameManagerDraw0->m_ObjectEventName] = g_pEventCallback_oGameManagerDraw0;

	g_CallbackMap[g_pEventCallback_oGameManagerDraw75->m_ObjectEventName] = g_pEventCallback_oGameManagerDraw75;

	g_CallbackMap[g_pEventCallback_oInstructionPromptDraw75->m_ObjectEventName] = g_pEventCallback_oInstructionPromptDraw75;

	g_CallbackMap[g_pEventCallback_oPlayerAlarm2->m_ObjectEventName] = g_pEventCallback_oPlayerAlarm2;

	g_CallbackMap[g_pEventCallback_oFragileBranchCreate0->m_ObjectEventName] = g_pEventCallback_oFragileBranchCreate0;

	g_CallbackMap[g_pEventCallback_oRoomTransitionCreate0->m_ObjectEventName] = g_pEventCallback_oRoomTransitionCreate0;

	g_CallbackMap[g_pEventCallback_oPlayerCleanup0->m_ObjectEventName] = g_pEventCallback_oPlayerCleanup0;
}