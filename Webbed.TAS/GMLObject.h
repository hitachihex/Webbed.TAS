#pragma once

#include "GMLRoom.h"

#pragma pack(push, 1)

typedef struct t_GMLObjectInstance
{

} GMLObjectInstance;


// this is..pointless? lol
typedef struct t_GMLObject : GMLRoomObjectInstance
{

	//[20596] oMainStep0 player.base.unk=015BE8BC


	// because we're dumb

	/*
	// 0x04 - 0x07
	GMLObjectInstance * m_pObjectInstance;

	// 0x08 - 0x0B
	unsigned long m_dwBadFood00;

	// 0x0C - 0x0F
	unsigned long m_dwBadFood01;

	// 0x10 - 0x13
	unsigned long m_dwUnknown01;

	// 0x14 - 0x17
	unsigned long m_dwUknown02;

	// 0x18 - 0x1B
	unsigned long m_dwUnknown03;

	// 0x1C - 0x1F
	unsigned long m_dwUnknown04;

	// 0x20 - 0x23
	unsigned long m_dwWeirdlyBehavingCounter;

	// 0x24 - 0x27
	unsigned long m_dwUnknown05;

	// 0x28 - 0x2B
	unsigned long m_dwUnknown06;

	// 0x2C
	bool m_bDrawEnabled;

	// 0x2D
	bool m_bDeactivated;

	// 0x2E - 0x3F
	unsigned char m_ucUnknown2E_3F[0x40 - 0x2E];

	// 0x40 - 0x43
	unsigned long m_dwInstanceID;*/

} GMLObject;

#pragma pack(pop)
