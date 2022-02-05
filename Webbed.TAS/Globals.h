#pragma once
#include "GMLObject.h"
#include "PlaybackManager.h"


#define C_WHITE                            0xFFFFFFFF
#define FONT_TEXT_NEW                                 0x01

//00D157E1         | 68 43870100                        | push 0x18743 | ; this value determines if it can be destroyed by a bullet.
#define BULLET_CAN_DESTROY_INDEX           0x18743

extern bool g_bPaused;
extern bool g_bCursorLocked;
extern unsigned int g_GameSpeedPlayerStep;
extern bool g_bPressedFrameStepThisFrame;
extern GMLObject* g_pPlayerObject;
extern GMLObject* g_pCameraObject;
extern GMLObject* g_pTopBranch;
extern GMLObject* g_pBottomBranch;


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
#pragma pack(pop)

extern YYGDrawTextUnkArgument* g_pDrawTextUnk;
extern YYGDrawTextArgumentsPacked* g_pDrawTextArgs;
extern YYGStringObject* g_pStringObject;
extern PlaybackManager* g_pPlaybackMgr;