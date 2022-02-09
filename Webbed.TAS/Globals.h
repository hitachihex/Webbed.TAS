#pragma once
#include "GMLObject.h"
#include "PlaybackManager.h"


#define C_WHITE                            0xFFFFFFFF
#define FONT_TEXT_NEW                                 0x01

//00D157E1         | 68 43870100                        | push 0x18743 | ; this value determines if it can be destroyed by a bullet.
#define BULLET_CAN_DESTROY_INDEX           0x18743


#define CAMERA_TRANSITION_ACTIVE_INDEX     0x189C0

#define ROOMALL_INSTANCE_ID                0xFFFFFFFD
#define GLOBAL_INSTANCE_ID                 0xFFFFFFFB

//VarName with id 00018973 and index 4 is last_axis_rv
#define RSTICK_VERTICAL_INDEX  0x18973

//VarName with id 00018972 and index 83 is last_axis_rh
#define RSTICK_HORIZONTAL_INDEX 0x18972

//VarName with id 00018971 and index 34 is last_axis_lv
#define LSTICK_VERTICAL_INDEX   0x18971 

//VarName with id 00018970 and index 114 is last_axis_lh
#define LSTICK_HORIZONTAL_INDEX 0x18970

#define MPI 3.14159265358979323846

extern bool g_bPaused;
extern bool g_bCursorLocked;
extern unsigned int g_GameSpeedPlayerStep;
extern bool g_bPressedFrameStepThisFrame;
extern GMLObject* g_pPlayerObject;
extern GMLObject* g_pCameraObject;
extern GMLObject* g_pTopBranch;
extern GMLObject* g_pBottomBranch;
extern GMLObject* g_pGameManager;
extern GMLObject* g_pIOObject;
extern GMLObject* g_pDebugSpawner;


extern void PressKey(unsigned int, unsigned char);
extern void HoldKey(unsigned int, unsigned char);
extern void ReleaseKey(unsigned int, unsigned char);
extern void SetDrawTextColor(unsigned long);
extern void SetDrawTextFont(unsigned long);
extern GMLObject* GetGlobalGameObject();
extern void DoDrawText(GMLObject*, double, double, const char*, unsigned long, unsigned long);

#pragma pack(push, 1)
class YYGStringObject
{
public:


	void Set(char* text) {
		this->m_pszText = text;
		this->m_Type = 0x01;
		this->m_nLength = strlen(text);
	}

	// 0x0000 - 0x0003
	char* m_pszText;

	// 0x0004 - 0x0007  (looks like always 1?)
	unsigned int m_Type;

	// 0x0008 - 0x000B
	unsigned int m_nLength;
protected:
private:
};

class YYGDrawTextUnkArgument
{
public:

	YYGDrawTextUnkArgument() {
		this->m_dwUnk0000_0003 = 0x0;
		this->m_dwUnk0004_0007 = 0x0;
		this->m_dwUnk0008_000B = 0x0;
		this->m_Type = 0x05;
	}
	// 0x0000 - 0x0003
	unsigned long m_dwUnk0000_0003;

	// 0x0004 - 0x0007
	unsigned long m_dwUnk0004_0007;

	// 0x0008 - 0x000B
	unsigned long m_dwUnk0008_000B;

	// 0x000C - 0x000F
	unsigned long m_Type;
protected:
private:

};

class YYGDrawTextArgumentsPacked
{
public:

	void Set(double posx, double posy, YYGStringObject* pStrObj)
	{
		this->m_xPosition = posx;
		this->m_yPosition = posy;
		this->m_ColorMaybe = 0x0;
		this->m_dwUnk000C_000F = 0x0;
		this->m_dwUnk0018_001B = 0x0;
		this->m_dwUnk001C_001F = 0x0;
		this->m_pStringObject = pStrObj;
		this->m_dwUnk0024_0027 = 0x0;
		this->m_dwUnk0028_002B = 0x0;
		this->m_nObjectType = 0x01;
	}
	// 0x0000 - 0x0007
	double m_xPosition;

	// 0x0008 - 0x000B
	unsigned long m_ColorMaybe;

	// 0x000C - 0x000F
	unsigned long m_dwUnk000C_000F;

	// 0x0010 - 0x0017
	double m_yPosition;

	// 0x0018 - 0x001B
	unsigned long m_dwUnk0018_001B;

	// 0x001C - 0x001F
	unsigned long m_dwUnk001C_001F;

	// 0x0020 - 0x0023
	YYGStringObject* m_pStringObject;

	// 0x0024 - 0x0027
	unsigned long m_dwUnk0024_0027;

	// 0x0028 - 0x002B
	unsigned long m_dwUnk0028_002B;

	// 0x002C - 0x002F
	unsigned int m_nObjectType;

protected:
private:
};


class YYRealAgument {
public:
	// all the way to C. we only care about what's at 0-7
	// 0x0000 - 0x0007
	double value;

	// 0x0008 - 0x000F
	unsigned char stuff[16];
protected:
private:
};


class PhysicsRoomInstance {

public:
	// 0x0000 - 0x0057
	unsigned char ucUnk0000_005F[0x58];

	// 0x0058 - 0x005B
	unsigned long m_dwUpdateSpeed;

	// 0x005C - 0x005F
	unsigned long m_dwIterations;

	// 0x0060 - 0x0063
	float m_fStepRate;
protected:
private:
};
class PhysicsSystem {
public:

	// 0x0000 - 0x00B3
	unsigned char ucUnk0000_00B3[0xB4];

	// 0x00B4 - 0x00B7
	PhysicsRoomInstance* m_pPhysRoom;
protected:
private:

};

class MemoryChunk
{
public:

	MemoryChunk() {
		memset(&this->m_Memory[0], 0x0, 0x1000);
	}
	unsigned char m_Memory[0x1000];
protected:
private:
};

class YYGS_RetValDouble
{
public:

	// 0x0000 - 0x0007
	double m_Value;

	// 0x0008 - 0x000B
	unsigned long pad8_b;

	// 0x000C - 0x000F
	unsigned long padc_f;
protected:
private:
};


#define PREFAB_LIST_PTR_ADDR 0x1667860


class ObjectPrefabList {
public:

	struct ObjectPrefab {
		// this is always 20 bytes - but if the object name is > 16 bytes the pointer is unionized i believe

	};

	struct ObjectPrefabMap {
	};

	static ObjectPrefabList* GetInstance() {
		return (ObjectPrefabList*)(*(unsigned long*)PREFAB_LIST_PTR_ADDR);
	}


	const char* getObjectName(unsigned long index) {

		/*0120D139         | 8B48 04                            | mov ecx,dword ptr ds:[eax+0x4]    ; ecx = m_nMax
          0120D13C         | 8B5424 04                          | mov edx,dword ptr ss:[esp+0x4]    ; edx = object_index
          0120D140         | 23CA                               | and ecx,edx                       ; ecx &= object_index;
          0120D142         | 8B00                               | mov eax,dword ptr ds:[eax]        ; eax = *(unsigned long*)(inst)
          0120D144         | 8B04C8                             | mov eax,dword ptr ds:[eax+ecx*8]  ; eax = *(unsigned long*)(eax+ecx*8)
		                                                           ^^ so, we are working with 64 bit index
		  */


		// and ecx, edx
		unsigned long new_index = (this->m_nMax & index);
		// mov eax, dword ptr ds:[eax]
		unsigned long obj = *(unsigned long*)(this);
		// mov eax, dword ptr ds:[eax + ecx * 8];
		obj = *(unsigned long*)(obj + new_index * 0x08);
		if (!obj) return nullptr;

		obj = *(unsigned long*)(obj + 0x0C);


		return (const char*)(*(unsigned long*)obj);

	}

	// 0x0000 - 0x0003
	unsigned long inst;

	// 0x0004 - 0x0007
	unsigned long m_nMax;
protected:
private:
};

#pragma pack(pop)

extern YYGDrawTextUnkArgument* g_pDrawTextUnk;
extern YYGDrawTextArgumentsPacked* g_pDrawTextArgs;
extern YYGStringObject* g_pStringObject;
extern PlaybackManager* g_pPlaybackMgr;
extern YYGS_RetValDouble* g_pOutDbl;

typedef void* (__cdecl* fn_varinst_get_names)(MemoryChunk*, unsigned long, unsigned long, unsigned long, YYGS_RetValDouble*);
extern fn_varinst_get_names varinst_get_names;
extern double g_mouse_x;
extern double g_mouse_y;