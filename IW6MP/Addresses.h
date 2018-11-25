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
		TransformSeed = 0x8222EC70;
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

		AngleNormalize360 = 0x82552EC0;
		YawVectors2D = 0x82551B20;
		CL_ClientIsInMyParty = 0x8228DC48;
		CalcCompassFriendlySize = 0x8225FC18;
		SERVER_ID = 0x82F2B28;
		BG_EvaluateTrajectory = 0x0;
		CG_GetVelocity = 0x82237738;
		Sys_EnterCriticalSection = 0x82573DC0;
		Sys_LeaveCriticalSection = 0x82573DD8;
		CG_IsSelectingLocation = 0x8225F300;
		VehicleCl_IsClientValid = 0x825E54B0;
		VehicleCl_GetClientDef = 0x825E5548;
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

extern addr_s* addr;

extern void LoadAddresses();

enum miscKey_t {
	KEY_LT = 0x5555,
	KEY_RT = 0x6666
};