#include "stdafx.h"
#include "Addresses.h"

addr_s* addr = 0;

#if defined(DEVKIT)
void ApplyGamePatches();
int * Real_GetDetours();
void* Real_HookFunction(Detour* a, void* b, void* c);
VOID Real_XNotify(CONST PWCHAR NotifyText);
const char* Real_SL_ConvertToString(unsigned int stringValue);
unsigned int Real_SL_GetString(const char* str, unsigned int user);
unsigned int Real_SL_GetStringOfSize(const char *str, int user, int length, int type);

int Real_Dvar_FindVar(const char* Dvar);
void Real_Dvar_SetStringByName(const char* Dvar, const char*  val);
void Real_Dvar_SetBool(const char* Dvar, bool val);
bool Real_Dvar_GetBool(const char* Dvar);
const char* Real_Dvar_GetString(const char* Dvar);
float Real_Dvar_GetFloat(const char* Dvar);
int Real_Dvar_GetInt(const char* Dvar);

char Real_ClampChar_r(const int i);
const char* Real_SEH_StringEd_GetString(const char* str);
float Real_DEG2RAD(float DEG);
bool Real_getStructures(int** cgs, int** cg, int** Entity, int** ClientActive_t, int** ClientNetwork, int** clientUIActive);
bool Real_GetAsyncKeyState(DWORD KEY);

int Real_TextHeight(const char* font, float scale);
int Real_TextWidth(const char* text, const char* font);
int Real_R_RegisterFont(const char* name, int imageTrack);
const char* Real_R_GetFontPathFromName(const char* name);
void Real_CG_DrawRotatedPicPhysical(int ScreenPlacement, float x, float y, float width, float height, float angle, float* color, Material* material);
void Real_R_AddCmdDrawStretchPic(float x, float y, float w, float h, float s0, float t0, float s1, float t1, float* color, Material* material);
void Real_R_AddCmdDrawText(const char* Text, int MaxCharacters, int Font, float X, float Y, float XScale, float YScale, float Angle, float* Color, int Style);
int Real_R_TextHeight(int font);
int Real_R_TextWidth(const char* text, int maxchars, int font);

Material* Real_Material_RegisterHandle(const char* name, int imageTrack);
void Real_SetTextRight(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A);
void Real_SetTextCentered(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A);
void Real_R_AddCmdDrawStretchPicRotateXY(float x, float y, float w, float h, float xScale, float yScale, float xay, float yay, float Angle, float *color, const char* material);

#endif

int(*ApplyPatches)();
Detour* (*GetDetour)();
void(*XNotify)(wchar_t*);
void*(*user_memcpy)(void*, void*, size_t);
char*(*user_strcpy)(char*, const char*);
void*(*user_memset)(void*, int, size_t);
int(*user_strcmp)(const char*, const char*);
bool(*getStructures) (int**, int**, int**, int**, int**, int**);
void*(*xbOHookFunction)(Detour* a, void* b, void* c);
int(*Dvar_FindVar)(const char* Dvar);
void(*Dvar_SetStringByName)(const char* Dvar, const char*  val);
void(*Dvar_SetBool)(const char* Dvar, bool val);
bool(*Dvar_GetBool)(const char* Dvar);
const char* (*Dvar_GetString)(const char* Dvar);
float(*Dvar_GetFloat)(const char* Dvar);
int(*Dvar_GetInt)(const char* Dvar);
const char*(*SL_ConvertToString)(unsigned int);
unsigned int(*SL_GetString)(const char*, unsigned int);
unsigned int(*SL_GetStringOfSize)(const char *, int, int, int);
char(*ClampChar_r)(const int i);
const char*(*SEH_StringEd_GetString)(const char* str);
float(*DEG2RAD)(float DEG);
bool(*GetAsyncKeyState)(DWORD KEY);
int(*TextHeight)(const char* font, float scale);
int(*TextWidth)(const char* text, const char* font);
int(*R_RegisterFont)(const char* name, int imageTrack);
const char* (*R_GetFontPathFromName)(const char* name);
void(*CG_DrawRotatedPicPhysical)(int ScreenPlacement, float x, float y, float width, float height, float angle, float* color, Material* material);
void(*R_AddCmdDrawStretchPic)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, float* color, Material* material);
void(*R_AddCmdDrawText)(const char* Text, int MaxCharacters, int Font, float X, float Y, float XScale, float YScale, float Angle, float* Color, int Style);
int(*R_TextHeight)(int font);
int(*R_TextWidth)(const char* text, int maxchars, int font);
void(*R_AddCmdDrawStretchPicRotateXY)(float x, float y, float w, float h, float xScale, float yScale, float xay, float yay, float Angle, float *color, const char* material);
void(*SetTextCentered)(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A);
void(*SetTextRight)(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A);
Material* (*Material_RegisterHandle)(const char* name, int imageTrack);

__int64 __declspec(naked) HvxGetVersion(unsigned int key, __int64 type, __int64 SourceAddress, __int64 DestAddress, __int64 lenInBytes)
{
	__asm
	{
		li r0, 0x0
		sc
		blr
	}
}

unsigned int HVGetVersionsPeekDWORD(unsigned long long Address)
{
	unsigned int *data = (unsigned int *)XPhysicalAlloc(4, MAXULONG_PTR, 0, PAGE_READWRITE); ;

	__int64 dest = 0x8000000000000000ULL + (unsigned int)MmGetPhysicalAddress((unsigned int*)data);

	HvxGetVersion(0x72627472, 5, Address, dest, 4);

	unsigned int retData = *(unsigned int*)data;

	XPhysicalFree(data);

	return retData;
}

bool CWriteFile(const char* FilePath, const void* Data, unsigned int Size)
{
	HANDLE fHandle = CreateFile(FilePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fHandle == INVALID_HANDLE_VALUE) {
		return FALSE;
	}

	DWORD writeSize = Size;
	if (WriteFile(fHandle, Data, writeSize, &writeSize, NULL) != TRUE) {
		return FALSE;
	}
	CloseHandle(fHandle);
	return TRUE;

}
void LoadAddresses()
{
#if defined(DEVKIT)
	GetDetour = (Detour*(*)())Real_GetDetours;
	XNotify = (void(*)(wchar_t*))Real_XNotify;
	user_memcpy = (void*(*)(void*, void*, size_t))memcpy;
	user_strcpy = (char*(*)(char*, const char*))strcpy;
	user_memset = (void*(*)(void*, int, size_t))memset;
	user_strcmp = (int(*)(const char*, const char*))strcmp;
	xbOHookFunction = (void*(*)(Detour* a, void* b, void* c))Real_HookFunction;

	SL_ConvertToString = (const char*(*)(unsigned int))Real_SL_ConvertToString;
	SL_GetString = (unsigned int(*)(const char*, unsigned int))Real_SL_GetString;
	SL_GetStringOfSize = (unsigned int(*)(const char *, int, int, int))Real_SL_GetStringOfSize;

	Dvar_FindVar = (int(*)(const char* Dvar))Real_Dvar_FindVar;
	Dvar_SetStringByName = (void(*)(const char* Dvar, const char*  val))Real_Dvar_SetStringByName;
	Dvar_SetBool = (void(*)(const char* Dvar, bool val))Real_Dvar_SetBool;
	Dvar_GetBool = (bool(*)(const char* Dvar))Real_Dvar_GetBool;
	Dvar_GetString = (const char*(*)(const char* Dvar))Real_Dvar_GetString;
	Dvar_GetFloat = (float(*)(const char* Dvar))Real_Dvar_GetFloat;
	Dvar_GetInt = (int(*)(const char* Dvar))Real_Dvar_GetInt;

	ClampChar_r = (char(*)(const int i))Real_ClampChar_r;
	SEH_StringEd_GetString = (const char* (*)(const char* str))Real_SEH_StringEd_GetString;
	DEG2RAD = (float(*)(float DEG))Real_DEG2RAD;
	getStructures = (bool(*)(int**, int**, int**, int**, int**, int**))Real_getStructures;
	ApplyPatches = (int(*)())ApplyGamePatches;
	GetAsyncKeyState = (bool(*)(DWORD KEY))Real_GetAsyncKeyState;

	TextHeight = (int(*)(const char* font, float scale))Real_TextHeight;
	TextWidth = (int(*)(const char* text, const char* font))Real_TextWidth;
	R_RegisterFont = (int(*)(const char* name, int imageTrack))Real_R_RegisterFont;
	R_GetFontPathFromName = (const char* (*)(const char* name))Real_R_GetFontPathFromName;
	CG_DrawRotatedPicPhysical = (void(*)(int ScreenPlacement, float x, float y, float width, float height, float angle, float* color, Material* material))Real_CG_DrawRotatedPicPhysical;
	R_AddCmdDrawStretchPic = (void(*)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, float* color, Material* material))Real_R_AddCmdDrawStretchPic;
	R_AddCmdDrawText = (void(*)(const char* Text, int MaxCharacters, int Font, float X, float Y, float XScale, float YScale, float Angle, float* Color, int Style))Real_R_AddCmdDrawText;
	R_TextHeight = (int(*)(int font))Real_R_TextHeight;
	R_TextWidth = (int(*)(const char* text, int maxchars, int font))Real_R_TextWidth;

	R_AddCmdDrawStretchPicRotateXY = (void(*)(float x, float y, float w, float h, float xScale, float yScale, float xay, float yay, float Angle, float *color, const char* material))Real_R_AddCmdDrawStretchPicRotateXY;
	SetTextCentered = (void(*)(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A))Real_SetTextCentered;
	SetTextRight = (void(*)(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A))Real_SetTextRight;
	Material_RegisterHandle = (Material* (*)(const char* name, int imageTrack))Real_Material_RegisterHandle;

	addr = new addr_s;
	addr->SetAddresses();

#else
	GAME_ADDRESS_TRANSFER_GO* Game_Functions = (GAME_ADDRESS_TRANSFER_GO*)ReverseInt(HVGetVersionsPeekDWORD(0x800001000000BED0));
	int AddressLength = ReverseInt(HVGetVersionsPeekDWORD(0x800001000000BED4));
	SYS_FUNCTION_ADDRESS* Sys_Functions = (SYS_FUNCTION_ADDRESS*)ReverseInt(HVGetVersionsPeekDWORD(0x800001000000BED8));

	if (Sys_Functions)
	{
		GetDetour = (Detour*(*)())Sys_Functions->GetDetours;
		XNotify = (void(*)(wchar_t*))Sys_Functions->XNotify;

		user_memcpy = (void*(*)(void*, void*, size_t))Sys_Functions->memcpy;
		user_strcpy = (char*(*)(char*, const char*))Sys_Functions->strcpy;
		user_memset = (void*(*)(void*, int, size_t))Sys_Functions->memset;
		user_strcmp = (int(*)(const char*, const char*))Sys_Functions->strcmp;

		xbOHookFunction = (void*(*)(Detour* a, void* b, void* c))Sys_Functions->HookFunction;
	}

	if (Game_Functions)
	{
		addr = Game_Functions->addr;

		SL_ConvertToString = (const char*(*)(unsigned int))(void*)ReverseInt((int)Game_Functions->SL_ConvertToString);
		SL_GetString = (unsigned int(*)(const char*, unsigned int))(void*)ReverseInt((int)Game_Functions->SL_GetString);
		SL_GetStringOfSize = (unsigned int(*)(const char *, int, int, int))(void*)ReverseInt((int)Game_Functions->SL_GetStringOfSize);

		Dvar_FindVar = (int(*)(const char* Dvar))(void*)ReverseInt((int)Game_Functions->Dvar_FindVar);
		Dvar_SetStringByName = (void(*)(const char* Dvar, const char*  val))(void*)ReverseInt((int)Game_Functions->Dvar_SetStringByName);
		Dvar_SetBool = (void(*)(const char* Dvar, bool val))(void*)ReverseInt((int)Game_Functions->Dvar_SetBool);
		Dvar_GetBool = (bool(*)(const char* Dvar))(void*)ReverseInt((int)Game_Functions->Dvar_GetBool);
		Dvar_GetString = (const char*(*)(const char* Dvar))(void*)ReverseInt((int)Game_Functions->Dvar_GetString);
		Dvar_GetFloat = (float(*)(const char* Dvar))(void*)ReverseInt((int)Game_Functions->Dvar_GetFloat);
		Dvar_GetInt = (int(*)(const char* Dvar))(void*)ReverseInt((int)Game_Functions->Dvar_GetInt);

		ClampChar_r = (char(*)(const int i))(void*)ReverseInt((int)Game_Functions->ClampChar_r);
		SEH_StringEd_GetString = (const char* (*)(const char* str))(void*)ReverseInt((int)Game_Functions->SEH_StringEd_GetString);
		DEG2RAD = (float(*)(float DEG))(void*)ReverseInt((int)Game_Functions->DEG2RAD);
		getStructures = (bool(*)(int**, int**, int**, int**, int**, int**))(void*)ReverseInt((int)Game_Functions->getStructures);
		ApplyPatches = (int(*)())(void*)ReverseInt((int)Game_Functions->ApplyPatches);
		GetAsyncKeyState = (bool(*)(DWORD KEY))(void*)ReverseInt((int)Game_Functions->GetAsyncKeyState);

		TextHeight = (int(*)(const char* font, float scale))(void*)ReverseInt((int)Game_Functions->TextHeight);
		TextWidth = (int(*)(const char* text, const char* font))(void*)ReverseInt((int)Game_Functions->TextWidth);
		R_RegisterFont = (int(*)(const char* name, int imageTrack))(void*)ReverseInt((int)Game_Functions->R_RegisterFont);
		R_GetFontPathFromName = (const char* (*)(const char* name))(void*)ReverseInt((int)Game_Functions->R_GetFontPathFromName);
		CG_DrawRotatedPicPhysical = (void(*)(int ScreenPlacement, float x, float y, float width, float height, float angle, float* color, Material* material))(void*)ReverseInt((int)Game_Functions->CG_DrawRotatedPicPhysical);
		R_AddCmdDrawStretchPic = (void(*)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, float* color, Material* material))(void*)ReverseInt((int)Game_Functions->R_AddCmdDrawStretchPic);
		R_AddCmdDrawText = (void(*)(const char* Text, int MaxCharacters, int Font, float X, float Y, float XScale, float YScale, float Angle, float* Color, int Style))(void*)ReverseInt((int)Game_Functions->R_AddCmdDrawText);
		R_TextHeight = (int(*)(int font))(void*)ReverseInt((int)Game_Functions->R_TextHeight);
		R_TextWidth = (int(*)(const char* text, int maxchars, int font))(void*)ReverseInt((int)Game_Functions->R_TextWidth);

		R_AddCmdDrawStretchPicRotateXY = (void(*)(float x, float y, float w, float h, float xScale, float yScale, float xay, float yay, float Angle, float *color, const char* material))(void*)ReverseInt((int)Game_Functions->R_AddCmdDrawStretchPicRotateXY);
		SetTextCentered = (void(*)(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A))(void*)ReverseInt((int)Game_Functions->SetTextCentered);
		SetTextRight = (void(*)(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A))(void*)ReverseInt((int)Game_Functions->SetTextRight);
		Material_RegisterHandle = (Material* (*)(const char* name, int imageTrack))(void*)ReverseInt((int)Game_Functions->Material_RegisterHandle);
	}

#endif	

}

#if defined(DEVKIT)

Material* Real_Material_RegisterHandle(const char* name, int imageTrack)
{
	Material*(*Material_RegisterHandle)(const char *name, int imageTrack) = (Material*(*)(const char*, int))addr->Material_RegisterHandle;
	return Material_RegisterHandle(name, imageTrack);
}

void Real_SetTextRight(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A)
{
	float Color[4] = { R, G, B, A };
	R_AddCmdDrawText(text, 0x7FFFFFFF, Real_R_RegisterFont(font, 0), (x - ((Real_TextWidth(text, font)) * xScale)), y, xScale, yScale, 0, Color, 0);
}

void Real_SetTextCentered(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A)
{
	float Color[4] = { R, G, B, A };
	R_AddCmdDrawText(text, 0x7FFFFFFF, Real_R_RegisterFont(font, 0), (x - ((Real_TextWidth(text, font) / 2) * xScale)), y, xScale, yScale, 0, Color, 0);
}

void Real_R_AddCmdDrawStretchPicRotateXY(float x, float y, float w, float h, float xScale, float yScale, float xay, float yay, float Angle, float *color, const char* material)
{
	CG_DrawRotatedPicPhysical(addr->ScreenPlacement, x, y, w, h, Angle, color, Real_Material_RegisterHandle(material, 0));
}

int Real_R_TextWidth(const char* text, int maxchars, int font)
{
	int(*R_TextWidth)(const char* text, int maxchars, int font) = (int(*)(const char*, int, int))addr->R_TextWidth;
	return R_TextWidth(text, maxchars, font);
}

int Real_R_TextHeight(int font)
{
	int(*R_TextHeight)(int font) = (int(*)(int))addr->R_TextHeight;
	return R_TextHeight(font);
}

void Real_R_AddCmdDrawText(const char* Text, int MaxCharacters, int Font, float X, float Y, float XScale, float YScale, float Angle, float* Color, int Style)
{
	void(*R_AddCmdDrawText)(const char* Text, int MaxCharacters, int Font, float X, float Y, float XScale, float YScale, float Angle, float* Color, int Style) = (void(*)(const char*, int, int, float, float, float, float, float, float*, int))addr->R_AddCmdDrawText;
	R_AddCmdDrawText(Text, MaxCharacters, Font, X, Y, XScale, YScale, Angle, Color, Style);
}

void Real_R_AddCmdDrawStretchPic(float x, float y, float w, float h, float s0, float t0, float s1, float t1, float* color, Material* material)
{
	void(*R_AddCmdDrawStretchPic)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, float* color, Material* material) = (void(*)(float, float, float, float, float, float, float, float, float*, Material*))addr->R_AddCmdDrawStretchPic;
	R_AddCmdDrawStretchPic(x, y, w, h, s0, t0, s1, t1, color, material);
}

void Real_CG_DrawRotatedPicPhysical(int ScreenPlacement, float x, float y, float width, float height, float angle, float* color, Material* material)
{
	void(*CG_DrawRotatedPicPhysical)(int ScreenPlacement, float x, float y, float width, float height, float angle, float* color, Material* material) = (void(*)(int, float, float, float, float, float, float*, Material*))addr->CG_DrawRotatedPicPhysical;
	CG_DrawRotatedPicPhysical(ScreenPlacement, x, y, width, height, angle, color, material);
}

const char* Real_R_GetFontPathFromName(const char* name)
{
	static char Buffer[0x200];
	ZeroMemory(Buffer, sizeof(Buffer));
	_snprintf(Buffer, sizeof(Buffer), "fonts/%s", name);
	return Buffer;
}

int Real_R_RegisterFont(const char* name, int imageTrack)
{
	static int(*R_RegisterFont)(const char * name, int imageTrack) = (int(*)(const char*, int))addr->R_RegisterFont;
	return R_RegisterFont(Real_R_GetFontPathFromName(name), 0);
}

int Real_TextWidth(const char* text, const char* font)
{
	return R_TextWidth(text, 0x7FFFFFFF, Real_R_RegisterFont(font, 0));
}

int Real_TextHeight(const char* font, float scale)
{
	return (R_TextHeight(Real_R_RegisterFont(font, 0)) * scale);
}

bool Real_GetAsyncKeyState(DWORD KEY)
{
	XINPUT_STATE InputState;
	XInputGetState(0, &InputState);

	if (!KEY)
		return true;

	if (KEY == KEY_LT) {
		return (InputState.Gamepad.bLeftTrigger / 30) > 0;
	}

	if (KEY == KEY_RT)
		return (InputState.Gamepad.bRightTrigger / 30) > 0;

	return (InputState.Gamepad.wButtons & KEY);
}


bool Real_getStructures(int** cgs, int** cg, int** Entity, int** ClientActive_t, int** ClientNetwork, int** clientUIActive)
{
	*clientUIActive = (int*)addr->clientUIActive;
	*cgs = *(int**)addr->cgs_t;
	*cg = *(int**)addr->cg_s;
	*Entity = *(int**)addr->centity_s;
	*ClientActive_t = *(int**)addr->clientactive_t;
	*ClientNetwork = (int*)addr->ClientNetwork_s;

	if (!*(int*)addr->cgs_t || !*(int*)addr->cg_s || !*(int*)addr->centity_s || !*(int*)addr->clientactive_t) return false;
	return true;
}

const char* Real_SEH_StringEd_GetString(const char* str)
{
	const char*(*SEH_StringEd_GetString)(const char* str) = (const char*(*)(const char*))addr->SEH_StringEd_GetString;
	return SEH_StringEd_GetString(str);
}

char Real_ClampChar_r(const int i)
{
	if (i < -128) {
		return -128;
	}
	if (i > 127) {
		return 127;
	}
	return i;
}

float Real_DEG2RAD(float DEG)
{
	return ((DEG * 3.14159265358979323846f) / 180.0f);
}

int Real_Dvar_GetInt(const char* Dvar)
{
	int(*Dvar_GetInt)(const char* Dvar) = (int(*)(const char*))addr->Dvar_GetInt;
	return Dvar_GetInt(Dvar);
}

float Real_Dvar_GetFloat(const char* Dvar)
{
	float(*Dvar_GetFloat)(const char*) = (float(*)(const char*))addr->Dvar_GetFloat;
	return Dvar_GetFloat(Dvar);
}

const char* Real_Dvar_GetString(const char* Dvar)
{
	const char*(*Dvar_GetString)(const char*) = (const char*(*)(const char*))addr->Dvar_GetString;
	return Dvar_GetString(Dvar);
}

bool Real_Dvar_GetBool(const char* Dvar)
{
	bool(*Dvar_GetBool)(const char* Dvar) = (bool(*)(const char*))addr->Dvar_GetBool;
	return Dvar_GetBool(Dvar);
}

void Real_Dvar_SetBool(const char* Dvar, bool val)
{
	bool(*Dvar_SetBool)(int dvar, bool val) = (bool(*)(int, bool))addr->Dvar_SetBool;
	int pDvar = Real_Dvar_FindVar(Dvar);
	if (pDvar)
		Dvar_SetBool(pDvar, val);
}


void Real_Dvar_SetStringByName(const char* Dvar, const char*  val)
{
	bool(*Dvar_SetString)(int dvar, const char*  val) = (bool(*)(int, const char*))addr->Dvar_SetStringByName;
	int pDvar = Real_Dvar_FindVar(Dvar);
	if (pDvar)
		Dvar_SetString(pDvar, val);
}

int Real_Dvar_FindVar(const char* Dvar)
{
	int(*Dvar_FindVar)(const char* Dvar) = (int(*)(const char*))addr->Dvar_FindVar;
	return Dvar_FindVar(Dvar);
}

unsigned int Real_SL_GetStringOfSize(const char *str, int user, int length, int type)
{
	unsigned int(*SL_GetStringOfSize)(const char *str, unsigned int user, unsigned int length, int type) = (unsigned int(*)(const char *, unsigned int, unsigned int, int))addr->SL_GetStringOfSize;
	return SL_GetStringOfSize(str, user, length, type);
}

unsigned int Real_SL_GetString(const char* str, unsigned int user)
{
	return Real_SL_GetStringOfSize(str, user, strlen(str) + 1, 7);
}

const char* Real_SL_ConvertToString(unsigned int stringValue)
{
	const char*(*SL_ConvertToString)(unsigned int stringValue) = (const char*(*)(unsigned int stringValue))addr->SL_ConvertToString;
	return SL_ConvertToString(stringValue);
}


VOID Real_XNotify(CONST PWCHAR NotifyText)
{
	ExCreateThread(0, 0, 0, 0, (LPTHREAD_START_ROUTINE)XNotifyThread, NotifyText, 0);
}

int * Real_GetDetours()
{
	return (int*)new Detour();
}

void* Real_HookFunction(Detour* a, void* b, void* c)
{
	return (void*)a->HookFunction((unsigned int)b, (unsigned int)c);
}


void ApplyGamePatches()
{
	*(int*)(*(int*)(0x82B141E0) + 0xC) = 100; //cl_maxpackets
	int snaps = Dvar_FindVar("snaps");
	if (snaps != 0)
		*(int*)(snaps + 0xC) = 40; //snaps
	int rate = Dvar_FindVar("rate");
	if(rate != 0)
		*(int*)(rate + 0xC) = 25000; //rate

	//version number 
	//*(int*)(0x822ECB84) = addr->_0x390A018C; //0x390A0194

	//disable impacts
	*(int*)0x822C73C0 = 0x4E800020;
	*(int*)0x82385B38 = 0x4E800020;

	//Misc
	*(char*)(*(int*)(0x82ADE840) + 0x0C) = 0x01; //cg_scoreboardPingText

	//offensive clan tags
	//*(int*)(0x82309A30) = addr->_0x38600001;
	//*(int*)(0x82182C10) = addr->_0x38600001;
	//*(int*)(0x82182C14) = addr->_0x4E800020;
	//*(int*)(0x82309B84) = addr->_0x60000000;
	//*(int*)(0x82309B90) = addr->_0x60000000;
	//*(int*)(0x8217E8D8) = addr->_0x38600001;
	//*(int*)(0x8247C590) = addr->_0x38600001;
}

#endif