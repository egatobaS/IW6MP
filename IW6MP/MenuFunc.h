#pragma once

void ConsoleCommand();
void Changeclantag();
void ChangeGamertag();
void ResetGamertag();
void EndGame();
void LeaveGame();
void ModLooper();

void StoreName();
void FlashingNameThread(LPVOID r3);
void GamertagLooper();
void FlashingGmaertag();

void SetNameAsIP();
void StealName();
void OnPlayerSpawn();
void DoCrouchBot();
void SaveClientsSettings(int SelectedPlayer);
void SaveCurrentClientsSettings();
void EnableSettingsFromCFG();
void ParseClientName(int Client);
void SetUserInfoFromClientInfo(int i);
void SetUserInfo(int i, bool PlayerMenu);
void UpdateClientList();

void Enable_Rainbow();
void Menu_Reset();
void MenuLoadCFG();
void MenuWriteCFG();
void MenuSettingsLoadCFG();
void MenuSettingsWriteCFG();
void SaveLooperNames();
void LoadLooperNames();
void SaveUserNames();
void LoadUserNames();
void LoadPreset();