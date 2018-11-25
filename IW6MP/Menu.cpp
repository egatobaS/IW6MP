#include "stdafx.h"
#include "Menu.h"
#include "Functions.h"
#include "Drawing.h"
#include "MenuFunc.h"
#include "LoadCFG.h"

CE_s CE;
Menu_s Menu;

const char* TagList[19] = { "j_helmet", "j_head", "j_neck", "j_shoulder_le", "j_shoulder_ri", "j_elbow_le", "j_elbow_ri", "j_wrist_le", "j_wrist_ri", "j_mainroot", "j_spineupper", "j_spinelower", "j_spine4", "j_hip_le", "j_hip_ri", "j_knee_le", "j_knee_ri", "j_ankle_le", "j_ankle_ri" };
const char* HitBoxesP[] = { "j_head", "j_neck", "j_shoulder_le", "j_shoulder_ri", "j_knee_le", "j_knee_ri", "j_ankle_le", "j_ankle_ri", "j_elbow_le", "j_elbow_ri", "j_ball_le", "j_ball_ri" };
const char* HitBoxes[] = { "j_head", "j_spineupper", "j_knee_le", "j_knee_ri", "j_shoulder_le", "j_shoulder_ri", "j_ankle_le", "j_ankle_ri", "j_ball_le", "j_ball_ri" };
const char* RiotBoxes[] = { "j_head", "j_spine4", "j_hip_le", "j_hip_ri", "j_knee_ri", "j_knee_le", "j_elbow_le", "j_elbow_ri", "j_ankle_ri", "j_ankle_le", "j_wrist_ri", "j_wrist_le", "j_ball_le", "j_ball_ri" };

int SubIndex = 0;
int CurrentSub = -1;

void CreateSub(const char* Name, char* MenuID, char* LastMenuID)
{
	SubIndex++;
	*MenuID = SubIndex;

	CurrentSub = -1;

	Menu.SubTitle[SubIndex] = Name;
	Menu.LastSub[SubIndex] = LastMenuID;

	for (int i = 0; i < Menu.MaxRows; i++)
		Menu.SubType[SubIndex][i] = SUB_EMPTY;
}

void InitSub(const char* SubName, const char* SubExp, char SubType)
{
	CurrentSub++;

	Menu.SubType[SubIndex][CurrentSub] = SubType;
	user_strcpy((char*)Menu.SubText[SubIndex][CurrentSub], SubName);
	user_strcpy((char*)Menu.SubExp[SubIndex][CurrentSub], SubExp);
	Menu.MaxScroll[SubIndex]++;
}

void AddSubMenuBranch(const char* SubName, const char* SubExp, char* NextMenuID)
{
	InitSub(SubName, SubExp, SUB_BRANCH);

	Menu.NextSub[SubIndex][CurrentSub] = NextMenuID;
}

void AddFuctionCall(const char* SubName, const char* SubExp, void(*Function)(Params), Params ParamValue)
{
	InitSub(SubName, SubExp, SUB_FUNCTION);

	Menu.FunctionCall[SubIndex][CurrentSub] = Function;
	Menu.PassParam[SubIndex][CurrentSub] = ParamValue;
}

void AddFuctionCall(const char* SubName, const char* SubExp, void(*Function)(char*), char* ParamValue)
{
	InitSub(SubName, SubExp, SUB_FUNCTIONCHAR);

	Menu.FunctionCallChar[SubIndex][CurrentSub] = Function;
	Menu.PassParam[SubIndex][CurrentSub].String = ParamValue;
}

void AddFuctionCall(const char* SubName, const char* SubExp, void(*Function)())
{
	InitSub(SubName, SubExp, SUB_FUNCTIONVOID);

	Menu.FunctionCallVoid[SubIndex][CurrentSub] = Function;
}

void AddBool(const char* SubName, const char* SubExp, bool* Boolean)
{
	InitSub(SubName, SubExp, SUB_BOOL);

	Menu.BoolToggle[SubIndex][CurrentSub] = Boolean;
}

void AddBoolCall(const char* SubName, const char* SubExp, void(*Function)(), bool* Boolean)
{
	InitSub(SubName, SubExp, SUB_BOOLCALL);

	Menu.BoolToggle[SubIndex][CurrentSub] = Boolean;
	Menu.BoolCall[SubIndex][CurrentSub] = Function;
}

void AddStringArray(const char* SubName, const char* SubExp, int* ArrayScrollPointer, const char* StringArray[], int SizeOfArray)
{
	InitSub(SubName, SubExp, SUB_STRINGARRAY);

	Menu.StrArrayMax[SubIndex][CurrentSub] = SizeOfArray - 1;
	for (int i = 0; i < SizeOfArray; i++)
		Menu.StrArray[SubIndex][CurrentSub][i] = StringArray[i];
	Menu.StrArrayScroll[SubIndex][CurrentSub] = ArrayScrollPointer;
}

void StringArrayFunctionCall(const char* SubName, const char* SubExp, void(*Function)(), int* ArrayScrollPointer, const char* StringArray[], int SizeOfArray)
{
	InitSub(SubName, SubExp, SUB_STRINGARRAYCALL);

	Menu.StrArrayMax[SubIndex][CurrentSub] = SizeOfArray - 1;
	for (int i = 0; i < SizeOfArray; i++)
		Menu.StrArray[SubIndex][CurrentSub][i] = StringArray[i];
	Menu.StrArrayScroll[SubIndex][CurrentSub] = ArrayScrollPointer;
	Menu.FunctionCallVoid[SubIndex][CurrentSub] = Function;
}

void AddFloat(const char* SubName, const char* SubExp, float* Float, float Ammount, float min, float max)
{
	InitSub(SubName, SubExp, SUB_FLOAT);

	Menu.FloatPointer[SubIndex][CurrentSub] = Float;
	*Float = min;
	Menu.FloatAmmount[SubIndex][CurrentSub] = Ammount;
	Menu.FloatMin[SubIndex][CurrentSub] = min;
	Menu.FloatMax[SubIndex][CurrentSub] = max;
}

void AddFloatProgress(const char* SubName, const char* SubExp, float* Float, float Ammount, float min, float max)
{
	InitSub(SubName, SubExp, SUB_FLOATPROGRESS);

	Menu.FloatPointer[SubIndex][CurrentSub] = Float;
	*Float = min;
	Menu.FloatAmmount[SubIndex][CurrentSub] = Ammount;
	Menu.FloatMin[SubIndex][CurrentSub] = min;
	Menu.FloatMax[SubIndex][CurrentSub] = max;
}

void AddInt(const char* SubName, const char* SubExp, int* Int32, int Ammount, int min, int max)
{
	InitSub(SubName, SubExp, SUB_INT);

	Menu.IntPointer[SubIndex][CurrentSub] = Int32;
	*Int32 = min;
	Menu.IntAmmount[SubIndex][CurrentSub] = Ammount;
	Menu.IntMin[SubIndex][CurrentSub] = min;
	Menu.IntMax[SubIndex][CurrentSub] = max;
}

void AddIntCall(const char* SubName, const char* SubExp, void(*Function)(), int* Int32, int Ammount, int min, int max)
{
	InitSub(SubName, SubExp, SUB_INTCALL);

	Menu.IntPointer[SubIndex][CurrentSub] = Int32;
	*Int32 = min;
	Menu.IntAmmount[SubIndex][CurrentSub] = Ammount;
	Menu.IntMin[SubIndex][CurrentSub] = min;
	Menu.IntMax[SubIndex][CurrentSub] = max;
	Menu.FunctionCallVoid[SubIndex][CurrentSub] = Function;
}

void AddIntProgress(const char* SubName, const char* SubExp, int* Int32, int Ammount, int min, int max)
{
	InitSub(SubName, SubExp, SUB_INTPROGRESS);

	Menu.IntPointer[SubIndex][CurrentSub] = Int32;
	*Int32 = min;
	Menu.IntAmmount[SubIndex][CurrentSub] = Ammount;
	Menu.IntMin[SubIndex][CurrentSub] = min;
	Menu.IntMax[SubIndex][CurrentSub] = max;
}

void AddIntProgressMS(const char* SubName, const char* SubExp, int* Int32, int Ammount, int min, int max)
{
	InitSub(SubName, SubExp, SUB_INTPROGRESSMS);

	Menu.IntPointer[SubIndex][CurrentSub] = Int32;
	*Int32 = min;
	Menu.IntAmmount[SubIndex][CurrentSub] = Ammount;
	Menu.IntMin[SubIndex][CurrentSub] = min;
	Menu.IntMax[SubIndex][CurrentSub] = max;
}

void AddRGB(const char* SubName, const char* SubExp, RGBA* RGB)
{
	InitSub(SubName, SubExp, SUB_RGB);

	Menu.RGBPointer[SubIndex][CurrentSub] = RGB;
}

void AddFloatProgressRGB(const char* SubName, const char* SubExp, float Ammount)
{
	InitSub(SubName, SubExp, SUB_FLOATPROGRESSRGB);

	Menu.FloatAmmount[SubIndex][CurrentSub] = Ammount;
	Menu.FloatMin[SubIndex][CurrentSub] = 0.0f;
	Menu.FloatMax[SubIndex][CurrentSub] = 1.0f;
}

void AddBoolRGB(const char* SubName, const char* SubExp)
{
	InitSub(SubName, SubExp, SUB_BOOLCALL);
}

void AddTextBox(const char* SubName, const char* SubExp, char* Text, char* DefaultValue, int MaxLen)
{
	InitSub(SubName, SubExp, SUB_TEXTBOX);

	Menu.Text[SubIndex][CurrentSub] = Text;
	user_strcpy((char*)Menu.Text[SubIndex][CurrentSub], DefaultValue);
	Menu.MaxLen[SubIndex][CurrentSub] = MaxLen;
}

void AddTextBoxCall(const char* SubName, const char* SubExp, char* Text, char* DefaultValue, int MaxLen, void(*Function)(char*))
{
	InitSub(SubName, SubExp, SUB_TEXTBOXCALL);

	Menu.FunctionCallChar[SubIndex][CurrentSub] = Function;
	Menu.Text[SubIndex][CurrentSub] = Text;
	user_strcpy((char*)Menu.Text[SubIndex][CurrentSub], DefaultValue);
	Menu.MaxLen[SubIndex][CurrentSub] = MaxLen;
}

void SetTextBox(char* val)
{
	user_strcpy((char*)Menu.Text[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]], val);
}

void FailedToReadUserNames()
{
	XNotify(L"xbOnline - Failed to read file check format and try again.");
}

char NONE = 0xFF;
void CreateMenu()
{
	/*Main*/
	CreateSub("Main", &Menu.MenuIDs.Main, &NONE);
	AddSubMenuBranch("Name Menu", "Change your Gamertag and Clantag.", &Menu.MenuIDs.NameMenu);
	AddSubMenuBranch("HvH Menu", "change Hack vs Hack settings.", &Menu.MenuIDs.HVH);
	AddBool("No Recoil", "Remove the recoil of your weapons.", &CE.NoRecoil);
	AddBool("Constant Radar", "Enable the drawing of enemies on Ghosts's radar.", &CE.UAV);
	AddBool("No Spread", "Counter the spread of your weapons.", &CE.NoSpread);
	AddBool("No Sway", "Remove the sway of your weapons", &CE.NoSway);
	AddBool("No Flinch", "Remove the angles offset for when you are shot.", &CE.NoFlinch);
	AddBool("Laser", "Enable/Disable the drawing of the laser.", &CE.Laser);
	AddBool("Third Person", "Enable/Disable the third person camera mode.", &CE.ThirdPerson);
	//AddBool("IP Spoofing", "Enable/Disable spoofing of your IP Address if you are trying to party up with some one you need to disable this first.", &CE.IPSpoofer);
	//AddBool("Never Host", "", &CE.NeverHost);
	AddFloatProgress("Field Of View", "Change the ammount of observable game world you can see.", &CE.Fov, 5, 65, 120);
	AddFuctionCall("Change Team", "Allows you to change teams off host though the teams become broken so you will need to enable \"Invert Team\".", ChangeTeam);
	AddBool("Invert Team", "Invert the team check so when you spoof teams you will shoot the correct enemies. (use with \"Change Team\")", &CE.InverseTeams);
	AddIntCall("Class Number", "This will be the class used when you get Un-Infected or spawn as the Juggernaut or set your current class.", ChangeClass, &CE.ClassNum, 1, 1, 15);
	AddBool("Auto Un-Infector", "Will automatically Un-Infect you if you become infected (Only works in the Infected Game Mode).", &CE.AutoUnInfect);
	AddFuctionCall("Set Juggernaut", "Change into the Juggernaut streak when ever you want. (Note: If you want your custom class choose this right when you spawn in at the start of the game.)", JugClass);
	AddFuctionCall("Console Command", "Send a command to the console.", ConsoleCommand);
	AddFuctionCall("End Game", "End the game with out being host.", EndGame);
	AddFuctionCall("Leave Game", "Leave the game at any point handy for long host migrations.", LeaveGame);

	/*Aimbot*/
	CreateSub("Aimbot", &Menu.MenuIDs.Aimbot, &NONE);
	AddBool("Aimbot Status", "Enable/Disable the aimbot.", &CE.AimbotStatus);
	const char* AimKeyArray[] = { "None", "LB", "LT", "RB", "RT" };
	AddStringArray("Aim Key", "If enabled the aimbot will only lock on to players when the selected key is pressed", &CE.AimKey, AimKeyArray, 5);
	const char* TargetTypeArray[] = { "Closest", "Visible", "On-Screen", "Logical" };
	AddStringArray("Target Type", "How the aimbot will decide what player to target.\nClosest: It will target the closest client.\nVisible: It will target only visible clients.\nLogical: It will target the killable client closest first.", &CE.TargetType, TargetTypeArray, 4);
	const char* AimbotTypeArray[] = { "Normal", "Silent", "Snap" };
	AddStringArray("Aimbot Type", "The type of aimbot that will be used.\nNormal: the aimbot will lock on with local angles\nSilent: The aimbot will lock on with angles spoofed so it wont be visible\nSnap: The aimbot will lock on and off of clients.", &CE.AimbotType, AimbotTypeArray, 3);
	const char* tagnames[] = { "Auto", "j_helmet", "j_head", "j_neck", "j_shoulder_le", "j_shoulder_ri", "j_elbow_le", "j_elbow_ri", "j_wrist_le", "j_wrist_ri", "j_mainroot", "j_spineupper", "j_spinelower", "j_spine4", "j_hip_le", "j_hip_ri", "j_knee_le", "j_knee_ri", "j_ankle_le", "j_ankle_ri" };
	AddStringArray("Aim Tag", "The tag the aimbot will aim at on the target.", &CE.AimTag, tagnames, 20);
	AddBool("Auto Shoot", "When the aimbot has a target it will shoot for you.", &CE.AutoShoot);
	AddIntProgressMS("Auto Shoot Delay", "The time the autoshoot will wait between shots, best for bolt action rifles.", &CE.ShootDelay, 50, 0, 300);
	const char* AutowallArray[] = { "None", "Normal", "pAutoWall", "pAutoWall for Priority Only" };
	AddStringArray("Autowall", "This option if enabled will check if you can shoot through walls.\nNormal: Just a normal trace.\npAutoWall: An improved version.\npAutoWall for Priority Only: Will use Normal for regular clients and pAutoWall for priority clients", &CE.Autowall, AutowallArray, 4);
	AddBool("Show Target Details", "Show information about the client the aimbot is targeting.", &CE.TargetDetails);
	AddBool("Ping Prediction", "The aimbot will predict where the target will be based on your connection (only use when you have a high ping).", &CE.AimPrediction);
	AddBool("Position Adjustment", "The aimbot will predict where the target will be based on their velocity.", &CE.PosPrediction);
	AddFloatProgress("Position Adjustment Scale", "The Scale in which the position will be predicted the best is usually around 0.50.", &CE.PosPredictionScale, 0.05f, 0.0f, 0.75f);
	AddBool("Point Correction", "Correction to hit the very edge of a bone.", &CE.PosAdjustment);
	AddFloatProgress("Point Correction Scale", "The amount of Point Correction, how far from the edge of the bone.", &CE.PosAdjustmentScale, 0.50f, 1.0f, 30.0f);
	AddBool("Show Aimbot Target", "Show the aimbot target by the colour of the ESP.", &CE.ShowAimbotTarget);


	/*Visual*/
	CreateSub("Visual", &Menu.MenuIDs.Visual, &NONE);
	const char* CorsshairArray[] = { "None", "+", "X", "Spin" };
	AddStringArray("Crosshair", "Draw a custom external crosshair.", &CE.Crosshair, CorsshairArray, 4);
	AddBool("Proximity Warning", "Draw a warning when enemies are within 10m of your position.", &CE.ProximityWarning);
	AddBool("Visible Warning", "Draw a warning when enemies are visible.", &CE.VisibleWarning);
	AddBool("Aim Warning ", "Draw a warning when enemies are aiming at you.", &CE.AimWarning);
	AddBool("Health Bar", "Draw your current health on screen with a progress bar.", &CE.HealthBar);
	AddBool("Weapon Impacts", "Draw the impacts from weapons, disabling this can help improve frame rate.", &CE.Impacts);
	AddBool("External Radar", "Draw the external radar.", &CE.ExternalRadar);
	AddSubMenuBranch("Radar Settings", "Edit what is drawn on the radar.", &Menu.MenuIDs.RadarSettings);
	const char* ESPTypeArray[] = { "None", "2D", "3D", "Filled", "Corner" };
	AddStringArray("ESP Type", "Change the style of ESP.", &CE.ESPType, ESPTypeArray, 5);
	AddSubMenuBranch("ESP Settings", "Edit ESP is drawn.", &Menu.MenuIDs.ESPSettings);
	AddSubMenuBranch("Visions", "Change your current vision.", &Menu.MenuIDs.Visions);
	AddSubMenuBranch("Debug Settings", "Draw various game information on your screen.", &Menu.MenuIDs.DebugSettings);


	/*Players*/
	CreateSub("Players", &Menu.MenuIDs.Players, &NONE);
	for (int i = 0; i < 12; i++)
		AddSubMenuBranch("Empty", "", &Menu.MenuIDs.PlayerOptions);


	/*Settings*/
	CreateSub("Settings", &Menu.MenuIDs.Settings, &NONE);
	const char* configs[] = { "Rage", "HvH", "Public Chill", "OSM's Config", "Nigel's Config" };
	StringArrayFunctionCall("Load Preset Config", "Load a config to fit your play type.", LoadPreset, &CE.ConfigType, configs, 5);
	AddFuctionCall("Load Config", "Load the menu settings from the \"HDD:\\xbOnline\\IW6MP.cfg\"", MenuLoadCFG);
	AddFuctionCall("Save Config", "Save the current menu settings to the \"HDD:\\xbOnline\\IW6MP.cfg\"", MenuWriteCFG);
	AddSubMenuBranch("Menu Customization Settings", "Change the look of the menu to suit you.", &Menu.MenuIDs.MenuSettings);
	//AddBool("Kill Sounds", "Enable/Disable the quake sounds.", &CE.killsounds);
	AddBool("Quake style notifications", "Enable/Disable the quake killing spree notifications.", &CE.quakeprints);
	//AddBool("Modder Detection", "Automatically detect other modders and Prioritze that client.", &CE.MDetection);

	CreateSub("Menu Customization Settings", &Menu.MenuIDs.MenuSettings, &Menu.MenuIDs.Settings);
	const char* BoolType[] = { "Text", "Check Box", "Fill Box", "Switches" };
	AddStringArray("Bool Type", "Change the way the menu draws bools.", &CE.BoolType, BoolType, 4);
	//const char* ArrayStyles[] = { "Single", "Drop Down" };
	//AddStringArray("Array Style", "Change the style of arrays", &CE.ArrayStyle, ArrayStyles, 2);
	AddSubMenuBranch("Colour Settings", "Change the colours of the menu to suit you.", &Menu.MenuIDs.ColourSettings);
	AddFloatProgress("Safe Area Horizontal", "Change the area where the cheats will draw (useful if things are drawing off your screen).", &CE.MenuSafeArea[0], 5, 0, 60);
	AddFloatProgress("Safe Area Vertical", "Change the area where the cheats will draw (useful if things are drawing off your screen).", &CE.MenuSafeArea[1], 5, 0, 60);
	AddBool("Controller Vibration", "Enable/Disable controller vibration in the menu.", &CE.ControllerVibration);
	AddBool("Menu Sounds", "Enable/Disable the sounds in the menu.", &CE.MenuSounds);
	AddBool("Control Hints", "If enabled hints will be displayed showing how to control the menu.", &CE.Controls);
	AddBool("Menu Hints", "If enabled hints will be displayed at the bottom of the menu.", &CE.MenuHints);
	AddBool("Scroller bar", "Draw a scroller bar to show the current selected option.", &CE.Scroller);
	AddBool("Spawn Message", "Enable/Disbale the message that is printed in the killfeed when you respawn.", &CE.SpawnMessage);
	AddBool("UI Colour Mods", "This will change the colours of the game menu to the \"Menu Primary\" Colours.", &CE.UI_Modification);
	//AddBool("Infinite Scrolling", "", &CE.InfiniteScrolling);
	//AddIntProgress("Number Of Options", "", &CE.NumberOfOptions, 1, 10, 21);
	AddFuctionCall("Reset Menu Settings", "Reset the current menu settings to defaults", Menu_Reset);
	AddFuctionCall("Load Menu Settings", "Load the menu settings from the \"HDD:\\xbOnline\\MenuSettings.cfg\"", MenuSettingsLoadCFG);
	AddFuctionCall("Save Menu Settings", "Save the current menu settings to the \"HDD:\\xbOnline\\MenuSettings.cfg\"", MenuSettingsWriteCFG);


	CreateSub("Colour Settings", &Menu.MenuIDs.ColourSettings, &Menu.MenuIDs.MenuSettings);
	AddRGB("Menu Primary", "Change the Primary colour of the menu.", &CE.MenuPrimaryColour);
	AddRGB("Menu Secondary", "Change the Secondary colour of the menu.", &CE.MenuSecondaryColour);
	AddRGB("Menu Text", "Change the Text colour of the menu.", &CE.TextColour);
	AddRGB("Aimbot Target", "Change the Aimbot Target colour.", &CE.AimbotTargetColor);
	AddRGB("Enemy", "Change the Enemy colour of the ESP.", &CE.EnemyColor);
	AddRGB("Priority Enemy", "Change the Priority Enemy colour of the ESP.", &CE.PriorityEnemyColor);
	AddRGB("Friendly", "Change the Friendly colour of the ESP.", &CE.FriendlyColor);
	AddRGB("Killable", "Change the Killable colour of the ESP.", &CE.KillableColor);
	AddRGB("Visible", "Change the Visible colour of the ESP.", &CE.VisibleColor);
	AddRGB("Bones", "Change the Bones colour.", &CE.BonesColor);


	CreateSub("RGB Editor", &Menu.MenuIDs.RGBEditor, &NONE);
	AddFloatProgressRGB("R", "", 0.01f);
	AddFloatProgressRGB("G", "", 0.01f);
	AddFloatProgressRGB("B", "", 0.01f);
	AddFloatProgressRGB("A", "", 0.01f);
	AddBoolRGB("Fade", "Fade this colour through all the colours of the rainbow.");


	CreateSub("Player Options", &Menu.MenuIDs.PlayerOptions, &Menu.MenuIDs.Players);
	AddBool("Prioritize", "If enabled if this player is able to be killed the aimbot will target this player over all other players.", &ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].Prioritized);
	AddBool("Ignore", "If enabled the aimbot will not target this player.", &ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].Ignored);
	AddBool("Aim at Root", "If enabled the aimbot will aim at the root of the player.", &ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].AimAtRoot);
	AddBool("Invert Team", "Invert the team check so if this client is killing you from your team (doing the team change mod) you can still kill them.", &ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].InvertTeam);
	AddFuctionCall("Change Name to IP", "Change your Gamertag to their IP.", SetNameAsIP);
	AddFuctionCall("Steal Name", "Take this players name.", StealName);
	AddFuctionCall("Remember Player", "Save this players details and current settings to the \"HDD://xbOnline//SavedPlayers.cfg\".", SaveCurrentClientsSettings);


	CreateSub("Radar Settings", &Menu.MenuIDs.RadarSettings, &Menu.MenuIDs.Visual);
	AddBool("Draw Enemies", "Draw enemy players.", &CE.RadarSettings.DrawEnemies);
	AddBool("Draw Friendlies", "Draw friendly players.", &CE.RadarSettings.DrawFriendies);
	AddBool("Draw Explosives", "Draw explosives on the radar.", &CE.RadarSettings.DrawExplosives);
	AddBool("Draw Weapons", "Draw weapon pickups on the radar.", &CE.RadarSettings.DrawWeapons);
	AddBool("Draw Scavenger Bags", "Draw scavenger bags on the radar.", &CE.RadarSettings.DrawScavengerPacks);
	AddBool("Draw Kill Streaks", "Draw kill streaks on the radar.", &CE.RadarSettings.DrawKillStreaks);


	CreateSub("ESP Settings", &Menu.MenuIDs.ESPSettings, &Menu.MenuIDs.Visual);
	AddBool("Draw Enemies", "Draw enemy players.", &CE.ESPSettings.DrawEnemies);
	AddBool("Draw Friendlies", "Draw friendly players.", &CE.ESPSettings.DrawFriendies);
	AddBool("Bones", "Draw the player bones on the ESP.", &CE.ESPSettings.ESPBones);
	AddBool("Names", "Draw the player names on the ESP.", &CE.ESPSettings.ESPName);
	AddBool("Distance", "Draw the distance of players on the ESP.", &CE.ESPSettings.ESPDistance);
	AddBool("Weapon Names", "Draw the names of weapons on the ESP.", &CE.ESPSettings.ESPWeaponNames);
	AddBool("Weapon Shaders", "Draw the weapon shaders on ESP.", &CE.ESPSettings.ESPWeaponShaders);
	const char* SnapLinesArray[] = { "None", "Top", "Middle", "Bottom" };
	AddStringArray("Snap Lines", "Draw a line from a place on the screen to the players in the game.", &CE.ESPSettings.SnapLines, SnapLinesArray, 4);
	AddBool("Draw Explosives", "Draw explosives ESP.", &CE.ESPSettings.DrawExplosives);
	AddBool("Draw Weapons", "Draw weapon pickups ESP.", &CE.ESPSettings.DrawWeapons);
	AddBool("Draw Scavenger Bags", "Draw scavenger bags ESP.", &CE.ESPSettings.DrawScavengerPacks);
	AddBool("Draw Kill Streaks", "Draw kill streaks ESP.", &CE.ESPSettings.DrawKillStreaks);
	AddBool("Player Indicator", "Draw a pointer that that will idicate the direction of players.", &CE.ESPSettings.PlayerIndicator);
	const char* PlayerIndicatorIconArray[] = { "Score Board Arrow", "Radar Arrow", "Triangle", "Objective Pointer", "Grenade Pointer", "Grenade Pointer Outlined" };
	AddStringArray("Indicator Icon", "The Icon that the Player Indicator draws.", &CE.ESPSettings.PlayerIndicatorIcon, PlayerIndicatorIconArray, 6);
	AddIntProgress("Indicator Scale Width", "", &CE.ESPSettings.PlayerIndicatorSize[0], 5, 10, 50);
	AddIntProgress("Indicator Scale Height", "", &CE.ESPSettings.PlayerIndicatorSize[1], 5, 10, 50);
	AddIntProgress("Indicator Diameter", "", &CE.ESPSettings.PlayerIndicatorDiameter, 5, 100, 200);


	CreateSub("Visions", &Menu.MenuIDs.Visions, &Menu.MenuIDs.Visual);
	AddFuctionCall("contingency_thermal_inverted", "Set your current vision to \"contingency_thermal_inverted\".", SetVision, "contingency_thermal_inverted");
	AddFuctionCall("icbm", "Set your current vision to \"icbm\".", SetVision, "icbm");
	AddFuctionCall("default_night_mp", "Set your current vision to \"default_night_mp\".", SetVision, "default_night_mp");
	AddFuctionCall("sniperescape", "Set your current vision to \"sniperescape\".", SetVision, "sniperescape");
	AddFuctionCall("sepia", "Set your current vision to \"sepia\".", SetVision, "sepia");
	AddFuctionCall("cheat_chaplinnight", "Set your current vision to \"cheat_chaplinnight\".", SetVision, "cheat_chaplinnight");
	AddFuctionCall("cheat_invert", "Set your current vision to \"cheat_invert\".", SetVision, "cheat_invert");
	AddFuctionCall("cheat_invert_contrast", "Set your current vision to \"cheat_invert_contrast\".", SetVision, "cheat_invert_contrast");
	AddFuctionCall("default", "Set your current vision back to the default vision.", ResetSetVision);


	CreateSub("Debug Settings", &Menu.MenuIDs.DebugSettings, &Menu.MenuIDs.Visual);
	AddBool("Screen Resolution", "Draw your screen resolution on the screen.", &CE.ScreenResolution);
	AddBool("FPS", "Draw your frames per second on the screen.", &CE.FPS);
	AddBool("Ping", "Draw your ping on the screen.", &CE.Ping);
	AddBool("Map", "Draw the map you are playing on the screen.", &CE.Map);
	AddBool("Game Mode", "Draw your gamemode you are playing on the screen.", &CE.GameMode);
	AddBool("Host", "Draw your host of the lobby on the screen.", &CE.Host);
	AddBool("Number of Entities", "Draw your draw the number of entities on the screen.", &CE.NumberOfEnt);
	AddBool("Number of Clients", "Draw your draw the number of connected clients on the screen.", &CE.NumberOfClients);


	CreateSub("User Gamertags", &Menu.MenuIDs.UserGamertags, &Menu.MenuIDs.NameMenu);
	for (int i = 0; i < Menu.MaxRows - 2; i++) {

		if (cuser_strcmp(CE.UserSetGTs[i], ""))
			AddTextBoxCall("Empty", "", &CE.UserSetGTs[i][0], "", 32, SetName);
		else
			AddTextBoxCall(CE.UserSetGTs[i], "", &CE.UserSetGTs[i][0], CE.UserSetGTs[i], 32, SetName);
	}
	AddFuctionCall("Save User Gamertags", "Save User Gamertags to the \"HDD://xbOnline//UserGamertags.txt\"", SaveUserNames);
	AddFuctionCall("Load User Gamertags", "Load User Gamertags from the \"HDD://xbOnline//UserGamertags.txt\"", LoadUserNames);


	CreateSub("Name Menu", &Menu.MenuIDs.NameMenu, &Menu.MenuIDs.Main);
	AddFuctionCall("Change Gamertag", "", ChangeGamertag);
	AddFuctionCall("Change Clantag", "", Changeclantag);
	AddSubMenuBranch("User Gamertags", "Gamertags loaded from the \"HDD://xbOnline//UserGamertags.txt\"", &Menu.MenuIDs.UserGamertags);
	AddSubMenuBranch("Gamertag Looper List", "", &Menu.MenuIDs.GamertagLooperList);
	AddBoolCall("Gamertag Looper", "Loop Through the gamertags set in \"Gamertag Looper List\".", GamertagLooper, &CE.GamertagLooper);
	AddIntProgressMS("Gamertag Looper Speed", "", &CE.GamertagLooperSpeed, 100, 100, 1000);
	//const char* WAWNamesArray[] = { "None", "RAIN", "CYCL", "KRDR", "MOVE" };
	//AddStringArray("WAW Style Names", "", &CE.WAWNames, WAWNamesArray, 5);
	AddBool("Gamertag Stealer", "Steal the Gamertag of the player you last killed.", &CE.GamertagStealer);
	AddBoolCall("Flashing Gamertag", "Make your Gamertag flash diferent colours.", FlashingGmaertag, &CE.RainbowGamertag);
	AddIntProgressMS("Flashing Gamertag Speed", "", &CE.RainbowLooperSpeed, 100, 100, 1000);
	AddSubMenuBranch("Preset Gamertags", "", &Menu.MenuIDs.PreSetNames);
	AddSubMenuBranch("Preset Clantags", "", &Menu.MenuIDs.PresetClan);
	AddFuctionCall("Reset Gamertag", "Reset your Gamertag back to your original one.", ResetGamertag);


	CreateSub("Gamertag Looper List", &Menu.MenuIDs.GamertagLooperList, &Menu.MenuIDs.NameMenu);
	for (int i = 0; i < 10; i++)
	{
		if (cuser_strcmp(CE.GamertagLooperList[i], ""))
			AddTextBox("Empty", "", &CE.GamertagLooperList[i][0], "", 32);
		else
			AddTextBox(CE.GamertagLooperList[i], "", &CE.GamertagLooperList[i][0], CE.GamertagLooperList[i], 32);
	}
	AddFuctionCall("Save Looper Gamertags", "Save looper Gamertags to the \"HDD://xbOnline//LooperGamerTags.txt\"", SaveLooperNames);
	AddFuctionCall("Load Looper Gamertags", "Load looper Gamertags from the \"HDD://xbOnline//LooperGamerTags.txt\"", LoadLooperNames);


	CreateSub("Preset Names", &Menu.MenuIDs.PreSetNames, &Menu.MenuIDs.NameMenu);
	AddFuctionCall("The Aperture Project", "", SetName, "The Aperture Project");
	AddFuctionCall("xbOnline.live", "", SetName, "xbOnline.live");
	AddFuctionCall("OLDSCHOOLMODZHD", "", SetName, "OLDSCHOOLMODZHD");
	AddFuctionCall("Nigel", "", SetName, "Nigel");
	AddFuctionCall("el Sabotage", "", SetName, "el Sabotage");
	AddFuctionCall("Krazaki", "", SetName, "Krazaki");
	AddFuctionCall("Lachie", "", SetName, "Lachie");
	AddFuctionCall("Exile", "", SetName, "Exile");
	AddFuctionCall("Blasts Mods", "", SetName, "Blasts Mods");


	CreateSub("Preset Clantags", &Menu.MenuIDs.PresetClan, &Menu.MenuIDs.NameMenu);
	AddFuctionCall("IW", "", SetClantag, "IW");
	AddFuctionCall("3arc", "", SetClantag, "3arc");
	AddFuctionCall("FUCK", "", SetClantag, "FUCK");
	AddFuctionCall("JTAG", "", SetClantag, "JTAG");
	AddFuctionCall("RGH", "", SetClantag, "RGH");


	CreateSub("HvH Menu", &Menu.MenuIDs.HVH, &Menu.MenuIDs.Main);
	const char* YArray[] = { "None", "Riot", "Spin", "Fake-Static", "Down", "Up" };
	AddStringArray("Pitch", "", &CE.AAPitch, YArray, 6);
	const char* XArray[] = { "None", "Riot", "Spin", "Fake-Static", "Jitter", "Fake-Jitter", "Fake-90" };
	AddStringArray("Yaw", "", &CE.AAYaw, XArray, 7);
	const char* ZArray[] = { "None", "Spin" };
	AddStringArray("Roll", "", &CE.AARoll, ZArray, 2);
	AddFloatProgress("Yaw Spin Speed", "", &CE.SpinSpeed, 1, 1, 20);
	AddFloatProgress("Roll Spin Speed", "", &CE.RollSpinSpeed, 1, 1, 20);
	AddIntProgressMS("Fake Lag", "", &CE.FakeLag, 100, 0, 1000);
	AddBool("Crouch on Damage", "Automatically crouch when you take more than 5 damage.", &CE.CrouchOnDamage);
	const char* AutoCroucArray[] = { "None", "Constant", "Constant On Visible", "Constant on Killable", "On Visible", "On Killable" };
	AddStringArray("Auto Crouch", "When enabled it will automatically crouch for you. Is best for HvH or just fun :).", &CE.AutoCrouch, AutoCroucArray, 6);
	AddIntProgressMS("Auto Crouch Delay", "", &CE.CrouchDelay, 100, 100, 1000);


	for (int i = 0; i < Menu.MaxRows; i++) {
		if (Menu.SubType[Menu.MenuIDs.PlayerOptions][i] != SUB_EMPTY)
			user_strcpy(Menu.SubExpPlayerOptions[i], Menu.SubExp[Menu.MenuIDs.PlayerOptions][i]);
	}

	Menu.OldTabScroll = -1;
	Menu.CurrentSub = Menu.MenuIDs.Main;
	Menu.TabMenuIDs[0] = Menu.MenuIDs.Main;
	Menu.TabMenuIDs[1] = Menu.MenuIDs.Aimbot;
	Menu.TabMenuIDs[2] = Menu.MenuIDs.Visual;
	Menu.TabMenuIDs[3] = Menu.MenuIDs.Players;
	Menu.TabMenuIDs[4] = Menu.MenuIDs.Settings;
}

void SelectOption(char MenuID, char Scroll, const char* Button)
{
	if (cuser_strcmp(Button, "GAMEPAD_A"))
	{
		if (Menu.SubType[MenuID][Scroll] == SUB_BRANCH)
			Menu.CurrentSub = *Menu.NextSub[MenuID][Scroll];
		else if (Menu.SubType[MenuID][Scroll] == SUB_FUNCTIONVOID)
			Menu.FunctionCallVoid[MenuID][Scroll]();
		else if (Menu.SubType[MenuID][Scroll] == SUB_FUNCTION)
			Menu.FunctionCall[MenuID][Scroll](Menu.PassParam[MenuID][Scroll]);
		else if (Menu.SubType[MenuID][Scroll] == SUB_FUNCTIONCHAR)
			Menu.FunctionCallChar[MenuID][Scroll]((char*)Menu.PassParam[MenuID][Scroll].String);
		else if (Menu.SubType[MenuID][Scroll] == SUB_BOOL) {
			if (Menu.CurrentSub == Menu.MenuIDs.PlayerOptions)
			{
				if (Menu.CurrentScroll[Menu.MenuIDs.PlayerOptions] == 0)
					ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].Prioritized = !ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].Prioritized;
				else if (Menu.CurrentScroll[Menu.MenuIDs.PlayerOptions] == 1)
					ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].Ignored = !ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].Ignored;
				else if (Menu.CurrentScroll[Menu.MenuIDs.PlayerOptions] == 2)
					ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].AimAtRoot = !ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].AimAtRoot;
				else
					ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].InvertTeam = !ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].InvertTeam;
			}
			else
				*Menu.BoolToggle[MenuID][Scroll] = !*Menu.BoolToggle[MenuID][Scroll];
		}
		else if (Menu.SubType[MenuID][Scroll] == SUB_BOOLCALL) {
			if (Menu.CurrentSub == Menu.MenuIDs.PlayerOptions)
			{
				if (Menu.CurrentScroll[Menu.MenuIDs.PlayerOptions] == 0)
					ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].Prioritized = !ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].Prioritized;
				else if (Menu.CurrentScroll[Menu.MenuIDs.PlayerOptions] == 1)
					ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].Ignored = !ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].Ignored;
				else if (Menu.CurrentScroll[Menu.MenuIDs.PlayerOptions] == 2)
					ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].AimAtRoot = !ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].AimAtRoot;
				else
					ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].InvertTeam = !ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].InvertTeam;
			}
			else
			{
				*Menu.BoolToggle[MenuID][Scroll] = !*Menu.BoolToggle[MenuID][Scroll];
				Menu.BoolCall[MenuID][Scroll]();
			}
		}
		else if (Menu.SubType[MenuID][Scroll] == SUB_RGB)
		{
			Menu.FloatPointer[Menu.MenuIDs.RGBEditor][0] = &Menu.RGBPointer[MenuID][Scroll]->R;
			Menu.FloatPointer[Menu.MenuIDs.RGBEditor][1] = &Menu.RGBPointer[MenuID][Scroll]->G;
			Menu.FloatPointer[Menu.MenuIDs.RGBEditor][2] = &Menu.RGBPointer[MenuID][Scroll]->B;
			Menu.FloatPointer[Menu.MenuIDs.RGBEditor][3] = &Menu.RGBPointer[MenuID][Scroll]->A;
			Menu.BoolToggle[Menu.MenuIDs.RGBEditor][4] = &Menu.RGBPointer[MenuID][Scroll]->Rainbow;
			Menu.RGBPointer[Menu.MenuIDs.RGBEditor][4] = Menu.RGBPointer[MenuID][Scroll];
			Menu.BoolCall[Menu.MenuIDs.RGBEditor][4] = Enable_Rainbow;

			Menu.RGBReturnSub = MenuID;
			Menu.LastSub[Menu.MenuIDs.RGBEditor] = &Menu.RGBReturnSub;
			Menu.CurrentSub = Menu.MenuIDs.RGBEditor;
		}
		else if (Menu.SubType[MenuID][Scroll] == SUB_STRINGARRAYCALL)
			Menu.FunctionCallVoid[MenuID][Scroll]();
		else if (Menu.SubType[MenuID][Scroll] == SUB_TEXTBOX || Menu.SubType[MenuID][Scroll] == SUB_TEXTBOXCALL) {
			static WCHAR hint[0x100], GT[32];
			char Buffer[0x100];
			_snprintf(Buffer, sizeof(Buffer), "Enter some text for the text box thats less than %i letters", Menu.MaxLen[MenuID][Scroll]);
			mbstowcs(hint, Buffer, strlen(Buffer) + 1);
			if (Menu.Text[MenuID][Scroll] && !cuser_strcmp((char*)Menu.Text[MenuID][Scroll], ""))
				mbstowcs(GT, Menu.Text[MenuID][Scroll], strlen(Menu.Text[MenuID][Scroll]) + 1);
			else
				wcscpy(GT, L"");

			ThreadKeyBoardAction((int)SetTextBox, GT, L"Set text box value", hint, Menu.MaxLen[MenuID][Scroll]);
		}
	}
	else if (cuser_strcmp(Button, "DPAD_LEFT"))
	{
		if (Menu.SubType[MenuID][Scroll] == SUB_STRINGARRAY || Menu.SubType[MenuID][Scroll] == SUB_STRINGARRAYCALL)
		{
			*Menu.StrArrayScroll[MenuID][Scroll] -= 1;
			if (*Menu.StrArrayScroll[MenuID][Scroll] < 0)
				*Menu.StrArrayScroll[MenuID][Scroll] = Menu.StrArrayMax[MenuID][Scroll];
		}
		else if (Menu.SubType[MenuID][Scroll] == SUB_FLOAT || Menu.SubType[MenuID][Scroll] == SUB_FLOATPROGRESS || Menu.SubType[MenuID][Scroll] == SUB_FLOATPROGRESSRGB)
		{
			if ((Menu.SubType[MenuID][Scroll] == SUB_FLOATPROGRESSRGB) && Menu.RGBPointer[Menu.MenuIDs.RGBEditor][4]->Rainbow)
				return;

			*Menu.FloatPointer[MenuID][Scroll] -= Menu.FloatAmmount[MenuID][Scroll];
			if (*Menu.FloatPointer[MenuID][Scroll] < Menu.FloatMin[MenuID][Scroll])
				*Menu.FloatPointer[MenuID][Scroll] = Menu.FloatMax[MenuID][Scroll];
		}
		else if (Menu.SubType[MenuID][Scroll] == SUB_INT || Menu.SubType[MenuID][Scroll] == SUB_INTCALL || Menu.SubType[MenuID][Scroll] == SUB_INTPROGRESS || Menu.SubType[MenuID][Scroll] == SUB_INTPROGRESSMS)
		{
			*Menu.IntPointer[MenuID][Scroll] -= Menu.IntAmmount[MenuID][Scroll];
			if (*Menu.IntPointer[MenuID][Scroll] < Menu.IntMin[MenuID][Scroll])
				*Menu.IntPointer[MenuID][Scroll] = Menu.IntMax[MenuID][Scroll];

			if (Menu.SubType[MenuID][Scroll] == SUB_INTCALL)
				Menu.FunctionCallVoid[MenuID][Scroll]();
		}

		UI_PlaySound("mouse_click");
		UI_PlaySound("mouse_over");
		CG_PlayRumbleOnClient("riotshield_impact");
	}
	else
	{
		if (Menu.SubType[MenuID][Scroll] == SUB_STRINGARRAY || Menu.SubType[MenuID][Scroll] == SUB_STRINGARRAYCALL)
		{
			*Menu.StrArrayScroll[MenuID][Scroll] += 1;
			if (*Menu.StrArrayScroll[MenuID][Scroll] > Menu.StrArrayMax[MenuID][Scroll])
				*Menu.StrArrayScroll[MenuID][Scroll] = 0;
		}
		else if (Menu.SubType[MenuID][Scroll] == SUB_FLOAT || Menu.SubType[MenuID][Scroll] == SUB_FLOATPROGRESS || Menu.SubType[MenuID][Scroll] == SUB_FLOATPROGRESSRGB)
		{
			if ((Menu.SubType[MenuID][Scroll] == SUB_FLOATPROGRESSRGB) && Menu.RGBPointer[Menu.MenuIDs.RGBEditor][4]->Rainbow)
				return;

			*Menu.FloatPointer[MenuID][Scroll] += Menu.FloatAmmount[MenuID][Scroll];
			if (*Menu.FloatPointer[MenuID][Scroll] > Menu.FloatMax[MenuID][Scroll])
				*Menu.FloatPointer[MenuID][Scroll] = Menu.FloatMin[MenuID][Scroll];
		}
		else if (Menu.SubType[MenuID][Scroll] == SUB_INT || Menu.SubType[MenuID][Scroll] == SUB_INTCALL || Menu.SubType[MenuID][Scroll] == SUB_INTPROGRESS || Menu.SubType[MenuID][Scroll] == SUB_INTPROGRESSMS)
		{
			*Menu.IntPointer[MenuID][Scroll] += Menu.IntAmmount[MenuID][Scroll];
			if (*Menu.IntPointer[MenuID][Scroll] > Menu.IntMax[MenuID][Scroll])
				*Menu.IntPointer[MenuID][Scroll] = Menu.IntMin[MenuID][Scroll];

			if (Menu.SubType[MenuID][Scroll] == SUB_INTCALL)
				Menu.FunctionCallVoid[MenuID][Scroll]();
		}

		UI_PlaySound("mouse_click");
		UI_PlaySound("mouse_over");
		CG_PlayRumbleOnClient("riotshield_impact");
	}
}

void ScrollSubs(char MenuID, char Direction)
{
	if (Direction)
	{
		Menu.CurrentScroll[MenuID]--;
		if (Menu.CurrentScroll[MenuID] < 0)
			Menu.CurrentScroll[MenuID] = (Menu.MaxScroll[MenuID] - 1);
	}
	else
	{
		Menu.CurrentScroll[MenuID]++;
		if (Menu.CurrentScroll[MenuID] > (Menu.MaxScroll[MenuID] - 1))
			Menu.CurrentScroll[MenuID] = 0;
	}

	UI_PlaySound("mouse_over");
	CG_PlayRumbleOnClient("riotshield_impact");
}

void ScrollTabs(char Direction)
{
	if (Direction)
	{
		Menu.TabScroll++;
		if (Menu.TabScroll > 4)
			Menu.TabScroll = 0;
	}
	else
	{
		Menu.TabScroll--;
		if (Menu.TabScroll < 0)
			Menu.TabScroll = 4;
	}
	UI_PlaySound("mouse_over");
	CG_PlayRumbleOnClient("riotshield_impact");
	Menu.CurrentSub = Menu.TabMenuIDs[Menu.TabScroll];
}

void ScrollCurrentArray(int Scroll, int MenuID, char Direction)
{
	if (Direction)
	{
		if (Menu.SubType[MenuID][Scroll] == SUB_STRINGARRAY || Menu.SubType[MenuID][Scroll] == SUB_STRINGARRAYCALL)
		{
			*Menu.StrArrayScroll[MenuID][Scroll] += 1;
			if (*Menu.StrArrayScroll[MenuID][Scroll] > Menu.StrArrayMax[MenuID][Scroll])
				*Menu.StrArrayScroll[MenuID][Scroll] = 0;
		}
		else if (Menu.SubType[MenuID][Scroll] == SUB_FLOAT || Menu.SubType[MenuID][Scroll] == SUB_FLOATPROGRESS || Menu.SubType[MenuID][Scroll] == SUB_FLOATPROGRESSRGB)
		{
			if ((Menu.SubType[MenuID][Scroll] == SUB_FLOATPROGRESSRGB) && Menu.RGBPointer[Menu.MenuIDs.RGBEditor][4]->Rainbow)
				return;

			*Menu.FloatPointer[MenuID][Scroll] += Menu.FloatAmmount[MenuID][Scroll];
			if (*Menu.FloatPointer[MenuID][Scroll] > Menu.FloatMax[MenuID][Scroll])
				*Menu.FloatPointer[MenuID][Scroll] = Menu.FloatMin[MenuID][Scroll];
		}
		else if (Menu.SubType[MenuID][Scroll] == SUB_INT || Menu.SubType[MenuID][Scroll] == SUB_INTCALL || Menu.SubType[MenuID][Scroll] == SUB_INTPROGRESS || Menu.SubType[MenuID][Scroll] == SUB_INTPROGRESSMS)
		{
			*Menu.IntPointer[MenuID][Scroll] += Menu.IntAmmount[MenuID][Scroll];
			if (*Menu.IntPointer[MenuID][Scroll] > Menu.IntMax[MenuID][Scroll])
				*Menu.IntPointer[MenuID][Scroll] = Menu.IntMin[MenuID][Scroll];

			if (Menu.SubType[MenuID][Scroll] == SUB_INTCALL)
				Menu.FunctionCallVoid[MenuID][Scroll]();
		}
	}
	else
	{
		if (Menu.SubType[MenuID][Scroll] == SUB_STRINGARRAY || Menu.SubType[MenuID][Scroll] == SUB_STRINGARRAYCALL)
		{
			*Menu.StrArrayScroll[MenuID][Scroll] -= 1;
			if (*Menu.StrArrayScroll[MenuID][Scroll] < 0)
				*Menu.StrArrayScroll[MenuID][Scroll] = Menu.StrArrayMax[MenuID][Scroll];
		}
		else if (Menu.SubType[MenuID][Scroll] == SUB_FLOAT || Menu.SubType[MenuID][Scroll] == SUB_FLOATPROGRESS || Menu.SubType[MenuID][Scroll] == SUB_FLOATPROGRESSRGB)
		{
			if ((Menu.SubType[MenuID][Scroll] == SUB_FLOATPROGRESSRGB) && Menu.RGBPointer[Menu.MenuIDs.RGBEditor][4]->Rainbow)
				return;

			*Menu.FloatPointer[MenuID][Scroll] -= Menu.FloatAmmount[MenuID][Scroll];
			if (*Menu.FloatPointer[MenuID][Scroll] < Menu.FloatMin[MenuID][Scroll])
				*Menu.FloatPointer[MenuID][Scroll] = Menu.FloatMax[MenuID][Scroll];
		}
		else if (Menu.SubType[MenuID][Scroll] == SUB_INT || Menu.SubType[MenuID][Scroll] == SUB_INTCALL || Menu.SubType[MenuID][Scroll] == SUB_INTPROGRESS || Menu.SubType[MenuID][Scroll] == SUB_INTPROGRESSMS)
		{
			*Menu.IntPointer[MenuID][Scroll] -= Menu.IntAmmount[MenuID][Scroll];
			if (*Menu.IntPointer[MenuID][Scroll] < Menu.IntMin[MenuID][Scroll])
				*Menu.IntPointer[MenuID][Scroll] = Menu.IntMax[MenuID][Scroll];

			if (Menu.SubType[MenuID][Scroll] == SUB_INTCALL)
				Menu.FunctionCallVoid[MenuID][Scroll]();
		}
	}

	UI_PlaySound("mouse_over");
	CG_PlayRumbleOnClient("riotshield_impact");
}

float fHintH;
void DrawMenu()
{
	UiContext uicontext = *(UiContext*)0x83D51E20;

	if ((Menu.MenuStatus == 1) && (!CE.IsMenuOpen))
	{
		float BaseLocation = uicontext.screenWidth / 2;
		float BaseLocationY = uicontext.screenHeight / 2;

		DrawBoxNoTopMenu("white", BaseLocation - (320 / 2), BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2)), 320, (Menu.MaxScroll[Menu.CurrentSub] * 21) + 8, 3, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.Scroller ? 0 : CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);

		if (CE.Scroller)
		{
			SetShader("white", BaseLocation - (320 / 2), BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2)), 320, ((Menu.CurrentScroll[Menu.CurrentSub]) * 21) + 4, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A); //Top BG
			SetShader("white", BaseLocation - (320 / 2), BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 4) + ((Menu.CurrentScroll[Menu.CurrentSub] * 21)), 320, 21, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A); //Scroller
			SetShader("white", BaseLocation - (320 / 2), BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2)) + ((((Menu.CurrentScroll[Menu.CurrentSub] + 1) * 21)) + 4), 320, ((Menu.MaxScroll[Menu.CurrentSub] - (Menu.CurrentScroll[Menu.CurrentSub] + 1)) * 21) + 4, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A); //Bottom BG
		}

		float MainH = 25, AimbotH = 25, VisualH = 25, PlayersH = 25, SettingsH = 25;
		float MainA = 0.12, AimbotA = 0.12, VisualA = 0.12, PlayersA = 0.12, SettingsA = 0.12;
		if (Menu.TabScroll == 0) {
			MainA = CE.MenuSecondaryColour.A;
			MainH += 3;
		}
		else if (Menu.TabScroll == 1) {
			AimbotA = CE.MenuSecondaryColour.A;
			AimbotH += 3;
		}
		else if (Menu.TabScroll == 2) {
			VisualA = CE.MenuSecondaryColour.A;
			VisualH += 3;
		}
		else if (Menu.TabScroll == 3) {
			PlayersA = CE.MenuSecondaryColour.A;
			PlayersH += 3;
		}
		else if (Menu.TabScroll == 4) {
			SettingsA = CE.MenuSecondaryColour.A;
			SettingsH += 3;
		}

		if (Menu.CurrentSub > Menu.MenuIDs.Settings)
		{
			char Title[0x200];
			if (Menu.CurrentSub == Menu.MenuIDs.RGBEditor)
				_snprintf(Title, sizeof(Title), "%s (%s)", Menu.SubTitle[Menu.CurrentSub], Menu.SubText[*Menu.LastSub[Menu.CurrentSub]][Menu.CurrentScroll[*Menu.LastSub[Menu.CurrentSub]]]);
			else
				user_strcpy(Title, Menu.SubTitle[Menu.CurrentSub]);

			SetShader("white", (BaseLocation - (326 / 2)), BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + (4 + TextHeight("normalFont", 0.5))), 326, 3, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A); //Top
			SetShader("white", (BaseLocation - (326 / 2)), (BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 4) - (TextHeight("normalFont", 0.5) - 3)), 326, TextHeight("normalFont", 0.5) - 2, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A); //BG
			SetTextCentered(Title, "normalFont", BaseLocation, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 3), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A);
			SetShader("white", (BaseLocation - (320 / 2)), BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 3), 320, 3, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A); //Bottom
		}
		else
		{
			SetShader("white", (BaseLocation - (320 / 2)), BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 28), 47, MainH, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, MainA); //MainMods BG
			SetShader("white", (BaseLocation - (320 / 2)) - 3, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 28), 3, 25, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, MainA); //MainMods Left
			SetShader("white", (BaseLocation - (320 / 2)) - 3, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 31), 50, 3, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, MainA); //MainMods Top
			SetShader("white", (BaseLocation - (320 / 2)) + 47, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 31), 3, (Menu.TabScroll == 0) ? 31 : 28, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, (Menu.TabScroll == 0 || Menu.TabScroll == 1) ? CE.MenuPrimaryColour.A : MainA); //MainMods Right
			SetShader("white", (BaseLocation - (320 / 2)), BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 3), 50, 3, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, (Menu.TabScroll == 0) ? 0 : CE.MenuPrimaryColour.A); //MainMods Block

			SetShader("white", (BaseLocation - (320 / 2)) + 50, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 28), 67, AimbotH, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, AimbotA); //Aimbot BG
			SetShader("white", (BaseLocation - (320 / 2)) + 50, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 31), 67, 3, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, AimbotA); //Aimbot Top
			SetShader("white", (BaseLocation - (320 / 2)) + 117, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 31), 3, (Menu.TabScroll == 1) ? 31 : 28, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, (Menu.TabScroll == 1 || Menu.TabScroll == 2) ? CE.MenuPrimaryColour.A : AimbotA); //Aimbot Right
			SetShader("white", (BaseLocation - (320 / 2)) + 50, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 3), 70, 3, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, (Menu.TabScroll == 1) ? 0 : 0.43f); //Aimbot Block

			SetShader("white", (BaseLocation - (320 / 2)) + 120, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 28), 58, VisualH, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, VisualA); //Visual BG
			SetShader("white", (BaseLocation - (320 / 2)) + 120, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 31), 58, 3, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, VisualA); //Visual Top
			SetShader("white", (BaseLocation - (320 / 2)) + 178, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 31), 3, (Menu.TabScroll == 2) ? 31 : 28, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, (Menu.TabScroll == 2 || Menu.TabScroll == 3) ? CE.MenuPrimaryColour.A : VisualA); //Visual Right
			SetShader("white", (BaseLocation - (320 / 2)) + 120, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 3), 61, 3, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, (Menu.TabScroll == 2) ? 0 : 0.43f); //Visual Block

			SetShader("white", (BaseLocation - (320 / 2)) + 181, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 28), 65, PlayersH, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, PlayersA); //Players BG
			SetShader("white", (BaseLocation - (320 / 2)) + 181, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 31), 65, 3, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, PlayersA); //Players Top
			SetShader("white", (BaseLocation - (320 / 2)) + 246, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 31), 3, (Menu.TabScroll == 3) ? 31 : 28, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, (Menu.TabScroll == 3 || Menu.TabScroll == 4) ? CE.MenuPrimaryColour.A : PlayersA); //Players Right
			SetShader("white", (BaseLocation - (320 / 2)) + 181, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 3), 68, 3, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, (Menu.TabScroll == 3) ? 0 : 0.43f); //Players Block

			SetShader("white", (BaseLocation - (320 / 2)) + 249, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 28), 71, SettingsH, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, SettingsA); //Settings BG
			SetShader("white", (BaseLocation - (320 / 2)) + 249, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 31), 71, 3, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, SettingsA); //Settings Top
			SetShader("white", (BaseLocation - (320 / 2)) + 320, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 31), 3, 28, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, (Menu.TabScroll == 4) ? CE.MenuPrimaryColour.A : SettingsA); //Settings Right
			SetShader("white", (BaseLocation - (320 / 2)) + 249, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 3), 71, 3, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, (Menu.TabScroll == 4) ? 0 : 0.43f); //Settings Block

			SetTextCentered("Main", "normalFont", (BaseLocation - (320 / 2)) + (47 / 2), BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 4), 0.55, 0.55, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, (Menu.TabScroll == 0) ? CE.TextColour.A : 0.5);
			SetTextCentered("Aimbot", "normalFont", (BaseLocation - (320 / 2)) + 50 + (67 / 2), BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 4), 0.55, 0.55, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, (Menu.TabScroll == 1) ? CE.TextColour.A : 0.5);
			SetTextCentered("Visual", "normalFont", (BaseLocation - (320 / 2)) + 120 + (58 / 2), BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 4), 0.55, 0.55, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, (Menu.TabScroll == 2) ? CE.TextColour.A : 0.5);
			SetTextCentered("Players", "normalFont", (BaseLocation - (320 / 2)) + 181 + (65 / 2), BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 4), 0.55, 0.55, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, (Menu.TabScroll == 3) ? CE.TextColour.A : 0.5);
			SetTextCentered("Settings", "normalFont", (BaseLocation - (320 / 2)) + 249 + (71 / 2), BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) + 4), 0.55, 0.55, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, (Menu.TabScroll == 4) ? CE.TextColour.A : 0.5);
		}

		UpdateClientList();

		for (int i = 0; i < Menu.MaxRows; i++) {
			if (Menu.SubType[Menu.CurrentSub][i] != SUB_EMPTY) {
				if (Menu.SubType[Menu.CurrentSub][i] == SUB_TEXTBOX || Menu.SubType[Menu.CurrentSub][i] == SUB_TEXTBOXCALL) {
					if (Menu.Text[Menu.CurrentSub][i] && !cuser_strcmp((char*)Menu.Text[Menu.CurrentSub][i], ""))
						user_strcpy(Menu.SubText[Menu.CurrentSub][i], (char*)Menu.Text[Menu.CurrentSub][i]);
					else
						user_strcpy(Menu.SubText[Menu.CurrentSub][i], "Empty");
				}

				if (Menu.CurrentSub == Menu.MenuIDs.PlayerOptions)
				{
					if (ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].LoadedFromCFG && cuser_strcmp(Menu.SubText[Menu.CurrentSub][i], "Remember Player"))
						strcpy(Menu.SubText[Menu.CurrentSub][i], "Update Player");
					else if (!ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].LoadedFromCFG && cuser_strcmp(Menu.SubText[Menu.CurrentSub][i], "Update Player"))
						strcpy(Menu.SubText[Menu.CurrentSub][i], "Remember Player");
				}

				if ((i == Menu.CurrentScroll[Menu.CurrentSub]) && !CE.Scroller)
					SetText(Menu.SubText[Menu.CurrentSub][i], "normalFont", (BaseLocation - (320 / 2)) + 10, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 23) + (21 * i), 0.5, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, 1);
				else
					SetText(Menu.SubText[Menu.CurrentSub][i], "normalFont", (BaseLocation - (320 / 2)) + 10, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 23) + (21 * i), 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A);
			}
		}

		for (int i = 0; i < Menu.MaxRows; i++) {
			if (Menu.SubType[Menu.CurrentSub][i] == SUB_BOOL || Menu.SubType[Menu.CurrentSub][i] == SUB_BOOLCALL)
			{
				bool status = *Menu.BoolToggle[Menu.CurrentSub][i];
				if (Menu.CurrentSub == Menu.MenuIDs.PlayerOptions)
				{
					if (i == 0)
						status = ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].Prioritized;
					else if (i == 1)
						status = ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].Ignored;
					else if (i == 2)
						status = ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].AimAtRoot;
					else
						status = ClientOptions[Menu.CurrentScroll[Menu.MenuIDs.Players]].InvertTeam;
				}

				if (CE.BoolType == Text)
					status ? SetTextRight("ON", "normalFont", (BaseLocation - (320 / 2)) + 310, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 23) + (21 * i), 0.5, 0.5, 0, 1, 0, 1) : SetTextRight("OFF", "normalFont", (BaseLocation - (320 / 2)) + 310, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 23) + (21 * i), 0.5, 0.5, 1, 0, 0, 1);
				else if (CE.BoolType == CheckBox)
					DrawCheckBox("box_check", (BaseLocation - (320 / 2)) + 295, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 23) + (21 * i) - 16, 15, 15, 1, 1, 1, 1, status);
				else if (CE.BoolType == FillBox)
					DrawCheckBox("white", (BaseLocation - (320 / 2)) + 295, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 23) + (21 * i) - 16, 15, 15, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, 0.5, status);
				else
					DrawSwitch((BaseLocation - (320 / 2)) + 295, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 23) + (21 * i) - 16, 30, 15, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, 0.5, status);
			}
			else if (Menu.SubType[Menu.CurrentSub][i] == SUB_FLOAT)
				SetTextRight(va("%0.2f", *Menu.FloatPointer[Menu.CurrentSub][i]), "normalFont", (BaseLocation - (320 / 2)) + 310, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 23) + (21 * i), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A);
			else if (Menu.SubType[Menu.CurrentSub][i] == SUB_INT || Menu.SubType[Menu.CurrentSub][i] == SUB_INTCALL)
				SetTextRight(va("%i", *Menu.IntPointer[Menu.CurrentSub][i]), "normalFont", (BaseLocation - (320 / 2)) + 310, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 23) + (21 * i), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A);
			else if (Menu.SubType[Menu.CurrentSub][i] == SUB_BRANCH)
				SetTextRight("->", "normalFont", (BaseLocation - (320 / 2)) + 310, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 23) + (21 * i), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A);
			else if (Menu.SubType[Menu.CurrentSub][i] == SUB_FLOATPROGRESS)
			{
				SetTextRight(va("%0.2f", *Menu.FloatPointer[Menu.CurrentSub][i]), "normalFont", (((BaseLocation - (320 / 2)) + 220)), BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 23) + (21 * i), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A);
				float ForeColour[] = { 1, 1, 1, 1 }; float BackColour[] = { 0, 0, 0, 0.5 };
				DrawProgressBar((BaseLocation - (320 / 2)) + 310, (BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 25) + (21 * i)) - (TextHeight("normalFont", 0.5) / 2), 80, 5, Menu.FloatMin[Menu.CurrentSub][i], Menu.FloatMax[Menu.CurrentSub][i], *Menu.FloatPointer[Menu.CurrentSub][i], ForeColour, BackColour);
			}
			else if (Menu.SubType[Menu.CurrentSub][i] == SUB_FLOATPROGRESSRGB)
			{
				SetTextRight(va("%0.2f", (*Menu.FloatPointer[Menu.CurrentSub][i] * 255.0f)), "normalFont", (((BaseLocation - (320 / 2)) + 220)), BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 23) + (21 * i), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A);
				float ForeColour[] = { 1, 1, 1, 1 }; float BackColour[] = { 0, 0, 0, 0.5 };
				DrawProgressBar((BaseLocation - (320 / 2)) + 310, (BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 25) + (21 * i)) - (TextHeight("normalFont", 0.5) / 2), 80, 5, Menu.FloatMin[Menu.CurrentSub][i], Menu.FloatMax[Menu.CurrentSub][i], *Menu.FloatPointer[Menu.CurrentSub][i], *Menu.FloatPointer[Menu.MenuIDs.RGBEditor], BackColour);
			}
			else if (Menu.SubType[Menu.CurrentSub][i] == SUB_INTPROGRESS || Menu.SubType[Menu.CurrentSub][i] == SUB_INTPROGRESSMS)
			{
				if (Menu.SubType[Menu.CurrentSub][i] == SUB_INTPROGRESSMS)
					SetTextRight(va("%i ms", *Menu.IntPointer[Menu.CurrentSub][i]), "normalFont", (((BaseLocation - (320 / 2)) + 220)), BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 23) + (21 * i), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A);
				else
					SetTextRight(va("%i", *Menu.IntPointer[Menu.CurrentSub][i]), "normalFont", (((BaseLocation - (320 / 2)) + 220)), BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 23) + (21 * i), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A);

				float ForeColour[] = { 1, 1, 1, 1 }; float BackColour[] = { 0, 0, 0, 0.5 };
				DrawProgressBar((BaseLocation - (320 / 2)) + 310, (BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 25) + (21 * i)) - (TextHeight("normalFont", 0.5) / 2), 80, 5, Menu.IntMin[Menu.CurrentSub][i], Menu.IntMax[Menu.CurrentSub][i], *Menu.IntPointer[Menu.CurrentSub][i], ForeColour, BackColour);
			}
			else if (Menu.SubType[Menu.CurrentSub][i] == SUB_STRINGARRAY || Menu.SubType[Menu.CurrentSub][i] == SUB_STRINGARRAYCALL)
			{
				if (CE.ArrayStyle == Single || Menu.SubType[Menu.CurrentSub][i] == SUB_STRINGARRAYCALL)
					SetTextRight(Menu.StrArray[Menu.CurrentSub][i][*Menu.StrArrayScroll[Menu.CurrentSub][i]], "normalFont", (BaseLocation - (320 / 2)) + 310, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 23) + (21 * i), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A);
				else if (CE.ArrayStyle == DropDown) {
					if (CE.IsDropDownOpen && Menu.CurrentScroll[Menu.CurrentSub] == i)
						continue;

					DropDownText(Menu.StrArray[Menu.CurrentSub][i][*Menu.StrArrayScroll[Menu.CurrentSub][i]], "normalFont", (BaseLocation - (320 / 2)) + 310, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 23) + (21 * i) + 1, 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A, 0, 0, 0, 1, 0, 0, 0, 0);
				}
			}
			else if (Menu.SubType[Menu.CurrentSub][i] == SUB_RGB)
				DrawCheckBox("white", (BaseLocation - (320 / 2)) + 295, BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2) - 23) + (21 * i) - 16, 15, 15, Menu.RGBPointer[Menu.CurrentSub][i]->R, Menu.RGBPointer[Menu.CurrentSub][i]->G, Menu.RGBPointer[Menu.CurrentSub][i]->B, Menu.RGBPointer[Menu.CurrentSub][i]->A, true);
		}

		if (CE.MenuHints || (Menu.CurrentSub == Menu.MenuIDs.PlayerOptions || Menu.CurrentSub == Menu.MenuIDs.Players)) {
			if (fHintH > 0)
				DrawBoxNoTop("white", (BaseLocation - (320 / 2)), (BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2)) - 6) + (((Menu.MaxScroll[Menu.CurrentSub] * 21)) + TextHeight("normalFont", 0.5)), 320, (fHintH + 8), 3, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);
			fHintH = UI_DrawWrappedText(Menu.SubExp[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]], "normalFont", ((BaseLocation - (320 / 2)) + 9), (BaseLocationY - (((Menu.MaxScroll[Menu.CurrentSub] * 21) / 2)) + 15) + (((Menu.MaxScroll[Menu.CurrentSub] * 21)) + TextHeight("normalFont", 0.5)), 300, 200, 0.5, 0, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A);
		}
	}
}

int NonHostwait, NonHostTick;
void DoNonHostWait(int time) {
	NonHostTick = GetTickCount();
	NonHostwait = time;
}

void MenuMonitor()
{
	if (((GetTickCount() - NonHostTick) > NonHostwait) && !CE.IsMenuOpen)
	{
		if (Menu.MenuStatus == 2)
			Menu.MenuStatus = 0;
		if (!Menu.MenuStatus)
		{
			if (GetAsyncKeyState(XINPUT_GAMEPAD_DPAD_UP) && GetAsyncKeyState(XINPUT_GAMEPAD_RIGHT_THUMB))
			{
				if (Menu.OldTabScroll != -1)
				{
					Menu.TabScroll = Menu.OldTabScroll;
					Menu.CurrentSub = Menu.TabMenuIDs[Menu.OldTabScroll];
					Menu.OldTabScroll = -1;
				}

				Menu.MenuStatus = 1;
				UI_PlaySound("mp_bonus_start");
				CG_PlayRumbleOnClient("smg_fire");
				DoNonHostWait(200);
			}
			else if (GetAsyncKeyState(XINPUT_GAMEPAD_DPAD_DOWN) && GetAsyncKeyState(XINPUT_GAMEPAD_RIGHT_THUMB))
			{
				Menu.OldTabScroll = Menu.TabScroll;
				Menu.TabScroll = 3;
				Menu.CurrentSub = Menu.MenuIDs.Players;

				Menu.MenuStatus = 1;
				UI_PlaySound("mouse_over");
				CG_PlayRumbleOnClient("smg_fire");
				DoNonHostWait(200);
			}
		}
		else
		{
			if (CE.IsDropDownOpen)
			{
				if (CE.ArrayStyle == DropDown)
				{
					if (GetAsyncKeyState(XINPUT_GAMEPAD_DPAD_UP))
					{
						ScrollCurrentArray(Menu.CurrentScroll[Menu.CurrentSub], Menu.CurrentSub, 0);
						DoNonHostWait(140);
					}
					else if (GetAsyncKeyState(XINPUT_GAMEPAD_DPAD_DOWN))
					{
						ScrollCurrentArray(Menu.CurrentScroll[Menu.CurrentSub], Menu.CurrentSub, 1);
						DoNonHostWait(140);
					}
					else if (GetAsyncKeyState(XINPUT_GAMEPAD_A))
					{
						UI_PlaySound("mouse_over");
						CG_PlayRumbleOnClient("riotshield_impact");
						CE.IsDropDownOpen = false;
						DoNonHostWait(180);
					}
				}
				else
					CE.IsDropDownOpen = false;
			}
			else
			{
				if (GetAsyncKeyState(XINPUT_GAMEPAD_DPAD_UP))
				{
					ScrollSubs(Menu.CurrentSub, 1);
					DoNonHostWait(140);
				}
				else if (GetAsyncKeyState(XINPUT_GAMEPAD_DPAD_DOWN))
				{
					ScrollSubs(Menu.CurrentSub, 0);
					DoNonHostWait(140);
				}
				else if (GetAsyncKeyState(XINPUT_GAMEPAD_A))
				{
					if (Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_TEXTBOXCALL || Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_TEXTBOX || Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_STRINGARRAYCALL || Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_RGB || Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_BOOL || Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_BOOLCALL || Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_BRANCH || Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_FUNCTIONVOID || Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_FUNCTIONCHAR) {
						SelectOption(Menu.CurrentSub, Menu.CurrentScroll[Menu.CurrentSub], "GAMEPAD_A");
						UI_PlaySound("mouse_click");
						UI_PlaySound("mouse_over");
						CG_PlayRumbleOnClient("riotshield_impact");
						DoNonHostWait(180);
					}
					else
					{
						if (CE.ArrayStyle == DropDown)
						{
							CE.IsDropDownOpen = true;
							UI_PlaySound("mouse_click");
							UI_PlaySound("mouse_over");
							CG_PlayRumbleOnClient("riotshield_impact");
							DoNonHostWait(180);
						}
					}
				}
				else if (GetAsyncKeyState(XINPUT_GAMEPAD_B))
				{
					UI_PlaySound("mouse_click");
					CG_PlayRumbleOnClient("riotshield_impact");
					if (*Menu.LastSub[Menu.CurrentSub] == NONE) {
						Menu.MenuStatus = 2;
						DoNonHostWait(200);
						return;
					}
					else {
						Menu.CurrentSub = *Menu.LastSub[Menu.CurrentSub];
						DoNonHostWait(190);
					}
				}

				if (*Menu.LastSub[Menu.CurrentSub] == NONE)
				{
					if (GetAsyncKeyState(XINPUT_GAMEPAD_RIGHT_SHOULDER))
					{
						ScrollTabs(1);
						DoNonHostWait(160);
					}
					else if (GetAsyncKeyState(XINPUT_GAMEPAD_LEFT_SHOULDER))
					{
						ScrollTabs(0);
						DoNonHostWait(160);
					}
				}

				if (Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_TEXTBOX || Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_TEXTBOXCALL)
				{
					if (GetAsyncKeyState(XINPUT_GAMEPAD_Y) && !cuser_strcmp(Menu.Text[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]], "")) {
						strcpy(Menu.Clipboard, Menu.Text[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]]);
						UI_PlaySound("mouse_click");
						CG_PlayRumbleOnClient("riotshield_impact");
					}
					else if (GetAsyncKeyState(XINPUT_GAMEPAD_X) && !cuser_strcmp(Menu.Clipboard, "")) {
						strcpy(Menu.Text[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]], Menu.Clipboard);
						UI_PlaySound("mouse_click");
						CG_PlayRumbleOnClient("riotshield_impact");
					}
					else if (GetAsyncKeyState(XINPUT_GAMEPAD_RIGHT_THUMB)) {
						strcpy(Menu.Text[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]], "");
						UI_PlaySound("mouse_click");
						CG_PlayRumbleOnClient("riotshield_impact");
					}
					else if (GetAsyncKeyState(XINPUT_GAMEPAD_START)) {
						Menu.FunctionCallChar[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]]((char*)Menu.Text[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]]);
						UI_PlaySound("mouse_click");
						CG_PlayRumbleOnClient("riotshield_impact");
					}
				}

				if (Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] != SUB_TEXTBOX && Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] != SUB_RGB && Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] != SUB_BOOL && Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] != SUB_BOOLCALL && Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] != SUB_BRANCH && Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] != SUB_FUNCTIONVOID && Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] != SUB_FUNCTIONCHAR) {
					if (CE.ArrayStyle == Single || Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_STRINGARRAYCALL)
					{
						if (GetAsyncKeyState(XINPUT_GAMEPAD_DPAD_LEFT))
						{
							SelectOption(Menu.CurrentSub, Menu.CurrentScroll[Menu.CurrentSub], "DPAD_LEFT");
							if (Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_FLOATPROGRESSRGB)
								DoNonHostWait(100);
							else
								DoNonHostWait(170);
						}
						else if (GetAsyncKeyState(XINPUT_GAMEPAD_DPAD_RIGHT))
						{
							SelectOption(Menu.CurrentSub, Menu.CurrentScroll[Menu.CurrentSub], "DPAD_RIGHT");
							if (Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_FLOATPROGRESSRGB)
								DoNonHostWait(100);
							else
								DoNonHostWait(170);
						}
					}
				}
			}
		}
	}
}