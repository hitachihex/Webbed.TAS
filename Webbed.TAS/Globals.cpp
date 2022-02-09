
#include "pch.h"
#include "Globals.h"
#include "Addresses.h"
#include "Hooks.h"

bool g_bPaused = false;
bool g_bCursorLocked = false;
unsigned int g_GameSpeedPlayerStep = 1;
bool g_bPressedFrameStepThisFrame = false;
GMLObject* g_pPlayerObject = nullptr;
GMLObject* g_pGameManager = nullptr;
GMLObject* g_pCameraObject = nullptr;
PlaybackManager* g_pPlaybackMgr = nullptr;
GMLObject* g_pTopBranch = nullptr;
GMLObject* g_pBottomBranch = nullptr;
GMLObject* g_pIOObject = nullptr;
GMLObject* g_pDebugSpawner = nullptr;
double g_mouse_x = 0.0;
double g_mouse_y = 0.0;

YYGDrawTextArgumentsPacked* g_pDrawTextArgs = new YYGDrawTextArgumentsPacked();
YYGStringObject* g_pStringObject = new YYGStringObject();
YYGDrawTextUnkArgument* g_pDrawTextUnk = new YYGDrawTextUnkArgument();
YYGS_RetValDouble* g_pOutDbl = new YYGS_RetValDouble();

fn_varinst_get_names varinst_get_names = (fn_varinst_get_names)(WEBBED_VARINST_GETNAMES_ADDRESS);


__declspec(noinline) void PressKey(unsigned int k, unsigned char v)
{
	unsigned char* ptr = (unsigned char*)(k + WEBBED_KEYPRESS_PTR_ADDR);
	*ptr = v;
}

__declspec(noinline) void HoldKey(unsigned int k, unsigned char v)
{
	unsigned char* ptr = (unsigned char*)(k + WEBBED_KEYHELD_PTR_ADDR);
	*ptr = v;
}

__declspec(noinline) void ReleaseKey(unsigned int k, unsigned char v)
{
	unsigned char* ptr = (unsigned char*)(k + WEBBED_KEYRELEASE_PTR_ADDR);
	*ptr = v;
}

GMLObject* GetGlobalGameObject()
{
	return (GMLObject*)(*(unsigned long*)(WEBBED_GLOBALGAMEOBJECT_ADDR));
}

void SetDrawTextColor(unsigned long dwColor)
{
	*(unsigned long*)(WEBBED_FONTCOLOR_PTR_ADDR) = dwColor;
}

void SetDrawTextFont(unsigned long dwFont)
{
	*(unsigned long*)(WEBBED_FONT_PTR_ADDR) = dwFont;
}

void DoDrawText(GMLObject* pObject, double x, double y, const char* pszText, unsigned long dwColor, unsigned long dwFont)
{
	SetDrawTextColor(dwColor);
	SetDrawTextFont(dwFont);

	
	g_pStringObject->Set((char*)pszText);
	g_pDrawTextArgs->Set(x, y, g_pStringObject);

	unsigned long gmft = *(unsigned long*)(WEBBED_GMFUNTABLE_PTR_ADDR);
	gmft = gmft + WEBBED_GMFUNTABLE_DRAWTEXT_OFFSET;

	//0118E41F         | 8B35 88646501                      | mov esi,dword ptr ds:[0x1656488]                                        | esi:"minkernel\\ntdll\\ldrinit.c"
	*(unsigned long*)(0x1656488) = gmft;
	g_pDrawTextUnk->m_Type = 0x05;
	original_draw_text(g_pDrawTextUnk, pObject, pObject, 0x03, g_pDrawTextArgs);
	*(unsigned long*)(0x1656488) = 0x0;

}