#pragma once

#include "Addresses.h"
#include "Globals.h"
#include <list>

typedef bool(__cdecl* oEventLoop)(void*, void*, void*, void*, void*);
typedef void(__cdecl* oYoyoUpdate)();
typedef void(__cdecl* oWindowMouseGetX)(YYRealAgument*);
typedef void(__cdecl* oWindowMouseGetY)(YYRealAgument*);
typedef void(__cdecl* oGMLCallFunction)(void*, void*, void*, void*, unsigned long, void*);
typedef bool(__cdecl* ocheck_key_held)(unsigned char);
typedef void(__cdecl* ooPhysCreate0)(GMLObject*, void*);
// hmm, i think the other was number of arguments
typedef void(__cdecl* odraw_text)(YYGDrawTextUnkArgument*, GMLObject*, GMLObject*, unsigned int, YYGDrawTextArgumentsPacked*);
extern oEventLoop original_EventLoop;
extern oYoyoUpdate original_YoyoUpdate;
extern oWindowMouseGetX original_window_mouse_get_x;
extern oWindowMouseGetY original_window_mouse_get_y;
extern oGMLCallFunction original_GMLCallFunction;
extern ocheck_key_held original_check_key_held;
extern ooPhysCreate0 original_phys_create0;
extern void __cdecl YoYoUpdate_Hook();
extern bool __cdecl EventLoop_Hook(void*, void*, void*, void*, void*);
extern void __cdecl window_mouse_get_x_Hook(YYRealAgument*);
extern void __cdecl window_mouse_get_y_Hook(YYRealAgument*);
extern void __cdecl GMLCallFunction_Hook(void*, void*, void*, void*, unsigned long, void*);
extern bool __cdecl check_key_held_Hook(unsigned char);
extern void __cdecl GMLObject_oPhysCreate0_Hook(GMLObject*, void*);

extern odraw_text original_draw_text;
