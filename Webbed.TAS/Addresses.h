#pragma once

//0116E5B0 < webbed | 803D EDD45C01 00 | cmp byte ptr ds : [0x15CD4ED] , 0x0 | ; EventLoop for webbed
#define WEBBED_EVENTLOOP_ADDR 0x0116E5B0

//0121F000 <webbed | 83EC 08                            | sub esp,0x8                                                             | ; YoYoUpdate for webbed
#define WEBBED_YOYOUPDATE_ADDR 0x0121F000

//004067CC         | FF35 28D96501                      | push dword ptr ds:[<maybe_gpGlobalVarList>]      
#define WEBBED_GLOBALGAMEOBJECT_ADDR 0x0165D928

//012B7ECB         | 8A80 685B8801                      | mov al,byte ptr ds:[eax+0x1885B68]   
#define WEBBED_KEYPRESS_PTR_ADDR 0x01885B68

//012B7E7B         | 8A80 68598801                      | mov al,byte ptr ds:[eax+0x1885968] 
#define WEBBED_KEYHELD_PTR_ADDR 0x1885968

//012B7EEB         | 8A80 685A8801                      | mov al,byte ptr ds:[eax+0x1885A68]      
#define WEBBED_KEYRELEASE_PTR_ADDR 0x01885A68

//011D4694         | 890D 34D65C01                      | mov dword ptr ds:[0x15CD634],ecx                                        |
#define WEBBED_FONTCOLOR_PTR_ADDR  0x015CD634

//012CD488         | C705 98555D01 FFFFFFFF             | mov dword ptr ds:[0x15D5598],0xFFFFFFFF                                 |
#define WEBBED_FONT_PTR_ADDR 0x015D5598

#define WEBBED_DRAWTEXT_ADDR 0x011C7FA0

#define WEBBED_WINDOWMOUSEGETX_ADDR 0x011CB750

#define WEBBED_WINDOWMOUSEGETY_ADDR 0x011CB770

//0121958B | 66:0F6E05 A8996601 | movd xmm0, dword ptr ds : [0x16699A8] |
#define WEBBED_CURRENTROOMID_ADDR 0x16699A8

//0118E322         | 0305 AC746601                      | add eax,dword ptr ds:[0x16674AC]                                        | 016674AC:&"camera_create"
#define WEBBED_GMFUNTABLE_PTR_ADDR 0x016674AC

#define WEBBED_GMFUNTABLE_DRAWTEXT_OFFSET 0x82A0

//011C663E         | A2 BF996601                        | mov byte ptr ds:[0x16699BF],al                                          |
#define WEBBED_DRAWENABLED_PTR_ADDR  0x16699BF

#define WEBBED_GMLCALLFUNCTION_ADDR  0x0118E2F0

//01218F60 <webbed | 8B0D 80996601                      | mov ecx,dword ptr ds:[0x1669980]                                        |
#define WEBBED_PHYSICS_SYSTEM_PTR_ADDR 0x1669980

//011BB8AF         | F2:0F1005 289A6601                 | movsd xmm0,qword ptr ds:[0x1669A28]                                     |
#define WEBBED_GAMESPEED_ADDR 0x1669A28

#define WEBBED_GAMESAVE_ADDR 0x0119D500