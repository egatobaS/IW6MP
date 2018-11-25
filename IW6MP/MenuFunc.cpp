#include "stdafx.h"
#include "MenuFunc.h"
#include "Functions.h"
#include "Menu.h"
#include "LoadCFG.h"

const char* TeamNumToName(int team)
{
	if (team == 1)
		return "axis";
	if (team == 2)
		return "allies";
	else
		return "spectator";
}

void ChangeTeam()
{
	if (ShouldHookRun())
	{
		if (cg->characterinfo[cg->ClientNumber].Team == 2)
			CL_AddReliableCommand(0, "mr %i 3 %s", *(int*)(addr->SERVER_ID), TeamNumToName(1));
		else
			CL_AddReliableCommand(0, "mr %i 3 %s", *(int*)(addr->SERVER_ID), TeamNumToName(2));
	}
}

void ChangeClass()
{
	CL_AddReliableCommand(0, "mr %i 10 custom%i", *(int*)(addr->SERVER_ID), CE.ClassNum);
	CG_GameMessage(0, va("Custom Class %i set!", CE.ClassNum));
}

void ConsoleCommand()
{
	ThreadKeyBoardAction((int)ConsoleCMD, L"", L"Console Command", L"Enter a console command ie. cg_fov 90", 999);
}

void Changeclantag()
{
	const char *Previousclantag = (char*)addr->ClantagAddress;

	size_t size = strlen(Previousclantag) + 1;
	wchar_t* DefaultVal = new wchar_t[size];

	size_t outSize;
	mbstowcs_s(&outSize, DefaultVal, size, Previousclantag, size - 1);

	ThreadKeyBoardAction((int)SetClantag, (LPWSTR)DefaultVal, L"Change Clantag", L"Enter a new Clan Tag ie. FUCK", 9);

	free(DefaultVal);
}

void ChangeGamertag()
{
	const char *PreviousName = (char*)addr->NameAddress;
	if (PreviousName)
	{
		size_t size = strlen(PreviousName) + 1;
		wchar_t* DefaultVal = new wchar_t[size];

		size_t outSize = 0;
		mbstowcs_s(&outSize, DefaultVal, size, PreviousName, size - 1);

		ThreadKeyBoardAction((int)SetName, (LPWSTR)DefaultVal, L"Change Name", L"Enter a new name ie. DankHacker1337", 33);

		free(DefaultVal);
	}
}

void ResetGamertag()
{
	if (CE.OriginalName != NULL)
		SetName(CE.OriginalName);
}

int TryCounter = 0;
void UnInfector()
{
	if (ShouldHookRun())
	{
		if (cuser_strcmp(UI_GetGameTypeName(Dvar_GetString("ui_gametype")), "Infected") && (cg->characterinfo[cg->ClientNumber].Team == 1) && CE.AutoUnInfect && TryCounter <= 10)
		{
			Sleep(200);
			ChangeClass();
			Sleep(50);
			CL_AddReliableCommand(0, "mr %i 3 %s", *(int*)(addr->SERVER_ID), TeamNumToName(2));
			Sleep(300);
			TryCounter++;
		}

		if ((cg->characterinfo[cg->ClientNumber].Team == 2))
			TryCounter = 0;
	}
}

void DoJuggWork()
{
	int Team = cg->characterinfo[cg->ClientNumber].Team;
	int ServerID = *(int*)(addr->SERVER_ID);

	CL_AddReliableCommand(0, "mr %i 3 spectator", ServerID);
	Sleep(100);
	CL_AddReliableCommand(0, "mr %i 3 %s", ServerID, TeamNumToName(Team));
	Sleep(100);
	CL_AddReliableCommand(0, "mr %i 10 axis", ServerID);
	Sleep(100);
	ChangeClass();

	CG_GameMessage(0, "^2Juggernaut Given");
	DWORD Thr_Exit = 0;
	ExitThread(Thr_Exit);
}

void JugClass()
{
	if (ShouldHookRun())
	{
		DWORD hThreadID;
		HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DoJuggWork, 0, 3, &hThreadID);
		XSetThreadProcessor(hThread, 1);
		ResumeThread(hThread);
		CloseHandle(hThread);
	}
}

void EndGame()
{
	Cbuf_AddText(0, "cmd mr %i -1 endround;", *(int*)(addr->SERVER_ID));
	CG_GameMessage(0, "^1Game Ended");
}

void LeaveGame()
{
	Cbuf_AddText(0, "disconnect\n");
}

void ModLooper()
{
	//Checks
	WeaponDef* wp = BG_GetWeaponDef(cg->ps.Weapon);
	CE.DelayTimeFix = (((wp->fireType != WEAPON_FIRETYPE_FULLAUTO) && (CE.ShootDelay == 0)) ? 50 : CE.ShootDelay);
	CE.ShouldRunXAM_191 = ShouldHookRun();

	//Mods
	*(float*)(*(int*)(0x82ACCADC) + 0xC) = CE.Fov;
	Dvar_SetBool("camera_thirdPerson", CE.ThirdPerson);
	*(int*)(0x822583D0) = CE.NoSway ? 0x60000000 : 0x4BFFEAA1;
	*(int*)(0x82258BEC) = CE.NoSway ? 0x60000000 : 0x4BFFFBA5;
	*(int*)(0x82258BE0) = CE.NoFlinch ? 0x60000000 : 0x4BFFDED9;
	*(int*)(0x82258BF8) = CE.NoFlinch ? 0x60000000 : 0x4BFFFD49;
	*(int*)(0x82258C04) = CE.NoFlinch ? 0x60000000 : 0x4BFFFEC5;
	*(int*)0x822C9344 = CE.NoRecoil ? 0x60000000 : 0x4BF8AE4D;

	*(int*)0x822C73C0 = CE.Impacts ? 0x7D8802A6 : 0x4E800020;
	*(int*)0x82385B38 = CE.Impacts ? 0x7D8802A6 : 0x4E800020;

	*(int*)(0x822654C8) = CE.UAV ? 0x3B600001 : 0x551B7FFE;
	*(char*)(0x8227F19B) = CE.Laser ? 0x01 : 0x00;

	//Remove Game Radar
	bool ShouldDisableRadar = (CE.ExternalRadar && (clientUIActive->OpenMenu != 0x40) && !CG_IsSelectingLocation(0));
	*(int*)(0x8220D778) = ShouldDisableRadar ? 0x4E800020 : 0x7D8802A6; //LUIElement_CompassRender
	*(int*)(0x8220DA60) = ShouldDisableRadar ? 0x4E800020 : 0x7D8802A6; //LUIElement_MinimapElementsRender
	*(int*)(0x8220D8B0) = ShouldDisableRadar ? 0x4E800020 : 0x7D8802A6; //LUIElement_MinimapRender
}

int Looperwait, LooperTick;
void DoLooperWait(int time) {
	LooperTick = GetTickCount();
	Looperwait = time;
}

int Rainbowwait, RainbowTick;
void DoRainbowWait(int time) {
	RainbowTick = GetTickCount();
	Rainbowwait = time;
}

void StoreName()
{
	char Buffer[0x100];
	_snprintf(Buffer, sizeof(Buffer), "%s%s", CE.ColourSet, CE.SetGamertag);
	strcpy((char*)addr->NameAddress, Buffer);
}

//TODO: Improve this
const char* NameColors[] = { "^0", "^1", "^2", "^3", "^4", "^5", "^6" };
void FlashingNameThread(LPVOID r3)
{
	static bool HasStartedGamerTagLooper = false;
	int LooperCount = 0, RainbowCount = 0;
	while ((CE.RainbowGamertag || CE.GamertagLooper) && !KillThread)
	{
		if (!HasStartedGamerTagLooper) {
			strcpy(CE.BeforeName, (char*)(addr->NameAddress));
			HasStartedGamerTagLooper = true;
		}

		if (CE.GamertagLooper)
		{
			if ((GetTickCount() - LooperTick) > Looperwait)
			{
				LooperCount++;
				if (LooperCount >= 9)
					LooperCount = 0;

				if (!cuser_strcmp(CE.GamertagLooperList[LooperCount], "")) {
					user_strcpy(CE.SetGamertag, CE.GamertagLooperList[LooperCount]);
					DoLooperWait(CE.GamertagLooperSpeed);

					if (!CE.RainbowGamertag)
						SetNameColour((char*)"", CE.SetGamertag);
				}
			}
		}

		if (CE.RainbowGamertag) {
			if ((GetTickCount() - RainbowTick) > Rainbowwait)
			{
				RainbowCount++;
				if (RainbowCount >= 7)
					RainbowCount = 0;

				SetNameColour((char*)NameColors[RainbowCount], CE.SetGamertag);
				DoRainbowWait(CE.RainbowLooperSpeed);
			}
		}

		Sleep(10);
	}

	if (HasStartedGamerTagLooper)
		SetName(CE.BeforeName);

	HasStartedGamerTagLooper = false;

	DWORD Thr_Exit;
	ExitThread(Thr_Exit);
}

void GamertagLooper()
{
	if (!CE.RainbowGamertag && CE.GamertagLooper)
	{
		DWORD hThreadID;
		HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)FlashingNameThread, NULL, 3, &hThreadID);
		XSetThreadProcessor(hThread, 1);
		ResumeThread(hThread);
		CloseHandle(hThread);
	}
}

void FlashingGmaertag()
{
	if (!CE.GamertagLooper && CE.RainbowGamertag)
	{
		DWORD hThreadID;
		HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)FlashingNameThread, NULL, 3, &hThreadID);
		XSetThreadProcessor(hThread, 1);
		ResumeThread(hThread);
		CloseHandle(hThread);
	}
}

void SetNameAsIP()
{
	SetName(va("%u.%u.%u.%u", ClientNetwork->Net[Menu.CurrentScroll[Menu.MenuIDs.Players]].IP[0], ClientNetwork->Net[Menu.CurrentScroll[Menu.MenuIDs.Players]].IP[1], ClientNetwork->Net[Menu.CurrentScroll[Menu.MenuIDs.Players]].IP[2], ClientNetwork->Net[Menu.CurrentScroll[Menu.MenuIDs.Players]].IP[3]));
}

void StealName()
{
	SetName(ClientNetwork->Net[Menu.CurrentScroll[Menu.MenuIDs.Players]].gamertag);
}

bool IsDead; int OnSpawnDelayTime;
void OnPlayerSpawn()
{
	if ((cg->ps.Health == 0)) {
		IsDead = true;
		OnSpawnDelayTime = GetTickCount();
	}

	if ((cg->ps.Health > 0) && (cg->ps.clientNum == cg->ClientNumber) && IsDead && (GetTickCount() - OnSpawnDelayTime > 600))
	{
		ClientActive_t->viewAngle.y += 10;
		if (CE.SpawnMessage)
		{
			CG_GameMessage(0, "Welcome to the ^1%s^7 %s!", CheatName, ClientNetwork->Net[cg->ClientNumber].gamertag);
			CG_GameMessage(0, "Press [{+actionslot 1}] and [{+melee}] to open the menu");
		}
		IsDead = false;
	}
}

int StoredHealth, CrouchDelayTime; bool Crouched;
void DoCrouchBot()
{
	if ((cg->ps.clientNum == cg->ClientNumber) && !GetAsyncKeyState(XINPUT_GAMEPAD_LEFT_THUMB))
	{
		if (cg->ps.Health < StoredHealth)
		{
			if (CE.CrouchOnDamage) {
				Crouched = true;
				ClientActive_t->stance = CL_STANCE_CROUCH;
			}
		}
		else
		{
			if (GetTickCount() - CrouchDelayTime > CE.CrouchDelay)
			{
				if (CE.AutoCrouch == Constant)
				{
					if (ClientActive_t->stance == CL_STANCE_STAND) {
						Crouched = true;
						ClientActive_t->stance = CL_STANCE_CROUCH;
					}
					else
						ClientActive_t->stance = CL_STANCE_STAND;
					CrouchDelayTime = GetTickCount();;
				}
				else if ((CE.AimbotTarget != -1) && ((CE.AutoCrouch == ConstantVis && CE.CanSeePlayer[CE.AimbotTarget]) || (CE.AutoCrouch == ConstantKillable && (CE.IsKillable[CE.AimbotTarget] || CE.CanSeePlayer[CE.AimbotTarget]))))
				{
					if (ClientActive_t->stance == CL_STANCE_STAND) {
						Crouched = true;
						ClientActive_t->stance = CL_STANCE_CROUCH;
					}
					else
						ClientActive_t->stance = CL_STANCE_STAND;
					CrouchDelayTime = GetTickCount();;
				}
				else if ((CE.AimbotTarget != -1) && ((CE.AutoCrouch == OnVis && CE.CanSeePlayer[CE.AimbotTarget]) || (CE.AutoCrouch == OnKillable && (CE.IsKillable[CE.AimbotTarget] || CE.CanSeePlayer[CE.AimbotTarget]))))
				{
					if (ClientActive_t->stance == CL_STANCE_STAND) {
						Crouched = true;
						ClientActive_t->stance = CL_STANCE_CROUCH;
					}
					CrouchDelayTime = 0;
				}
				else {
					if (Crouched) {
						Crouched = false;
						ClientActive_t->stance = CL_STANCE_STAND;
					}
				}
			}
		}

		if (ClientActive_t->stance == CL_STANCE_STAND)
			StoredHealth = cg->ps.Health;
	}
	else {
		if (Crouched) {
			Crouched = false;
			ClientActive_t->stance = CL_STANCE_STAND;
		}
	}
}

void SaveClientsSettings(int SelectedPlayer)
{
	if (SavedPlayers)
	{
		if (SavedPlayers->Count > 500)
			return;

		for (int i = 0; i < SavedPlayers->Count; i++) {
			if (!memcmp(ClientOptions[SelectedPlayer].Xuid, SavedPlayers->PlayerInfo[i].XUID, 8)) {

				SavedPlayers->PlayerInfo[i].Prioritized = ClientOptions[SelectedPlayer].Prioritized;
				SavedPlayers->PlayerInfo[i].Ignored = ClientOptions[SelectedPlayer].Ignored;
				SavedPlayers->PlayerInfo[i].AimAtRoot = ClientOptions[SelectedPlayer].AimAtRoot;

				memcpy(SavedPlayers->PlayerInfo[i].XUID, ClientOptions[SelectedPlayer].Xuid, 8);
				memcpy(SavedPlayers->PlayerInfo[i].Gamertag, ClientOptions[SelectedPlayer].Gamertag, 32);

				WritePlayersCFG(false);
				return;
			}
		}

		SavedPlayers->PlayerInfo[SavedPlayers->Count].Prioritized = ClientOptions[SelectedPlayer].Prioritized;
		SavedPlayers->PlayerInfo[SavedPlayers->Count].Ignored = ClientOptions[SelectedPlayer].Ignored;
		SavedPlayers->PlayerInfo[SavedPlayers->Count].AimAtRoot = ClientOptions[SelectedPlayer].AimAtRoot;

		memcpy(SavedPlayers->PlayerInfo[SavedPlayers->Count].XUID, ClientOptions[SelectedPlayer].Xuid, 8);
		memcpy(SavedPlayers->PlayerInfo[SavedPlayers->Count].Gamertag, ClientOptions[SelectedPlayer].Gamertag, 32);

		SavedPlayers->Count += 1;

		WritePlayersCFG(false);
	}
}

void SaveCurrentClientsSettings()
{
	ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].LoadedFromCFG = true;
	SaveClientsSettings(Menu.CurrentScroll[Menu.MenuIDs.Players]);
}

void EnableSettingsFromCFG()
{
	if (SavedPlayers && ClientNetwork && (Menu.CurrentSub != Menu.MenuIDs.PlayerOptions))
	{
		for (int i = 0; i < 12; i++)
		{
			if ((SavedPlayers->Count > 0))
			{
				for (int j = 0; j < SavedPlayers->Count; j++)
				{
					if ((ClientNetwork->Net[i].Active == 0x3) && !memcmp(ClientNetwork->Net[i].Xuid, SavedPlayers->PlayerInfo[j].XUID, 8))
					{
						ClientOptions[i].Prioritized = SavedPlayers->PlayerInfo[j].Prioritized;
						ClientOptions[i].Ignored = SavedPlayers->PlayerInfo[j].Ignored;
						ClientOptions[i].AimAtRoot = SavedPlayers->PlayerInfo[j].AimAtRoot;
						ClientOptions[i].LoadedFromCFG = true;
					}
				}
			}
		}
	}
}

void ParseClientName(int Client)
{
	char Name[0x200] = { 0 };
	char Prefix[0x100] = { 0 };

	if (Dvar_GetBool("cl_ingame") && GetStructs() && cg->characterinfo[Client].infoValid && (Entity[Client].nextState.Type == ET_PLAYER) && (Entity[Client].pose.eType & 1) && (Client != cg->ClientNumber)) {
		if (!IsEnemy(Client))
			user_strcpy(Prefix, "^2[F]^7");
		else
			user_strcpy(Prefix, "^1[E]^7");
	}

	if (ClientOptions[Client].Prioritized)
		strcat(Prefix, "^1[Prioritized]^7");
	else if (ClientOptions[Client].Ignored)
		strcat(Prefix, "^2[Ignored]^7");

	if (ClientNetwork && (ClientNetwork->Net[Client].Active == 0x3) && !cuser_strcmp(ClientNetwork->Net[Client].gamertag, ""))
		(cuser_strcmp(ClientNetwork->Net[Client].clantag, "")) ? (void)(user_strcpy(Name, ClientNetwork->Net[Client].gamertag)) : (void)(_snprintf(Name, sizeof(Name), "[%s]%s", ClientNetwork->Net[Client].clantag, ClientNetwork->Net[Client].gamertag));
	else if (Dvar_GetBool("cl_ingame") && GetStructs() && !cuser_strcmp(cg->clientinfo[Client].Name, ""))
		(cuser_strcmp(cg->clientinfo[Client].clanAbbrev, "")) ? (void)(user_strcpy(Name, cg->clientinfo[Client].Name)) : (void)(_snprintf(Name, sizeof(Name), "[%s]%s", cg->clientinfo[Client].clanAbbrev, cg->clientinfo[Client].Name));
	else {
		user_strcpy(Name, "Empty");
		ClientOptions[Client].reset();
	}

	strcat(Prefix, Name);

	user_strcpy((char*)Menu.SubText[Menu.MenuIDs.Players][Client], Prefix);
}

void SetUserInfoFromClientInfo(int i)
{
	if (Dvar_GetBool("cl_ingame") && GetStructs() && cuser_strcmp(cg->clientinfo[i].Name, ""))
	{
		user_strcpy((char*)Menu.SubExp[Menu.MenuIDs.Players][i], "");
		ClientOptions[i].reset();
	}
}

void SetUserInfo(int i, bool PlayerMenu)
{
	if (ClientNetwork)
	{
		user_memcpy(ClientOptions[i].Xuid, ClientNetwork->Net[i].Xuid, 8);
		user_strcpy(ClientOptions[i].Gamertag, ClientNetwork->Net[i].gamertag);
		char buffer[0x200] = { 0 };

		if (PlayerMenu) {
			_snprintf(buffer, sizeof(buffer), "Client Index: %i / %i\nXUID: %02X%02X%02X%02X%02X%02X%02X%02X\nMachineID: %02X%02X%02X%02X%02X%02X%02X%02X\nIP Address: %u.%u.%u.%u:%i\nMAC Address: %02X:%02X:%02X:%02X:%02X:%02X",
				i + 1,
				Dvar_GetInt("sv_maxclients"),
				ClientNetwork->Net[i].Xuid[0], ClientNetwork->Net[i].Xuid[1], ClientNetwork->Net[i].Xuid[2], ClientNetwork->Net[i].Xuid[3], ClientNetwork->Net[i].Xuid[4], ClientNetwork->Net[i].Xuid[5], ClientNetwork->Net[i].Xuid[6], ClientNetwork->Net[i].Xuid[7],
				ClientNetwork->Net[i].MachineID[7], ClientNetwork->Net[i].MachineID[6], ClientNetwork->Net[i].MachineID[5], ClientNetwork->Net[i].MachineID[4], ClientNetwork->Net[i].MachineID[3], ClientNetwork->Net[i].MachineID[2], ClientNetwork->Net[i].MachineID[1], ClientNetwork->Net[i].MachineID[0],
				ClientNetwork->Net[i].IP[0], ClientNetwork->Net[i].IP[1], ClientNetwork->Net[i].IP[2], ClientNetwork->Net[i].IP[3],
				ClientNetwork->Net[i].Port,
				ClientNetwork->Net[i].Mac[0], ClientNetwork->Net[i].Mac[1], ClientNetwork->Net[i].Mac[2], ClientNetwork->Net[i].Mac[3], ClientNetwork->Net[i].Mac[4], ClientNetwork->Net[i].Mac[5]
			);

			user_strcpy((char*)Menu.SubExp[Menu.MenuIDs.Players][i], buffer);
		}
		else
		{
			int Scroll = Menu.CurrentScroll[Menu.MenuIDs.PlayerOptions];
			if (CE.MenuHints) {
				_snprintf(buffer, sizeof(buffer), "%s\n%s\nClient Index: %i / %i\nXUID: %02X%02X%02X%02X%02X%02X%02X%02X\nMachineID: %02X%02X%02X%02X%02X%02X%02X%02X\nIP Address: %u.%u.%u.%u:%i\nMAC Address: %02X:%02X:%02X:%02X:%02X:%02X",
					Menu.SubExpPlayerOptions[Scroll],
					Menu.SubText[Menu.MenuIDs.Players][i],
					i + 1,
					Dvar_GetInt("sv_maxclients"),
					ClientNetwork->Net[i].Xuid[0], ClientNetwork->Net[i].Xuid[1], ClientNetwork->Net[i].Xuid[2], ClientNetwork->Net[i].Xuid[3], ClientNetwork->Net[i].Xuid[4], ClientNetwork->Net[i].Xuid[5], ClientNetwork->Net[i].Xuid[6], ClientNetwork->Net[i].Xuid[7],
					ClientNetwork->Net[i].MachineID[7], ClientNetwork->Net[i].MachineID[6], ClientNetwork->Net[i].MachineID[5], ClientNetwork->Net[i].MachineID[4], ClientNetwork->Net[i].MachineID[3], ClientNetwork->Net[i].MachineID[2], ClientNetwork->Net[i].MachineID[1], ClientNetwork->Net[i].MachineID[0],
					ClientNetwork->Net[i].IP[0], ClientNetwork->Net[i].IP[1], ClientNetwork->Net[i].IP[2], ClientNetwork->Net[i].IP[3],
					ClientNetwork->Net[i].Port,
					ClientNetwork->Net[i].Mac[0], ClientNetwork->Net[i].Mac[1], ClientNetwork->Net[i].Mac[2], ClientNetwork->Net[i].Mac[3], ClientNetwork->Net[i].Mac[4], ClientNetwork->Net[i].Mac[5]
				);
			}
			else {
				_snprintf(buffer, sizeof(buffer), "%s\nClient Index: %i / %i\nXUID: %02X%02X%02X%02X%02X%02X%02X%02X\nMachineID: %02X%02X%02X%02X%02X%02X%02X%02X\nIP Address: %u.%u.%u.%u:%i\nMAC Address: %02X:%02X:%02X:%02X:%02X:%02X",
					Menu.SubText[Menu.MenuIDs.Players][i],
					i + 1,
					Dvar_GetInt("sv_maxclients"),
					ClientNetwork->Net[i].Xuid[0], ClientNetwork->Net[i].Xuid[1], ClientNetwork->Net[i].Xuid[2], ClientNetwork->Net[i].Xuid[3], ClientNetwork->Net[i].Xuid[4], ClientNetwork->Net[i].Xuid[5], ClientNetwork->Net[i].Xuid[6], ClientNetwork->Net[i].Xuid[7],
					ClientNetwork->Net[i].MachineID[7], ClientNetwork->Net[i].MachineID[6], ClientNetwork->Net[i].MachineID[5], ClientNetwork->Net[i].MachineID[4], ClientNetwork->Net[i].MachineID[3], ClientNetwork->Net[i].MachineID[2], ClientNetwork->Net[i].MachineID[1], ClientNetwork->Net[i].MachineID[0],
					ClientNetwork->Net[i].IP[0], ClientNetwork->Net[i].IP[1], ClientNetwork->Net[i].IP[2], ClientNetwork->Net[i].IP[3],
					ClientNetwork->Net[i].Port,
					ClientNetwork->Net[i].Mac[0], ClientNetwork->Net[i].Mac[1], ClientNetwork->Net[i].Mac[2], ClientNetwork->Net[i].Mac[3], ClientNetwork->Net[i].Mac[4], ClientNetwork->Net[i].Mac[5]
				);
			}

			user_strcpy((char*)Menu.SubExp[Menu.MenuIDs.PlayerOptions][Scroll], buffer);
		}
	}
}


void UpdateClientList()
{
	for (int i = 0; i < 12; i++)
	{
		ParseClientName(i);
		if (ClientNetwork && (ClientNetwork->Net[i].Active == 0x3) && !cuser_strcmp(ClientNetwork->Net[i].gamertag, ""))
			SetUserInfo(i, true);
		else
			SetUserInfoFromClientInfo(i);
	}

	if (Menu.CurrentSub == Menu.MenuIDs.PlayerOptions)
		SetUserInfo(Menu.CurrentScroll[Menu.MenuIDs.Players], false);
}

void Enable_Rainbow()
{
	Menu.RGBPointer[Menu.MenuIDs.RGBEditor][4]->ToggleRainbow();
}

void Menu_Reset()
{
	CE.SetBaseColors();
	CE.Controls = true;
	CE.SpawnMessage = true;
	CE.MenuHints = true;
	CE.BoolType = Switches;
	CE.Scroller = true;
}

void MenuLoadCFG()
{
	MemoryBuffer Buf;
	if (CReadFile("xbOnline:\\xbOnline\\IW6MP.cfg", Buf))
	{
		std::string cfg = (const char*)Buf.GetData();

		if (cfg.find("Anti-Aim Menu") != -1)
			remove("xbOnline:\\xbOnline\\IW6MP.cfg");
	}

	LoadCFG();
}

void MenuWriteCFG()
{
	WriteCFG();
}

void MenuSettingsLoadCFG()
{
	LoadMenuSettingsCFG();
}

void MenuSettingsWriteCFG()
{
	WriteMenuSettingsCFG();
}

void SaveLooperNames()
{
	SaveLooperGamertags();
}

void LoadLooperNames()
{
	LoadLooperGamertags();
}

void SaveUserNames()
{
	SaveUserGamertags();
}

void LoadUserNames()
{
	LoadUserGamertags();
}

void LoadPreset()
{
	if (CE.ConfigType == 0)
	{ //Rage
	  //Main
		CE.NoRecoil = true;
		CE.UAV = true;
		CE.NoSpread = true;
		CE.NoSway = true;
		CE.NoFlinch = true;
		CE.Laser = true;
		CE.ThirdPerson = false;
		CE.NoRecoil = true;


		//Anti-Aim
		CE.AAPitch = 3;
		CE.AAYaw = 5;
		CE.FakeLag = 200;


		//Aimbot
		CE.AimbotStatus = true;
		CE.AimKey = 0;
		CE.TargetType = 3;
		CE.AimbotType = 1;
		CE.AimTag = 0;
		CE.AutoShoot = true;
		CE.ShootDelay = 0;
		CE.Autowall = 2;
		CE.TargetDetails = true;
		CE.AimPrediction = false;
		CE.PosPrediction = true;
		CE.PosPredictionScale = 0.25f;
		CE.PosAdjustment = true;
		CE.PosAdjustmentScale = 1.50f;
		CE.ShowAimbotTarget = false;
	}
	else if (CE.ConfigType == 1)
	{ //HvH
	  //Main
		CE.NoRecoil = true;
		CE.UAV = true;
		CE.NoSpread = true;
		CE.NoSway = true;
		CE.NoFlinch = true;
		CE.Laser = true;
		CE.ThirdPerson = false;
		CE.NoRecoil = true;
		CE.Fov = 90.0f;


		//Anti-Aim
		CE.AAPitch = 1;
		CE.AAYaw = 1;
		CE.FakeLag = 200;


		//Aimbot
		CE.AimbotStatus = true;
		CE.AimKey = 0;
		CE.TargetType = 3;
		CE.AimbotType = 1;
		CE.AimTag = 0;
		CE.AutoShoot = true;
		CE.ShootDelay = 0;
		CE.Autowall = 2;
		CE.TargetDetails = true;
		CE.AimPrediction = false;
		CE.PosPrediction = true;
		CE.PosPredictionScale = 0.25f;
		CE.PosAdjustment = true;
		CE.PosAdjustmentScale = 1.50f;
		CE.ShowAimbotTarget = false;
	}
	else if (CE.ConfigType == 2)
	{ //Public Chill
	  //Main
		CE.NoRecoil = true;
		CE.UAV = true;
		CE.NoSpread = true;
		CE.NoSway = true;
		CE.NoFlinch = true;
		CE.Laser = true;
		CE.ThirdPerson = false;
		CE.NoRecoil = true;


		//Anti-Aim
		CE.AAPitch = 0;
		CE.AAYaw = 0;
		CE.FakeLag = 0;


		//Aimbot
		CE.AimbotStatus = true;
		CE.AimKey = 0;
		CE.TargetType = 3;
		CE.AimbotType = 1;
		CE.AimTag = 2;
		CE.AutoShoot = true;
		CE.ShootDelay = 0;
		CE.Autowall = 1;
		CE.TargetDetails = true;
		CE.AimPrediction = false;
		CE.PosPrediction = true;
		CE.PosPredictionScale = 0.25f;
		CE.PosAdjustment = false;
		CE.PosAdjustmentScale = 1.0f;
		CE.ShowAimbotTarget = false;
	}
	else if (CE.ConfigType == 3)
	{ //OSM's Config
	  //Main
		CE.NoRecoil = true;
		CE.UAV = true;
		CE.NoSpread = true;
		CE.NoSway = true;
		CE.NoFlinch = true;
		CE.Laser = true;
		CE.ThirdPerson = false;
		CE.NoRecoil = true;
		CE.Fov = 90.0f;


		//Anti-Aim
		CE.AAPitch = 1;
		CE.AAYaw = 1;
		CE.FakeLag = 0;


		//Aimbot
		CE.AimbotStatus = true;
		CE.AimKey = 0;
		CE.TargetType = 3;
		CE.AimbotType = 1;
		CE.AimTag = 0;
		CE.AutoShoot = true;
		CE.ShootDelay = 0;
		CE.Autowall = 2;
		CE.TargetDetails = true;
		CE.AimPrediction = false;
		CE.PosPrediction = true;
		CE.PosPredictionScale = 0.25f;
		CE.PosAdjustment = true;
		CE.PosAdjustmentScale = 1.50f;
		CE.ShowAimbotTarget = false;


		//Visual
		CE.Crosshair = 1;
		CE.ProximityWarning = true;
		CE.VisibleWarning = true;
		CE.AimWarning = true;
		CE.HealthBar = true;
		CE.Impacts = false;
		CE.ExternalRadar = true;
		{ //Radar Settings
			CE.RadarSettings.DrawEnemies = true;
			CE.RadarSettings.DrawFriendies = true;
			CE.RadarSettings.DrawExplosives = true;
			CE.RadarSettings.DrawWeapons = true;
			CE.RadarSettings.DrawScavengerPacks = true;
			CE.RadarSettings.DrawKillStreaks = true;
		}
		CE.ESPType = 4;
		{ //ESP Settings
			CE.ESPSettings.DrawEnemies = true;
			CE.ESPSettings.DrawFriendies = true;
			CE.ESPSettings.DrawDoggos = true;
			CE.ESPSettings.ESPBones = true;
			CE.ESPSettings.ESPName = true;
			CE.ESPSettings.ESPDistance = true;
			CE.ESPSettings.ESPWeaponNames = true;
			CE.ESPSettings.ESPWeaponShaders = true;
			CE.ESPSettings.SnapLines = 0;
			CE.ESPSettings.DrawExplosives = true;
			CE.ESPSettings.DrawWeapons = true;
			CE.ESPSettings.DrawScavengerPacks = true;
			CE.ESPSettings.DrawKillStreaks = true;
			CE.ESPSettings.PlayerIndicator = true;
			CE.ESPSettings.PlayerIndicatorIcon = 0;
			CE.ESPSettings.PlayerIndicatorSize[0] = 25.0f;
			CE.ESPSettings.PlayerIndicatorSize[1] = 25.0f;
			CE.ESPSettings.PlayerIndicatorDiameter = 130.0f;
		}
	}
	else if (CE.ConfigType == 4)
	{
		//Main
		CE.NoRecoil = true;
		CE.UAV = true;
		CE.NoSpread = true;
		CE.NoSway = true;
		CE.NoFlinch = true;
		CE.Laser = true;
		CE.ThirdPerson = false;
		CE.NoRecoil = true;
		CE.Fov = 90.0f;


		//Anti-Aim
		CE.AAPitch = 1;
		CE.AAYaw = 1;
		CE.FakeLag = 0;


		//Aimbot
		CE.AimbotStatus = true;
		CE.AimKey = 0;
		CE.TargetType = 3;
		CE.AimbotType = 1;
		CE.AimTag = 0;
		CE.AutoShoot = true;
		CE.ShootDelay = 0;
		CE.Autowall = 2;
		CE.TargetDetails = true;
		CE.AimPrediction = false;
		CE.PosPrediction = true;
		CE.PosPredictionScale = 0.45f;
		CE.PosAdjustment = true;
		CE.PosAdjustmentScale = 1.50f;
		CE.ShowAimbotTarget = false;
	}

	const char* configs[] = { "Rage", "HvH", "Public Chill", "OSM's Config", "Nigel's Config" };
	XNotify(toWCHAR("xbOnline - Config Loaded [%s]", configs[CE.ConfigType]));
}