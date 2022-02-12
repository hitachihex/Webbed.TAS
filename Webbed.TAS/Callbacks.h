#pragma once


#include "Webbed.TAS.h"
#include "GMLObject.h"
#define gmobj_call_orig(a,b,c)  ((void(__cdecl *)(void *, void*))c->m_dwOriginal)(a,b)

extern void __cdecl GMLObject_Generic_Callback(GMLObject*, void*);
extern void __cdecl GMLObject_oPlayer_Step0_Callback(GMLObject*, void*);
extern void __cdecl GMLObject_oInput_Step2_Callback(GMLObject*, void*);
extern void __cdecl GMLObject_oCamera_Step1_Callback(GMLObject*, void*);
extern void __cdecl GMLObject_oIO_Step1_Callback(GMLObject*, void*);
extern void __cdecl GMLObject_oGameManager_Draw0_Callback(GMLObject*, void*);
extern void __cdecl GMLObject_oGameManager_Draw75_Callback(GMLObject*, void*);
extern void __cdecl GMLObject_oInstructionPrompt_Draw75_Callback(GMLObject*, void*);
// it's easier to just block the object script execute that handles pausing than it is to find more shit.
//00A5EF1D | C746 20 F6F54301 | mov dword ptr ds : [esi + 0x20] , webbed.143F5F6 | 143F5F6 : "gml_Object_oPlayer_Alarm_2"
extern void __cdecl GMLObject_oPlayer_Alarm2_Callback(GMLObject*, void*);

//00D0ED23         | C746 30 FA464401                   | mov dword ptr ds:[esi+0x30],webbed.14446FA                              | 14446FA:"gml_Object_oFragileBranch_Create_0"
extern void __cdecl GMLObject_oFragileBranch_Create0_Callback(GMLObject*, void*);

//009753E3         | C746 38 F7E04301                   | mov dword ptr ds:[esi+0x38],webbed.143E0F7                              | 143E0F7:"gml_Object_oRoomTransition_Create_0"
extern void __cdecl GMLObject_oRoomTransition_Create0_Callback(GMLObject*, void*);


//00B0AE33         | C746 30 97FA4301                   | mov dword ptr ds:[esi+0x30],webbed.143FA97                              | 143FA97:"gml_Object_oPlayer_CleanUp_0"
extern void __cdecl GMLObject_oPlayer_Cleanup0_Callback(GMLObject*, void*);

//00A57613         | C746 1C A4F54301                   | mov dword ptr ds:[esi+0x1C],webbed.143F5A4                              | 143F5A4:"gml_Object_oPlayer_Create_0"
extern void __cdecl GMLObject_oPlayer_Create0_Callback(GMLObject*, void*);

//00E78313         | C746 40 D0714401                   | mov dword ptr ds:[esi+0x40],webbed.14471D0                              | 14471D0:"gml_Object_oDebugSpawner_Step_0"
extern void __cdecl GMLObject_oDebugSpawnwer_Step0_Callback(GMLObject*, void*);

//0096FFEE         | C746 10 43DF4301                   | mov dword ptr ds:[esi+0x10],webbed.143DF43                              | 143DF43:"gml_Object_oPhys_Create_0"
extern void __cdecl GMLObject_oPhys_Create0_Callback(GMLObject*, void*);
extern void SetupEventCallbacks();