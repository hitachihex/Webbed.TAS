#pragma once

#include <list>

#define FLG_BBOX_SOMETHING 1 << 3
#define FLG_VISIBLE        1 << 4
#define FLG_SOLID          1 << 5



#define PHYS_FLG_ACTIVE    1 << 5

#pragma pack(push, 1)
struct GMLObjectBindingData
{
public:

	// 0x0000 - 0x0003
	char* m_pszObjectName;
protected:
private:
};


/*typedef struct t_GMLRoomObjectInstance
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

	// 0xCC = hspeed
	// 0xD0 = vspeed

} GMLRoomObjectInstance;*/


struct PhysicsPropertiesInstance {

public:
	// 0x0000 - 0x0003
	unsigned long m_dwUnk0000_0003;

	// 0x0004 
	unsigned char m_PhysFlag;

	// 0x0005
	bool m_bUnk0005;

	// 0x0006
	bool m_bUnk006;

	// 0x0007
	bool m_bUnk0007;

	// 0x0008 - 0x000B
	unsigned long m_dwUnk0008_000B;

	// 0x000C - 0x000F
	float m_fPhysX;

	// 0x0010 - 0x0013
	float m_fPhysY;
protected:
private:

};
struct PhysicsProperties {

public:

	void set_phy_active(bool active) {
		if (this->physInst) {
			if (active)
				this->physInst->m_PhysFlag |= PHYS_FLG_ACTIVE;
			else
				this->physInst->m_PhysFlag &= ~(PHYS_FLG_ACTIVE);
		}
	}
	// 0x0000 - 0x0003
	PhysicsPropertiesInstance* physInst;
protected:
private:
};


// just make GMLObject inherit from this, but add the vtable.
typedef struct t_GMLRoomObjectInstance
{



	// 0x00 - 0x03 - vtable
	// vtable[0]
	virtual void vfunc_00() = 0;

	// vtable[1]
	virtual void* get_variable_ptr(unsigned int index) = 0;


	// useful methods
	double* get_dbl_ptr(unsigned int index) {
		return (double*)this->get_variable_ptr(index);
	}


	double* getXSpeedPtr() {
		return get_dbl_ptr(0x1898E);
	}

	double* getYSpeedPtr() {
		return get_dbl_ptr(0x1898F);
	}

	void SetPos(float x, float y)
	{
		this->m_fX = x;
		this->m_fY = y;
	}

	void set_visible(bool b) {
		if (b)
			this->flags |= FLG_VISIBLE;
		else
			this->flags &= ~(FLG_VISIBLE);
	}

	// useful methods


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

	// 0x0030 - 0x004F
	unsigned char m_ucUnk0030_0077[0x50 - 0x30];

	// 0x0050 - 0x0053
	float m_phys_speed_x;

	// 0x0054 - 0x0057
	float m_phys_speed_y;

	// 0x0058 - 0x0067
	unsigned char m_ucUnk0058_0067[0x68 - 0x58];

	// 0x0068 - 0x006B
	GMLObjectBindingData* m_pBindData;

	// 0x006C - 0x006F
	unsigned long m_dwUnk006C_006F;

	// 0x0070 - 0x0073
	unsigned long m_dwUnk0070_0073;

	// 0x0074 - 0x0077
	PhysicsProperties* m_pPhysProp;

	// 0x0078 - 0x007B
	unsigned long m_InstanceID;

	// 0x007C - 0x007F
	unsigned long m_dwUnk007C_007F;

	// 0x0080 - 0x0083
	unsigned long flags;

	// 0x0084 - 0x0087
	unsigned long id_1;

	// 0x0088 - 0x00AF
	unsigned char m_ucUnk0080_009F[0xB0 - 0x88];

	// 0x00B0 - 0x00B3
	unsigned long m_dwUnk00B0_00B3;

	// 0x00B4 - 0x00B7
	unsigned long id_2;
	
	// 0x00B8 - 0x00BB
	float m_fX;

	// 0x00BC - 0x00BF
	float m_fY;

	// 0x00C0 - 0x00E3
	unsigned char m_ucUnk00A8_00CB[0xE4 - 0xC0];

	// 0x00E4 - 0x00E7
	float m_fHSpeed;

	// 0x00E8 - 0x00EB
	float m_fVSpeed;

	// 0x00D4 - 0x12F
	//unsigned char m_ucUnk00D4_0012F[0x130 - 0xD4];

	// 0xE4 - alarm (11*4 array)

	// 0x130 - 0x133
	//t_GMLRoomObjectInstance* m_pNext;


	// 0x15C = depth
	// 0xCC = hspeed
	// 0xD0 = vspeed

} GMLRoomObjectInstance;
typedef struct t_YoYoList
{
	// 0x00 - 0x03
	unsigned long m_dwUnknown00_03;

	// 0x04 - 0x12F
	unsigned char m_ucUnknown00_013B[0x130 - 0x04];

	// 0x130 - 0x133
	GMLRoomObjectInstance* m_pObjectLList;

} YoYoList;

class GMLCamera
{
public:

	// 0x0000 - 0x017F
	unsigned char m_ucUnknown0000_0017F[0x180];

	// 0x0180 - 0x0183
	float m_ViewX;

	// 0x0184 - 0x0187
	float m_ViewY;
protected:
private:

};

class GMLCameraData
{
public:

	// 0x0000 - 0x003F
	unsigned char m_ucUnknown[0x40];

	// 0x0040 - 0x0043
	unsigned long m_CameraViewID;
protected:
private:

};

struct GMLRoom
{
public:

	// 0x00 - 0x03
	unsigned long m_dwUnknown00_03;

	// 0x04 - 0x07
	unsigned long m_dwUnknown04_07;

	// 0x08 - 0x0B
	unsigned long m_dwUnk08_0B;

	// 0x0C - 0x0F
	unsigned long m_dwRoomSpeed;

	// 0x10 - 0x13
	unsigned long m_dwRoomWidth;

	// 0x14 - 0x17
	unsigned long m_dwRoomHeight;

	// 0x18 - 0x1B
	unsigned long m_dwUnk018_01B;

	// 0x1C - 0x1F
	unsigned long m_dwUnk01C_01F;

	/*
	// 0x20 - 0x7F
	unsigned char m_cUnk[0x80 - 0x20];*/


	// 0x0020 - 0x0047
	unsigned char m_ucUnk[0x48 - 0x20];

	// 0x0048 - 0x0063
	GMLCameraData* m_pListCamData[0x7];

	// 0x0064 - 0x0067
	unsigned long m_dwUnk0064_0067;

	// 0x0068 - 0x006B
	unsigned long m_dwUnk0068_006B;

	// 0x006C - 0x006F
	unsigned long m_dwUnk006C_006F;

	// 0x0070 - 0x0073
	unsigned long m_dwUnk0070_0073;

	// 0x0074 - 0x0077
	unsigned long m_dwUnk0074_0077;

	// 0x0078 - 0x007B
	unsigned long m_dwUnk0078_007B;

	// 0x007C - 0x007F
	unsigned long m_dwUnk007C_007F;

	// 0x80 - 0x83
	YoYoList* m_pYoyoList;

	// 0x84 - 0x87
	unsigned long m_dwUnknown84_87;

	// 0x88 - 0x8B
	// Instance list count
	unsigned long m_ObjListCount;

protected:
private:


};

#pragma pack(pop)