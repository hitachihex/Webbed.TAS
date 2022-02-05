#pragma once

#include "GMLObject.h"
#include <string>
#pragma pack(push, 1)

typedef enum t_EObjectEventType
{
	OET_UNKNOWN_00 = 0x00,
	OET_STEP = 0x01,
	OET_STEP1 = 0x02,
	OET_UNKNOWN_03 = 0x03
} EObjectEventType;


typedef struct t_GMLObjectSubData
{
	// 0x00 - 0x03
	char* m_pszObjectEventName;

	// 0x04 - 0x07
	unsigned long m_dwAddressToCall;
} GMLObjectSubData;

class GameMaker_Event
{
public:

	// 0x0000 - 0x0003
	unsigned long m_dwUnk0000_0003;

	// 0x0004 - 0x0007
	unsigned long m_dwUnk0004_0007;

	// 0x0008 - 0x000B
	// StepType, I believe it was
	EObjectEventType m_ObjectEventType;

	// 0x000C - 0x004F
	unsigned char uc_Unk0000_004F[0x50 - 0x0C];

	// 0x0050 - 0x0053
	unsigned long m_dwCodePtr;

	// 0x0054 - 0x0057
	unsigned long m_dwUnk0054_0057;

	// 0x0058 - 0x005B
	unsigned long m_dwUnk0058_005B;

	// 0x005C - 0x005F
	char* m_pszScriptName;

	// 0x0060 - 0x0063
	unsigned long m_dwUnk0060_0063;

	// 0x0064 - 0x0067
	GMLObjectSubData* m_pSubData;


protected:
private:
};

/*

struct GMLObjectBindingData
{
public:

	// 0x0000 - 0x0003
	char * m_pszObjectName;
protected:
private:
};

typedef struct t_GMLRoomObjectInstance
{

	// 0x00 - 0x03
	unsigned long m_dwUnknown00_03;

	// 0x04 - 0x2B
	unsigned char m_ucUnknown04_2B[0x2C - 0x04];

	// 0x2C
	bool m_bDoesDraw;

	// 0x2D
	bool m_bDeactivated;

	// 0x2E
	bool m_bUnk002E;

	// 0x2F
	bool m_bUnk002F;

	// 0x0030 - 0x0067
	unsigned char m_ucUnk0030_0077[0x68 - 0x30];

	// 0x0068 - 0x006B
	GMLObjectBindingData * m_pBindData;

	// 0x006C - 0x006F
	unsigned long m_dwUnk006C_006F;

	// 0x0070 - 0x0073
	unsigned long m_dwUnk0070_0073;

	// 0x0074 - 0x0077
	unsigned long m_dwUnk0074_0077;

	// 0x0078 - 0x007B
	unsigned long m_InstanceID;

	// 0x007C - 0x007F
	unsigned long m_dwUnk007C_007F;

	// 0x0080 - 0x009F
	unsigned char m_ucUnk0080_009F[0xA0 - 0x80];

	// 0x00A0 - 0x00A3
	float m_fX;

	// 0x00A4 - 0x00A7
	float m_fY;

	// 0x00A8 - 0x012F
	unsigned char m_ucUnk00A8_012F[0x130 - 0xA8];

	// 0x130 - 0x133
	t_GMLRoomObjectInstance * m_pNext;

} GMLRoomObjectInstance;
typedef struct t_YoYoList
{
	// 0x00 - 0x03
	unsigned long m_dwUnknown00_03;

	// 0x04 - 0x12F
	unsigned char m_ucUnknown00_013B[0x130 - 0x04];

	// 0x130 - 0x133
	GMLRoomObjectInstance * m_pObjectLList;

} YoYoList;

struct GMLRoom
{

	// 0x00 - 0x03
	unsigned long m_dwUnknown00_03;

	// 0x04 - 0x7F
	unsigned char m_ucUnknown04_7F[0x80 - 0x04];

	// 0x80 - 0x83
	YoYoList * m_pYoyoList;

	// 0x84 - 0x87
	unsigned long m_dwUnknown84_87;

	// 0x88 - 0x8B
	// Instance list count
	unsigned long m_ObjListCount;

};
*/

typedef struct t_EventCallbackHook
{
	static void Init(t_EventCallbackHook* _this, std::string name, unsigned long callback, unsigned long original)
	{
		_this->m_ObjectEventName = name;

		_this->m_dwCallback = callback;

		_this->m_dwOriginal = original;

		_this->m_bHooked = false;

		_this->m_pObjectInstance = nullptr;

		_this->m_pBaseObject = nullptr;
	}

	// Object events name.
	std::string m_ObjectEventName;

	// Our callback
	unsigned long m_dwCallback;

	// Original
	unsigned long m_dwOriginal;

	// Have we been hooked?
	bool m_bHooked;

	// Need our object instance set here, as a global pointer?
	GMLObjectInstance* m_pObjectInstance;

	// base object pointer, so we know when to step in other menus or not.
	GMLObject* m_pBaseObject;


	// Was the framestep hotkey pressed in the Event Perform hook?
	// Deprecated
	bool m_bShouldStepOnce;
} EventCallbackHook;


#pragma pack(pop)