#pragma once

struct PlayerInfo_s
{
	char Gamertag[32];
	unsigned char XUID[8];
	bool Prioritized;
	bool Ignored;
	bool AimAtRoot;
};

struct SavedPlayers_s
{
	int Count;
	PlayerInfo_s PlayerInfo[500];
};

extern SavedPlayers_s* SavedPlayers;

void ReadMenu(int ID);
DWORD WINAPI DoLoadCFG();
void LoadCFG(bool Direct = false);
void WriteMenu(int ID);
DWORD WINAPI DoWriteCFG();
void WriteCFG(bool Direct = false);
void WritePlayerToCFG(char* Gamertag, unsigned char* XUID, bool Prioritized, bool Ignored, bool AimAtRoot);
DWORD WINAPI DoWritePlayersCFG();
void WritePlayersCFG(bool Direct = false);
void ReadPlayerFromCFG(int index);
DWORD WINAPI DoLoadPlayersCFG();
void LoadPlayersCFG(bool Direct = false);
DWORD WINAPI DoWriteMenuSettingsCFG();
void WriteMenuSettingsCFG(bool Direct = false);
DWORD WINAPI DoLoadMenuSettingsCFG();
void LoadMenuSettingsCFG(bool Direct = false);
void LoadUserGamertags(bool Direct = false);
void SaveUserGamertags(bool Direct = false);
void LoadLooperGamertags(bool Direct = false);
void SaveLooperGamertags(bool Direct = false);

void MonitorCFGThread();