#pragma once
#include "Detour.h"

extern Detour* (*GetDetour)();
extern void(*XNotify)(wchar_t*);
extern void*(*user_memcpy)(void*, void*, size_t);
extern char*(*user_strcpy)(char*, const char*);
extern void*(*user_memset)(void*, int, size_t);
extern int(*user_strcmp)(const char*, const char*);
extern void* (*xbOHookFunction)(Detour* a, void* b, void* c);

#include "Structures.h"

struct addr_s
{
	//Dvars
	unsigned int Dvar_FindVar;
	unsigned int Dvar_GetBool;
	unsigned int Dvar_SetBool;
	unsigned int Dvar_GetInt;
	unsigned int Dvar_GetFloat;
	unsigned int Dvar_GetString;
	unsigned int Dvar_SetStringByName;

	//string tables
	unsigned int SL_GetStringOfSize;
	unsigned int SL_ConvertToString;

	//Misc
	unsigned int CG_GameMessage;
	unsigned int CG_BoldGameMessage;
	unsigned int Cbuf_AddText;
	unsigned int CL_AddReliableCommand;
	unsigned int CG_VisionSetStartLerp_To;
	unsigned int UI_GetMapName;
	unsigned int UI_GetGameTypeName;
	unsigned int SEH_StringEd_GetString;
	unsigned int Com_GetClientDObj;
	unsigned int CG_DObjGetWorldTagPos;
	unsigned int CG_GetPlayerVelocity;
	unsigned int CG_GetPlayerViewOrigin;
	unsigned int CG_LocationalTrace;
	unsigned int BG_GetSpreadForWeapon;
	unsigned int TransformSeed;
	unsigned int RandomBulletDir;
	unsigned int Trace_GetEntityHitId;
	unsigned int SV_IsMigrating;
	unsigned int CL_IsServerRestarting;
	unsigned int CG_PlayRumbleOnClient;
	unsigned int UI_PlaySound;
	unsigned int BG_GetAmmoInClip;

	//Drawing
	unsigned int R_TextWidth;
	unsigned int R_TextHeight;
	unsigned int R_AddCmdDrawText;
	unsigned int R_AddCmdDrawStretchPic;
	unsigned int CG_DrawRotatedPicPhysical;
	unsigned int Material_RegisterHandle;
	unsigned int R_RegisterFont;
	unsigned int ScreenPlacement;
	unsigned int R_TextLineWrapPosition;
	unsigned int R_ConvertColorToBytes;

	//Structs
	unsigned int WeaponDef;
	unsigned int WeaponVariantDef;
	unsigned int cgs_t;
	unsigned int cg_s;
	unsigned int centity_s;
	unsigned int clientactive_t;
	unsigned int ClientNetwork_s;
	unsigned int clientUIActive;

	//AutoWall
	unsigned int BulletTrace;
	unsigned int BG_AdvanceTrace;
	unsigned int BG_GetSurfacePenetrationDepth;
	unsigned int CG_ClientHasPerk;
	unsigned int FireBulletPenetrate;
	unsigned int BG_GetPerkIndexForName;

	//Name shit
	unsigned int NameAddress;
	unsigned int ClantagAddress;

	//Hooks
	unsigned int XAM_191;
	unsigned int RB_SetTessTechnique;
	unsigned int R_EndFrame;
	unsigned int SCR_DrawScreenField;
	unsigned int Menu_PaintAll;
	unsigned int CL_WritePacket;
	unsigned int CG_Obituary;
	unsigned int CL_Disconnect;
	unsigned int Com_Error;
	unsigned int CL_CreateNewCommand;

	//Misc
	unsigned int AngleNormalize360;
	unsigned int YawVectors2D;
	unsigned int CL_ClientIsInMyParty;
	unsigned int CalcCompassFriendlySize;
	unsigned int SERVER_ID;
	unsigned int BG_EvaluateTrajectory;
	unsigned int CG_GetVelocity;
	unsigned int Sys_EnterCriticalSection;
	unsigned int Sys_LeaveCriticalSection;
	unsigned int CG_IsSelectingLocation;
	unsigned int VehicleCl_IsClientValid;
	unsigned int VehicleCl_GetClientDef;

	unsigned int _0x8204B874;
	unsigned int _0x83D51E20;
	unsigned int _0x8431C270;
	unsigned int _0x84445380;
	unsigned int _0x00002000;
	unsigned int _0x00000040;
	unsigned int _0x00000004;
	unsigned int _0x0000000C;
	unsigned int _0x00000002;
	unsigned int _0x842BE2AC;
	unsigned int _0x00000008;
	unsigned int _0x00000038;
	unsigned int _0x00000010;
	unsigned int _0x00000014;
	unsigned int _0x00000018;
	unsigned int _0x0000001C;
	unsigned int _0x00000020;
	unsigned int _0x00000024;
	unsigned int _0x00000028;
	unsigned int _0x0000002C;
	unsigned int _0x00000030;
	unsigned int _0x00000034;
	unsigned int _0x0000003C;
	unsigned int _0x8265A6C0;
	unsigned int _0x82732100;
	unsigned int _0x82AC1188;
	unsigned int _0x00000D10;
	unsigned int _0x0000004C;
	unsigned int _0x00040000;
	unsigned int _0x82ACCD00;
	unsigned int _0x000003BC;
	unsigned int _0x000003B8;
	unsigned int _0x82AC0EC8;
	unsigned int _0x00000160;
	unsigned int _0x82AC0008;
	unsigned int _0x000000E0;
	unsigned int _0x00000088;
	unsigned int _0x840F07D0;
	unsigned int _0x00000680;
	unsigned int _0x82AC01C8;
	unsigned int _0x8225FB08;
	unsigned int _0x8228B8F0;
	unsigned int _0x822C73C0;
	unsigned int _0x82385B38;
	unsigned int _0x4E800020;
	unsigned int _0x7D8802A6;
	unsigned int _0x000C6448;
	unsigned int _0x00803003;
	unsigned int _0x8266ABA8;
	unsigned int _0x8431C278;
	unsigned int _0x8431C274;
	unsigned int _0x00001868;
	unsigned int _0x842BC180;
	unsigned int _0x842BC184;
	unsigned int _0x842BC188;
	unsigned int _0x842BC18C;
	unsigned int _0x82482168;
	unsigned int _0x000000FF;
	unsigned int _0x844DD004;
	unsigned int _0x82686220;
	unsigned int _0x00000021;
	unsigned int _0x82ACCADC;
	unsigned int _0x822583D0;
	unsigned int _0x82258BEC;
	unsigned int _0x82258BE0;
	unsigned int _0x82258BF8;
	unsigned int _0x82258C04;
	unsigned int _0x822C9344;
	unsigned int _0x4BFFEAA1;
	unsigned int _0x4BFFFBA5;
	unsigned int _0x4BFFDED9;
	unsigned int _0x4BFFFD49;
	unsigned int _0x4BFFFEC5;
	unsigned int _0x4BF8AE4D;
	unsigned int _0x60000000;
	unsigned int _0x822654C8;
	unsigned int _0x8227F19B;
	unsigned int _0x3B600001;
	unsigned int _0x551B7FFE;
	unsigned int _0x8220D778;
	unsigned int _0x8220DA60;
	unsigned int _0x8220D8B0;

#ifdef DEVKIT
	void SetAddresses()
	{
		Dvar_FindVar = 0x825637E0;
		Dvar_GetBool = 0x825639B0;
		Dvar_SetBool = 0x825667C8;
		Dvar_GetInt = 0x82563A18;
		Dvar_GetFloat = 0x82563A80;
		Dvar_GetString = 0x82563AF0;
		Dvar_SetStringByName = 0x825670E0;
		SL_GetStringOfSize = 0x824934D8;
		SL_ConvertToString = 0x82492318;
		CG_GameMessage = 0x82282090;
		CG_BoldGameMessage = 0x822820F0;
		Cbuf_AddText = 0x8244C738;
		CL_AddReliableCommand = 0x822D8F88;
		CG_VisionSetStartLerp_To = 0x822B74A8;
		UI_GetMapName = 0x82527AD0;
		UI_GetGameTypeName = 0x82527CF8;
		SEH_StringEd_GetString = 0x82526538;
		Com_GetClientDObj = 0x824723D8;
		CG_DObjGetWorldTagPos = 0x82273BC8;
		CG_GetPlayerVelocity = 0x0; //Not Used
		CG_GetPlayerViewOrigin = 0x822BC078;
		CG_LocationalTrace = 0x822CB3B0;
		BG_GetSpreadForWeapon = 0x82254700;
		TransformSeed = 0x82234840;
		RandomBulletDir = 0x82430F10;
		Trace_GetEntityHitId = 0x82443F98;
		SV_IsMigrating = 0x824E0C68;
		CL_IsServerRestarting = 0x822CDE20;
		CG_PlayRumbleOnClient = 0x82296FA0;
		UI_PlaySound = 0x82726738;
		BG_GetAmmoInClip = 0x8222C618;
		R_TextWidth = 0x8264DC78;
		R_TextHeight = 0x8264DD70;
		R_AddCmdDrawText = 0x8266CEB8;
		R_AddCmdDrawStretchPic = 0x8266AF28;
		CG_DrawRotatedPicPhysical = 0x82270138;
		Material_RegisterHandle = 0x8265A470;
		R_RegisterFont = 0x8264DBF0;
		ScreenPlacement = 0x82BCDD10;
		R_TextLineWrapPosition = 0x8264DDB8;
		R_ConvertColorToBytes = 0x82683280;
		WeaponDef = 0x8213EDE0;
		WeaponVariantDef = 0x82143408;
		cgs_t = 0x82AD56F8;
		cg_s = 0x82AD56FC;
		centity_s = 0x82AD11AC;
		clientUIActive = 0x82BAC408;
		clientactive_t = 0x82BAC404;
		ClientNetwork_s = 0x82BBD5D8;
		BulletTrace = 0x822BC168;
		BG_AdvanceTrace = 0x82253CC8;
		BG_GetSurfacePenetrationDepth = 0x82249270;
		CG_ClientHasPerk = 0x0; //not used
		FireBulletPenetrate = 0x822C7518;
		BG_GetPerkIndexForName = 0x8222BE48;
		NameAddress = 0x83F0A35C;
		ClantagAddress = 0x82BB6896;
		XAM_191 = 0x828103EC;
		RB_SetTessTechnique = 0x0; //not used
		R_EndFrame = 0x8266BFE0;
		SCR_DrawScreenField = 0x822E6C60;
		Menu_PaintAll = 0x8253E140;
		CL_WritePacket = 0x822D68C0;
		CG_Obituary = 0x82276650;
		CL_Disconnect = 0x822DD3C8;
		Com_Error = 0x824712B8;
		CL_CreateNewCommand = 0x822D67E8;
		AngleNormalize360 = 0x82552EC0;
		YawVectors2D = 0x82551B20;
		CL_ClientIsInMyParty = 0x8228DC48;
		CalcCompassFriendlySize = 0x8225FC18;
		SERVER_ID = 0x82B55908;
		BG_EvaluateTrajectory = 0x0;
		CG_GetVelocity = 0x82237738;
		Sys_EnterCriticalSection = 0x82573DC0;
		Sys_LeaveCriticalSection = 0x82573DD8;
		CG_IsSelectingLocation = 0x8225F300;
		VehicleCl_IsClientValid = 0x825E54B0;
		VehicleCl_GetClientDef = 0x825E5548;

		_0x8204B874 = 0x8204B874;
		_0x83D51E20 = 0x83D51E20;
		_0x8431C270 = 0x8431C270;
		_0x84445380 = 0x84445380;
		_0x00002000 = 0x00002000;
		_0x00000040 = 0x00000040;
		_0x00000004 = 0x00000004;
		_0x0000000C = 0x0000000C;
		_0x00000002 = 0x00000002;
		_0x842BE2AC = 0x842BE2AC;
		_0x00000008 = 0x00000008;
		_0x00000038 = 0x00000038;
		_0x00000010 = 0x00000010;
		_0x00000014 = 0x00000014;
		_0x00000018 = 0x00000018;
		_0x0000001C = 0x0000001C;
		_0x00000020 = 0x00000020;
		_0x00000024 = 0x00000024;
		_0x00000028 = 0x00000028;
		_0x0000002C = 0x0000002C;
		_0x00000030 = 0x00000030;
		_0x00000034 = 0x00000034;
		_0x0000003C = 0x0000003C;
		_0x8265A6C0 = 0x8265A6C0;
		_0x82732100 = 0x82732100;
		_0x82AC1188 = 0x82AC1188;
		_0x00000D10 = 0x00000D10;
		_0x0000004C = 0x0000004C;
		_0x00040000 = 0x00040000;
		_0x82ACCD00 = 0x82ACCD00;
		_0x000003BC = 0x000003BC;
		_0x000003B8 = 0x000003B8;
		_0x82AC0EC8 = 0x82AC0EC8;
		_0x00000160 = 0x00000160;
		_0x82AC0008 = 0x82AC0008;
		_0x000000E0 = 0x000000E0;
		_0x00000088 = 0x00000088;
		_0x840F07D0 = 0x840F07D0;
		_0x00000680 = 0x00000680;
		_0x82AC01C8 = 0x82AC01C8;
		_0x8225FB08 = 0x8225FB08;
		_0x8228B8F0 = 0x8228B8F0;
		_0x822C73C0 = 0x822C73C0;
		_0x82385B38 = 0x82385B38;
		_0x4E800020 = 0x4E800020;
		_0x7D8802A6 = 0x7D8802A6;
		_0x000C6448 = 0x000C6448;
		_0x00803003 = 0x00803003;
		_0x8266ABA8 = 0x8266ABA8;
		_0x8431C278 = 0x8431C278;
		_0x8431C274 = 0x8431C274;
		_0x00001868 = 0x00001868;
		_0x842BC180 = 0x842BC180;
		_0x842BC184 = 0x842BC184;
		_0x842BC188 = 0x842BC188;
		_0x842BC18C = 0x842BC18C;
		_0x82482168 = 0x82482168;
		_0x000000FF = 0x000000FF;
		_0x844DD004 = 0x844DD004;
		_0x82686220 = 0x82686220;
		_0x00000021 = 0x00000021;
		_0x82ACCADC = 0x82ACCADC;
		_0x822583D0 = 0x822583D0;
		_0x82258BEC = 0x82258BEC;
		_0x82258BE0 = 0x82258BE0;
		_0x82258BF8 = 0x82258BF8;
		_0x82258C04 = 0x82258C04;
		_0x822C9344 = 0x822C9344;
		_0x4BFFEAA1 = 0x4BFFEAA1;
		_0x4BFFFBA5 = 0x4BFFFBA5;
		_0x4BFFDED9 = 0x4BFFDED9;
		_0x4BFFFD49 = 0x4BFFFD49;
		_0x4BFFFEC5 = 0x4BFFFEC5;
		_0x4BF8AE4D = 0x4BF8AE4D;
		_0x60000000 = 0x60000000;
		_0x822654C8 = 0x822654C8;
		_0x8227F19B = 0x8227F19B;
		_0x3B600001 = 0x3B600001;
		_0x551B7FFE = 0x551B7FFE;
		_0x8220D778 = 0x8220D778;
		_0x8220DA60 = 0x8220DA60;
		_0x8220D8B0 = 0x8220D8B0;
	}
#endif
};

struct GAME_ADDRESS_TRANSFER_GO
{
	addr_s* addr;

void* SL_ConvertToString;
void* SL_GetString;
void* SL_GetStringOfSize;
void* Dvar_FindVar;
void* Dvar_SetStringByName;
void* Dvar_SetBool;
void* Dvar_GetBool;
void* Dvar_GetString;
void* Dvar_GetFloat;
void* Dvar_GetInt;
void* ClampChar_r;
void* SEH_StringEd_GetString;
void* DEG2RAD;
void* getStructures;
void* ApplyPatches;
void* GetAsyncKeyState;
void* TextHeight;
void* TextWidth;
void* R_RegisterFont;
void* R_GetFontPathFromName;
void* CG_DrawRotatedPicPhysical;
void* R_AddCmdDrawStretchPic;
void* R_AddCmdDrawText;
void* R_TextHeight;
void* R_TextWidth;
void* R_AddCmdDrawStretchPicRotateXY;
void* SetTextCentered;
void* SetTextRight;
void* Material_RegisterHandle;
void* RandomBulletDir;
void* CG_GoodRandomFloat;
void* TransformSeed;
void* CL_DrawStretchPicRotatedST;
void* R_AddCmdDrawStretchPicRotateST;

};

struct SYS_FUNCTION_ADDRESS
{
	void* HookFunction;
	void* XNotify;
	void* memcpy;
	void* memset;
	void* strcpy;
	void* memcmp;
	void* strcmp;
	void* Sleep;
	void* MmIsAddressValid;
	void* FIsMmIoAddress;
	void* GetDetours;
};

extern int(*Dvar_FindVar)(const char* Dvar);
extern void(*Dvar_SetStringByName)(const char* Dvar, const char*  val);
extern void(*Dvar_SetBool)(const char* Dvar, bool val);
extern bool(*Dvar_GetBool)(const char* Dvar);
extern const char* (*Dvar_GetString)(const char* Dvar);
extern float(*Dvar_GetFloat)(const char* Dvar);
extern int(*Dvar_GetInt)(const char* Dvar);
extern bool(*getStructures) (int**, int**, int**, int**, int**, int**);
extern const char*(*SL_ConvertToString)(unsigned int);
extern unsigned int(*SL_GetString)(const char*, unsigned int);
extern unsigned int(*SL_GetStringOfSize)(const char *, int, int, int);

extern int(*ApplyPatches)();

extern char(*ClampChar_r)(const int i);
extern const char* (*SEH_StringEd_GetString)(const char* str);
extern float(*DEG2RAD)(float DEG);
extern bool(*GetAsyncKeyState)(DWORD KEY);

extern int(*TextHeight)(const char* font, float scale);
extern int(*TextWidth)(const char* text, const char* font);
extern int(*R_RegisterFont)(const char* name, int imageTrack);
extern const char* (*R_GetFontPathFromName)(const char* name);
extern void(*CG_DrawRotatedPicPhysical)(int ScreenPlacement, float x, float y, float width, float height, float angle, float* color, Material* material);
extern void(*R_AddCmdDrawStretchPic)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, float* color, Material* material);
extern void(*R_AddCmdDrawText)(const char* Text, int MaxCharacters, int Font, float X, float Y, float XScale, float YScale, float Angle, float* Color, int Style);
extern int(*R_TextHeight)(int font);
extern int(*R_TextWidth)(const char* text, int maxchars, int font);

extern void(*R_AddCmdDrawStretchPicRotateXY)(float x, float y, float w, float h, float xScale, float yScale, float xay, float yay, float Angle, float *color, const char* material);
extern void(*SetTextCentered)(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A);
extern void(*SetTextRight)(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A);
extern Material* (*Material_RegisterHandle)(const char* name, int imageTrack);

extern void(*CL_DrawStretchPicRotatedST)(int scrPlace, float x, float y, float w, float h, int horzAlign, int vertAlign, float centerS, float centerT, float radiusST, float scaleFinalS, float s1, float t1, float s2, float t2, float Angle, float* colour, Material* material);
extern void(*R_AddCmdDrawStretchPicRotateST)(float x, float y, float w, float h, float centerS, float centerT, float radiusST, float scaleFinalS, float s1, float t1, float s2, float t2, float Angle, float* colour, Material* material);

extern void(*RandomBulletDir)(int* randSeed, float *x, float *y);
extern float(*CG_GoodRandomFloat)(int *pHoldrand);
extern int(*TransformSeed)(int *pHoldrand);

extern addr_s* addr;

extern void LoadAddresses();

enum miscKey_t {
	KEY_LT = 0x5555,
	KEY_RT = 0x6666
};