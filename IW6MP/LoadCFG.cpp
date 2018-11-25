#include "stdafx.h"
#include "LoadCFG.h"
#include "XIniRW.h"
#include "Menu.h"
#include "Functions.h"

#pragma region Congfig
bool createloadthread; bool createsavethread;

bool IsNumberdisBy(int number, int divisor) {
	return ((number % divisor) == 0);
}

void ReadMenu(int ID)
{
	for (int i = 0; i < Menu.MaxRows; i++)
	{
		if (Menu.SubType[ID][i] == SUB_BOOL || Menu.SubType[ID][i] == SUB_BOOLCALL)
			*Menu.BoolToggle[ID][i] = XIniRW.ReadBool(Menu.SubTitle[ID], Menu.SubText[ID][i]);
		else if (Menu.SubType[ID][i] == SUB_STRINGARRAY) {
			bool strfound;
			for (int j = 0; j < 20; j++)
			{
				char* ReadStr = XIniRW.ReadString(Menu.SubTitle[ID], Menu.SubText[ID][i]);
				if (Menu.StrArray[ID][i][j] != NULL && ReadStr != NULL && strstr(Menu.StrArray[ID][i][j], ReadStr)) {
					*Menu.StrArrayScroll[ID][i] = j;
					strfound = true;
					break;
				}
			}
			if (!strfound) {
				printf("[%s][%s] Could not match string (%s) to valid menu option.\n", CheatName, Menu.SubText[ID][i], XIniRW.ReadString(Menu.SubTitle[ID], Menu.SubText[ID][i]));
				continue;
			}
		}
		else if ((Menu.SubType[ID][i] == SUB_FLOAT) || (Menu.SubType[ID][i] == SUB_FLOATPROGRESS)) {
			float temp = XIniRW.ReadFloat(Menu.SubTitle[ID], Menu.SubText[ID][i]);
			if ((temp != 0) && (Menu.FloatAmmount[ID][i] > 0.0f))
			{
				float div = fmod(temp, Menu.FloatAmmount[ID][i]);
				if ((temp <= Menu.FloatMax[ID][i]) && (temp >= Menu.FloatMin[ID][i]) && (float_equals(div, 0.0f) || float_equals(div, Menu.FloatAmmount[ID][i])))
					*Menu.FloatPointer[ID][i] = temp;
				else {
					printf("[%s][%s] float (%f) was out side the bounds (less than %f, greater than %f, and divisible by %f) %f.\n", CheatName, Menu.SubText[ID][i], temp, Menu.FloatMax[ID][i], Menu.FloatMin[ID][i], Menu.FloatAmmount[ID][i], fmod(temp, Menu.FloatAmmount[ID][i]));
					continue;
				}
			}
		}
		else if (Menu.SubType[ID][i] == SUB_INTCALL || Menu.SubType[ID][i] == SUB_INT || Menu.SubType[ID][i] == SUB_INTPROGRESS || Menu.SubType[ID][i] == SUB_INTPROGRESSMS) {
			int temp = XIniRW.ReadInt(Menu.SubTitle[ID], Menu.SubText[ID][i]);
			if ((temp <= Menu.IntMax[ID][i]) && (temp >= Menu.IntMin[ID][i]) && IsNumberdisBy(temp, Menu.IntAmmount[ID][i]))
				*Menu.IntPointer[ID][i] = temp;
			else {
				printf("[%s][%s] float (%i) was out side the bounds (less than %i, greater than %i, and divisible by %i).\n", CheatName, Menu.SubText[ID][i], temp, Menu.IntMax[ID][i], Menu.IntMin[ID][i], Menu.IntAmmount[ID][i]);
				continue;
			}
		}
		else if (Menu.SubType[ID][i] == SUB_EMPTY)
			break;
	}
}

void LoadConfig()
{
	if (XIniRW.ROpenINI("xbOnline:\\xbOnline\\IW6MP.cfg"))
	{
		ReadMenu(Menu.MenuIDs.Main);
		ReadMenu(Menu.MenuIDs.Aimbot);
		ReadMenu(Menu.MenuIDs.Visual);
		ReadMenu(Menu.MenuIDs.RadarSettings);
		ReadMenu(Menu.MenuIDs.ESPSettings);
		ReadMenu(Menu.MenuIDs.Settings);
		ReadMenu(Menu.MenuIDs.DebugSettings);
		ReadMenu(Menu.MenuIDs.NameMenu);
		ReadMenu(Menu.MenuIDs.HVH);

		CG_GameMessage(0, "Config Loaded!\n");
		XNotify(toWCHAR("%s - Config Loaded!", CheatName));

		XIniRW.RCloseINI();
	}
}

DWORD WINAPI DoLoadCFG()
{
	LoadConfig();

	DWORD Thr_Exit = 0;
	ExitThread(Thr_Exit);
}

void LoadCFG(bool Direct)
{
	if (Direct)
		LoadConfig();
	else
		createloadthread = true;
}

void WriteMenu(int ID)
{
	if (ID != Menu.MenuIDs.Main || ID != Menu.MenuIDs.MenuSettings)
		XIniRW.AddComment(" ");
	XIniRW.CreateGroup(Menu.SubTitle[ID]);

	for (int i = 0; i < Menu.MaxRows; i++)
	{
		if (Menu.SubType[ID][i] == SUB_BOOL || Menu.SubType[ID][i] == SUB_BOOLCALL)
			XIniRW.WriteBool(Menu.SubText[ID][i], *Menu.BoolToggle[ID][i]);
		else if (Menu.SubType[ID][i] == SUB_STRINGARRAY)
			XIniRW.WriteString(Menu.SubText[ID][i], Menu.StrArray[ID][i][*Menu.StrArrayScroll[ID][i]]);
		else if (Menu.SubType[ID][i] == SUB_FLOAT || Menu.SubType[ID][i] == SUB_FLOATPROGRESS)
			XIniRW.WriteFloat(Menu.SubText[ID][i], *Menu.FloatPointer[ID][i]);
		else if (Menu.SubType[ID][i] == SUB_INTCALL || Menu.SubType[ID][i] == SUB_INT || Menu.SubType[ID][i] == SUB_INTPROGRESS || Menu.SubType[ID][i] == SUB_INTPROGRESSMS)
			XIniRW.WriteInt(Menu.SubText[ID][i], *Menu.IntPointer[ID][i]);
	}
}

void WriteConfig()
{
	if (XIniRW.WOpenINI("xbOnline:\\xbOnline\\IW6MP.cfg")) {

		WriteMenu(Menu.MenuIDs.Main);
		WriteMenu(Menu.MenuIDs.Aimbot);
		WriteMenu(Menu.MenuIDs.Visual);
		WriteMenu(Menu.MenuIDs.RadarSettings);
		WriteMenu(Menu.MenuIDs.ESPSettings);
		WriteMenu(Menu.MenuIDs.Settings);
		WriteMenu(Menu.MenuIDs.DebugSettings);
		WriteMenu(Menu.MenuIDs.NameMenu);
		WriteMenu(Menu.MenuIDs.HVH);

		CG_GameMessage(0, "Config Saved!\n");
		XNotify(toWCHAR("%s - Config Saved!", CheatName));

		XIniRW.WCloseINI();
	}
}

DWORD WINAPI DoWriteCFG()
{
	WriteConfig();

	DWORD Thr_Exit = 0;
	ExitThread(Thr_Exit);
}

void WriteCFG(bool Direct)
{
	if (Direct)
		WriteConfig();
	else
		createsavethread = true;
}
#pragma endregion

#pragma region Player CFG
SavedPlayers_s* SavedPlayers;
bool createpsavethread, createploadthread;

void WritePlayerToCFG(int index, char* Gamertag, unsigned char* XUID, bool Prioritized, bool Ignored, bool AimAtRoot)
{
	char NameIndex[0x200];
	_snprintf(NameIndex, sizeof(NameIndex), "Saved Player %i", index);
	XIniRW.CreateGroup(NameIndex);

	char xuid[16];
	CharArrayToString(xuid, XUID, 8);

	XIniRW.WriteString("Gamertag", Gamertag);
	XIniRW.WriteString("XUID", xuid);
	XIniRW.WriteBool("Prioritized", Prioritized);
	XIniRW.WriteBool("Ignored", Ignored);
	XIniRW.WriteBool("AimAtRoot", AimAtRoot);
}

void _WritePlayersCFG()
{
	if (SavedPlayers && XIniRW.WOpenINI("xbOnline:\\xbOnline\\SavedPlayers.cfg"))
	{
		XIniRW.AddComment(";  !!! DO NOT MODIFY THIS FILE !!!");
		XIniRW.CreateGroup("Saved Players");
		XIniRW.WriteInt("Count", SavedPlayers->Count);

		for (int i = 0; i < SavedPlayers->Count; i++)
			WritePlayerToCFG(i, SavedPlayers->PlayerInfo[i].Gamertag, SavedPlayers->PlayerInfo[i].XUID, SavedPlayers->PlayerInfo[i].Prioritized, SavedPlayers->PlayerInfo[i].Ignored, SavedPlayers->PlayerInfo[i].AimAtRoot);

		XIniRW.WCloseINI();
	}
}

DWORD WINAPI DoWritePlayersCFG()
{
	_WritePlayersCFG();

	DWORD Thr_Exit = 0;
	ExitThread(Thr_Exit);
}

void WritePlayersCFG(bool Direct)
{
	if (Direct)
		_WritePlayersCFG();
	else
		createpsavethread = true;
}

int char2int(char input)
{
	if (input >= '0' && input <= '9')
		return input - '0';
	if (input >= 'A' && input <= 'F')
		return input - 'A' + 10;
	if (input >= 'a' && input <= 'f')
		return input - 'a' + 10;
	throw std::invalid_argument("Invalid input string");
}

void hex2bin(const char* src, char* target)
{
	while (*src && src[1])
	{
		*(target++) = char2int(*src) * 16 + char2int(src[1]);
		src += 2;
	}
}
void ReadPlayerFromCFG(int index)
{
	char NameIndex[0x200];
	_snprintf(NameIndex, sizeof(NameIndex), "Saved Player %i", index);

	strcpy(SavedPlayers->PlayerInfo[index].Gamertag, XIniRW.ReadString(NameIndex, "Gamertag"));

	const char* xuid = XIniRW.ReadString(NameIndex, "XUID");
	char pxuid[16];
	hex2bin(xuid, (char*)pxuid);

	memcpy(SavedPlayers->PlayerInfo[index].XUID, pxuid, 8);

	SavedPlayers->PlayerInfo[index].Prioritized = XIniRW.ReadBool(NameIndex, "Prioritized");
	SavedPlayers->PlayerInfo[index].Ignored = XIniRW.ReadBool(NameIndex, "Ignored");
	SavedPlayers->PlayerInfo[index].AimAtRoot = XIniRW.ReadBool(NameIndex, "AimAtRoot");
}

void _ReadPlayersCFG()
{
	MemoryBuffer Buf;

	if (CReadFile("xbOnline:\\xbOnline\\SavedPlayers.cfg", Buf))
	{
		std::string cfg = (const char*)Buf.GetData();

		if (cfg.find("DO NOT MODIFY THIS FILE") == -1)
			remove("xbOnline:\\xbOnline\\SavedPlayers.cfg");
	}

	if (SavedPlayers)
	{
		SavedPlayers->Count = 0;
		if (XIniRW.ROpenINI("xbOnline:\\xbOnline\\SavedPlayers.cfg"))
		{
			SavedPlayers->Count = XIniRW.ReadInt("Saved Players", "Count");

			for (int i = 0; i < SavedPlayers->Count; i++)
				ReadPlayerFromCFG(i);

			XIniRW.RCloseINI();
		}
	}
}

DWORD WINAPI DoLoadPlayersCFG()
{
	_ReadPlayersCFG();

	DWORD Thr_Exit = 0;
	ExitThread(Thr_Exit);
}

void LoadPlayersCFG(bool Direct)
{
	if (Direct)
		_ReadPlayersCFG();
	else
		createploadthread = true;
}
#pragma endregion

#pragma region Menu Settings
bool CreateMenuSettingsWriteThread, CreateMenuSettingsLoadThread;

void WriteRGBA(RGBA rgb, const char * Name)
{
	XIniRW.AddComment(" ");
	XIniRW.CreateGroup(Name);

	XIniRW.WriteFloat("Red", rgb.R);
	XIniRW.WriteFloat("Green", rgb.G);
	XIniRW.WriteFloat("Blue", rgb.B);
	XIniRW.WriteFloat("Alpha", rgb.A);
	XIniRW.WriteBool("Rainbow", rgb.Rainbow);
}

void _WriteMenuSettingsCFG()
{
	if (XIniRW.WOpenINI("HDD:\\xbOnline\\MenuSettings.cfg")) {
		WriteMenu(Menu.MenuIDs.MenuSettings);

		//Menu
		WriteRGBA(CE.MenuPrimaryColour, "MenuPrimaryColour");
		WriteRGBA(CE.MenuSecondaryColour, "MenuSecondaryColour");
		WriteRGBA(CE.TextColour, "TextColour");

		//ESP
		WriteRGBA(CE.AimbotTargetColor, "AimbotTargetColor");
		WriteRGBA(CE.EnemyColor, "EnemyColor");
		WriteRGBA(CE.PriorityEnemyColor, "PriorityEnemyColor");
		WriteRGBA(CE.FriendlyColor, "FriendlyColor");
		WriteRGBA(CE.KillableColor, "KillableColor");
		WriteRGBA(CE.VisibleColor, "VisibleColor");
		WriteRGBA(CE.BonesColor, "BonesColor");

		CG_GameMessage(0, "Menu Settings Saved!\n");
		XNotify(toWCHAR("%s - Menu Settings Saved!", CheatName));

		XIniRW.WCloseINI();
	}
}

DWORD WINAPI DoWriteMenuSettingsCFG()
{
	_WriteMenuSettingsCFG();

	DWORD Thr_Exit = 0;
	ExitThread(Thr_Exit);
}

void WriteMenuSettingsCFG(bool Direct)
{
	if (Direct)
		_WriteMenuSettingsCFG();
	else
		CreateMenuSettingsWriteThread = true;
}

void CheckFloat(float* value, float min, float max)
{
	if (*value > max)
		*value = 1.0f;
	if (min > *value)
		*value = 0.0f;
}

RGBA ReturnValue;
RGBA GetRGBAFromCFG(const char * Name)
{
	ReturnValue.R = XIniRW.ReadFloat(Name, "Red");
	ReturnValue.G = XIniRW.ReadFloat(Name, "Green");
	ReturnValue.B = XIniRW.ReadFloat(Name, "Blue");
	ReturnValue.A = XIniRW.ReadFloat(Name, "Alpha");
	ReturnValue.Rainbow = XIniRW.ReadBool(Name, "Rainbow");
	if (ReturnValue.Rainbow)
		ReturnValue.ToggleRainbow();

	CheckFloat(&ReturnValue.R, 0.0f, 1.0f);
	CheckFloat(&ReturnValue.G, 0.0f, 1.0f);
	CheckFloat(&ReturnValue.B, 0.0f, 1.0f);
	CheckFloat(&ReturnValue.A, 0.0f, 1.0f);

	return ReturnValue;
}

void _LoadMenuSettingsCFG(bool Direct)
{
	if (XIniRW.ROpenINI("xbOnline:\\xbOnline\\MenuSettings.cfg"))
	{
		ReadMenu(Menu.MenuIDs.MenuSettings);

		//Menu
		CE.MenuPrimaryColour = GetRGBAFromCFG("MenuPrimaryColour");
		CE.MenuSecondaryColour = GetRGBAFromCFG("MenuSecondaryColour");
		CE.TextColour = GetRGBAFromCFG("TextColour");

		//ESP
		CE.AimbotTargetColor = GetRGBAFromCFG("AimbotTargetColor");
		CE.EnemyColor = GetRGBAFromCFG("EnemyColor");
		CE.PriorityEnemyColor = GetRGBAFromCFG("PriorityEnemyColor");
		CE.FriendlyColor = GetRGBAFromCFG("FriendlyColor");
		CE.KillableColor = GetRGBAFromCFG("KillableColor");
		CE.VisibleColor = GetRGBAFromCFG("VisibleColor");
		CE.BonesColor = GetRGBAFromCFG("BonesColor");

		if (!Direct)
		{
			CG_GameMessage(0, "Menu Settings Loaded!\n");
			XNotify(toWCHAR("%s - Menu Settings Loaded!", CheatName));
		}

		XIniRW.RCloseINI();
	}
}

DWORD WINAPI DoLoadMenuSettingsCFG()
{
	_LoadMenuSettingsCFG(false);

	DWORD Thr_Exit = 0;
	ExitThread(Thr_Exit);
}

void LoadMenuSettingsCFG(bool Direct)
{
	if (Direct)
		_LoadMenuSettingsCFG(Direct);
	else
		CreateMenuSettingsLoadThread = true;
}
#pragma endregion

#pragma region User Gamertags 

bool CreateLoadUserGamertagsThread, CreateSaveUserGamertagsThread;
void LoadUserSetGTs()
{
	fstream fs;
	int line = 0;
	string szLine;

	if (fs.is_open())
		goto CloseFs;

	fs = fstream("xbOnline:\\xbOnline\\UserGamertags.txt", std::ios::in);

	if (!fs.is_open())
		return;

	while (getline(fs, szLine))
	{
		line++;
		if (line > (Menu.MaxRows - 2))
			goto CloseFs;

		if (cuser_strcmp(szLine.c_str(), ""))
			break;

		//printf("Names[%i] = %s\n", line - 1, szLine.c_str());
		user_strcpy(CE.UserSetGTs[line - 1], szLine.c_str());
	}

CloseFs:
	fs.close();

	if (fs.is_open())
		goto CloseFs;
}

DWORD WINAPI LoadUserGamertagsThread()
{
	LoadUserSetGTs();
	XNotify(L"xbOnline - User Gamertags Loaded!");

	DWORD Thr_Exit = 0;
	ExitThread(Thr_Exit);
}

void LoadUserGamertags(bool Direct)
{
	if (Direct)
		LoadUserSetGTs();
	else
		CreateLoadUserGamertagsThread = true;
}

void SaveUserSetGTs()
{
	ofstream fs;
	const char* fileName = "xbOnline:\\xbOnline\\UserGamertags.txt";
	if (fs.is_open()) {
#if defined(DEVKIT)
		printf("Failed to open File (%s) - File already open\n", fileName);
#endif
		goto Close_Fs;
	}

	fs = ofstream(fileName, ofstream::out | ofstream::trunc);

	if (!fs.is_open()) {
#if defined(DEVKIT)
		printf("Failed to open File (%s)\n", fileName);
#endif
		return;
	}

	for (int i = 0; i < (Menu.MaxRows - 2); i++)
	{
		if (!fs.is_open())
			return;

		char Buffer[34];
		_snprintf(Buffer, sizeof(Buffer), "%s\n", CE.UserSetGTs[i]);

		fs.write(Buffer, strlen(Buffer));
	}

Close_Fs:
	if (!fs.is_open())
		return;

	fs.close();
}

DWORD WINAPI SaveUserGamertagsThread()
{
	SaveUserSetGTs();
	XNotify(L"xbOnline - User Gamertags Saved!");

	DWORD Thr_Exit = 0;
	ExitThread(Thr_Exit);
}

void SaveUserGamertags(bool Direct)
{
	if (Direct)
		SaveUserSetGTs();
	else
		CreateSaveUserGamertagsThread = true;
}

#pragma endregion

#pragma region Looper Gamertags 

bool CreateLoadLooperGamertagsThread, CreateSaveLooperGamertagsThread;
void LoadLooperGTs()
{
	fstream fs;
	int line = 0;
	string szLine;

	if (fs.is_open())
		goto CloseFs;

	fs = fstream("xbOnline:\\xbOnline\\LooperGamertags.txt", std::ios::in);

	if (!fs.is_open())
		return;

	while (getline(fs, szLine))
	{
		line++;
		if (line > 10)
			goto CloseFs;

		if (cuser_strcmp(szLine.c_str(), ""))
			break;

		//printf("Names[%i] = %s\n", line - 1, szLine.c_str());
		user_strcpy(CE.GamertagLooperList[line - 1], szLine.c_str());
	}

CloseFs:
	fs.close();

	if (fs.is_open())
		goto CloseFs;
}

DWORD WINAPI LoadLooperGamertagsThread()
{
	LoadLooperGTs();
	XNotify(L"xbOnline - Looper Gamertags Loaded!");

	DWORD Thr_Exit = 0;
	ExitThread(Thr_Exit);
}

void LoadLooperGamertags(bool Direct)
{
	if (Direct)
		LoadLooperGTs();
	else
		CreateLoadLooperGamertagsThread = true;
}

void SaveLooperGTs()
{
	ofstream fs;
	const char* fileName = "xbOnline:\\xbOnline\\LooperGamertags.txt";
	if (fs.is_open()) {
#if defined(DEVKIT)
		printf("Failed to open File (%s) - File already open\n", fileName);
#endif
		goto Close_Fs;
	}

	fs = ofstream(fileName, ofstream::out | ofstream::trunc);

	if (!fs.is_open()) {
#if defined(DEVKIT)
		printf("Failed to open File (%s)\n", fileName);
#endif
		return;
	}

	for (int i = 0; i < 10; i++)
	{
		if (!fs.is_open())
			return;

		char Buffer[34];
		_snprintf(Buffer, sizeof(Buffer), "%s\n", CE.GamertagLooperList[i]);

		fs.write(Buffer, strlen(Buffer));
	}

Close_Fs:
	if (!fs.is_open())
		return;

	fs.close();
}

DWORD WINAPI SaveLooperGamertagsThread()
{
	SaveLooperGTs();
	XNotify(L"xbOnline - Looper Gamertags Saved!");

	DWORD Thr_Exit = 0;
	ExitThread(Thr_Exit);
}

void SaveLooperGamertags(bool Direct)
{
	if (Direct)
		SaveLooperGTs();
	else
		CreateSaveLooperGamertagsThread = true;
}

#pragma endregion

void MonitorCFGThread()
{
#pragma region Config
	if (createloadthread)
	{
		HANDLE hThread; DWORD hThreadID;
		ExCreateThread(&hThread, 0x4, &hThreadID, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)DoLoadCFG, NULL, 0x2);
		ResumeThread(hThread);
		CloseHandle(hThread);
		createloadthread = false;
	}
	if (createsavethread)
	{
		HANDLE hThread; DWORD hThreadID;
		ExCreateThread(&hThread, 0x4, &hThreadID, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)DoWriteCFG, NULL, 0x2);
		ResumeThread(hThread);
		CloseHandle(hThread);
		createsavethread = false;
	}
#pragma endregion

#pragma region Player Settings CFG
	if (createploadthread)
	{
		HANDLE hThread; DWORD hThreadID;
		ExCreateThread(&hThread, 0x4, &hThreadID, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)DoLoadPlayersCFG, NULL, 0x2);
		ResumeThread(hThread);
		CloseHandle(hThread);
		createploadthread = false;
	}
	if (createpsavethread)
	{
		HANDLE hThread; DWORD hThreadID;
		ExCreateThread(&hThread, 0x4, &hThreadID, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)DoWritePlayersCFG, NULL, 0x2);
		ResumeThread(hThread);
		CloseHandle(hThread);
		createpsavethread = false;
	}
#pragma endregion

#pragma region Menu Settings CFG
	if (CreateMenuSettingsLoadThread)
	{
		HANDLE hThread; DWORD hThreadID;
		ExCreateThread(&hThread, 0x4, &hThreadID, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)DoLoadMenuSettingsCFG, NULL, 0x2);
		ResumeThread(hThread);
		CloseHandle(hThread);
		CreateMenuSettingsLoadThread = false;
	}
	if (CreateMenuSettingsWriteThread)
	{
		HANDLE hThread; DWORD hThreadID;
		ExCreateThread(&hThread, 0x4, &hThreadID, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)DoWriteMenuSettingsCFG, NULL, 0x2);
		ResumeThread(hThread);
		CloseHandle(hThread);
		CreateMenuSettingsWriteThread = false;
	}
#pragma endregion

#pragma region User Gamertags
	if (CreateLoadUserGamertagsThread)
	{
		HANDLE hThread; DWORD hThreadID;
		ExCreateThread(&hThread, 0x4, &hThreadID, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)LoadUserGamertagsThread, NULL, 0x2);
		ResumeThread(hThread);
		CloseHandle(hThread);
		CreateLoadUserGamertagsThread = false;
	}
	if (CreateSaveUserGamertagsThread)
	{
		HANDLE hThread; DWORD hThreadID;
		ExCreateThread(&hThread, 0x4, &hThreadID, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)SaveUserGamertagsThread, NULL, 0x2);
		ResumeThread(hThread);
		CloseHandle(hThread);
		CreateSaveUserGamertagsThread = false;
	}
#pragma endregion

#pragma region Looper Gamertags
	if (CreateLoadLooperGamertagsThread)
	{
		HANDLE hThread; DWORD hThreadID;
		ExCreateThread(&hThread, 0x4, &hThreadID, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)LoadLooperGamertagsThread, NULL, 0x2);
		ResumeThread(hThread);
		CloseHandle(hThread);
		CreateLoadLooperGamertagsThread = false;
	}
	if (CreateSaveLooperGamertagsThread)
	{
		HANDLE hThread; DWORD hThreadID;
		ExCreateThread(&hThread, 0x4, &hThreadID, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)SaveLooperGamertagsThread, NULL, 0x2);
		ResumeThread(hThread);
		CloseHandle(hThread);
		CreateSaveLooperGamertagsThread = false;
	}
#pragma endregion

}