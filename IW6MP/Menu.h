#pragma once
#include "Lib.h"

enum SnapLinesType
{
	SnapNone,
	SnapTop,
	SnapMiddle,
	SnapBottom,
};

enum ESPType
{
	ESPNone,
	ESP2D,
	ESP3D,
	FB,
	Corner,
};

enum ArrayStyle
{
	Single,
	DropDown,
};

enum CrosshairType
{
	NoCrosshair = 0,
	Plus,
	CrossCrosshair,
	Spin,
};

enum AimbotTargetType
{
	Closest,
	Visible,
	OnScreen,
	Logical,
};

enum AimbotType
{
	Normal,
	Silent,
	Snap,
};

enum Aimkey
{
	NoAimKey,
	LB,
	LT,
	RB,
	RT,
};

enum AutoWallType
{
	NoAutoWall,
	NormalAutoWall,
	pAutoWall,
	pAutoNormal,
};

enum AntiAimYaw
{
	YawNone,
	YawRiot,
	YawSpin,
	YawFakeStatic,
	Jitter,
	FakeJitter,
	Fake90,
};

enum AntiAimPitch
{
	PitchNone,
	Up,
	Down,
	Random,
};

enum BoolType
{
	Text,
	CheckBox,
	FillBox,
	Switches,
};

enum AutoCrouch
{
	disabledc,
	Constant,
	ConstantVis,
	ConstantKillable,
	OnVis,
	OnKillable,
};

extern const char* TagList[19];
extern const char* HitBoxesP[12];
extern const char* HitBoxes[10];
extern const char* RiotBoxes[14];

struct CE_s
{
	int StoredTimeForShotDelay;
	bool ShouldShoot;
	bool InverseTeams;
	bool AutoUnInfect;
	bool ShouldRunXAM_191;

	//Main
	bool NoRecoil;
	bool UAV;
	bool NoSpread;
	bool NoSway;
	bool NoFlinch;
	bool Laser;
	bool ThirdPerson;
	bool IPSpoofer;
	bool NeverHost;
	bool CrouchOnDamage;
	int AutoCrouch;
	int CrouchDelay;
	bool HearEveryone;
	int ClassNum;
	bool FlipTeams;
	float Fov;
	int FakeLag;

	//Name Menu
	bool GamertagStealer;
	bool RainbowGamertag;
	int RainbowLooperSpeed;
	bool GamertagLooper;
	int GamertagLooperSpeed;

	//Anti-Aim
	int AAPitch;
	int AAYaw;
	int AARoll;
	float SpinSpeed;
	float RollSpinSpeed;

	//Aimbot
	bool AimbotStatus;
	int AimKey;
	int TargetType;
	int AimbotType;
	int AimTag;
	bool AutoShoot;
	int ShootDelay;
	int DelayTimeFix;
	int Autowall;
	bool TargetDetails;
	bool AimPrediction;
	bool PosPrediction;
	float PosPredictionScale;
	bool PosAdjustment;
	float PosAdjustmentScale;
	bool ShowAimbotTarget;
	bool LockedOnToTarget;
	Vector3 OldAngles;
	char AimbotTag[12][0x100];

	//Visual
	int Crosshair;
	bool ProximityWarning;
	bool VisibleWarning;
	bool AimWarning;
	bool HealthBar;
	bool Impacts;
	bool ExternalRadar;
	int ESPType;
	struct
	{
		bool DrawEnemies;
		bool DrawFriendies;
		bool DrawExplosives;
		bool DrawWeapons;
		bool DrawScavengerPacks;
		bool DrawKillStreaks;
		bool DrawESP;
	}RadarSettings;

	bool DrawESP;
	struct
	{
		bool DrawEnemies;
		bool DrawFriendies;
		bool ESPBones;
		bool ESPName;
		bool ESPDistance;
		bool ESPWeaponNames;
		bool ESPWeaponShaders;
		int SnapLines;
		bool DrawExplosives;
		bool DrawWeapons;
		bool DrawScavengerPacks;
		bool DrawKillStreaks;
		bool DrawDoggos;
		bool PlayerIndicator;
		int PlayerIndicatorIcon;
		int PlayerIndicatorSize[2];
		int PlayerIndicatorDiameter;
	}ESPSettings;

	//Debug
	bool ScreenResolution;
	bool FPS;
	bool Ping;
	bool Map;
	bool GameMode;
	bool Host;
	bool NumberOfEnt;
	bool NumberOfClients;

	//Settings
	float MenuSafeArea[2];
	bool ControllerVibration;
	bool MenuSounds;
	bool killsounds;
	bool quakeprints;
	bool SpawnMessage;
	bool MenuHints;
	bool Controls;
	bool Scroller;
	bool MDetection;
	int BoolType;
	int ArrayStyle;
	bool UI_Modification;
	int ConfigType;
	int NumberOfOptions;
	bool InfiniteScrolling;

	//Menu Settings
	float FontSize;
	char ColourSet[4];
	char UserSetGTs[21][32];
	char GamertagLooperList[10][32];
	char OriginalName[32];
	char SetGamertag[32];
	char BeforeName[32];

	//Drop Dwon Array
	bool IsDropDownOpen;

	//Menu
	RGBA MenuPrimaryColour;
	RGBA MenuSecondaryColour;
	RGBA TextColour;

	//ESP
	RGBA AimbotTargetColor;
	RGBA EnemyColor;
	RGBA PriorityEnemyColor;
	RGBA FriendlyColor;
	RGBA KillableColor;
	RGBA VisibleColor;
	RGBA BonesColor;

	bool CanSeePlayer[12];
	bool IsKillable[12];
	int ClientNumber;
	int AimbotTarget;
	bool IsMenuOpen;

	void SetBaseColors()
	{
		//Menu
		MenuPrimaryColour.R = 0; MenuPrimaryColour.G = 0.6; MenuPrimaryColour.B = 1; MenuPrimaryColour.A = 0.43; MenuPrimaryColour.Rainbow = false;
		MenuSecondaryColour.R = 0; MenuSecondaryColour.G = 0; MenuSecondaryColour.B = 0; MenuSecondaryColour.A = 0.55; MenuSecondaryColour.Rainbow = false;
		TextColour.R = 1; TextColour.G = 1; TextColour.B = 1; TextColour.A = 1; TextColour.Rainbow = false;

		//ESP
		AimbotTargetColor.R = 0; AimbotTargetColor.G = 0; AimbotTargetColor.B = 1; AimbotTargetColor.A = 1; AimbotTargetColor.Rainbow = false;
		EnemyColor.R = 1; EnemyColor.G = 0; EnemyColor.B = 0; EnemyColor.A = 1; EnemyColor.Rainbow = false;
		PriorityEnemyColor.R = 0.96f; PriorityEnemyColor.G = 0.21; PriorityEnemyColor.B = 1; PriorityEnemyColor.A = 1; PriorityEnemyColor.Rainbow = false;
		FriendlyColor.R = 0; FriendlyColor.G = 1; FriendlyColor.B = 0; FriendlyColor.A = 1; FriendlyColor.Rainbow = false;
		KillableColor.R = 0.83f; KillableColor.G = 0.42f; KillableColor.B = 0; KillableColor.A = 1; KillableColor.Rainbow = false;
		VisibleColor.R = 0.97f; VisibleColor.G = 1; VisibleColor.B = 0; VisibleColor.A = 1; VisibleColor.Rainbow = false;
		BonesColor.R = 0; BonesColor.G = 0.84f; BonesColor.B = 1; BonesColor.A = 1; BonesColor.Rainbow = false;
	}

	void Reset()
	{
		SetBaseColors();
		Controls = true;
		SpawnMessage = true;
		MenuHints = true;
		MDetection = false;
		ConfigType = 4;
		Fov = 65;
		AimTag = 1;
		AimbotTarget = -1;
		BoolType = CheckBox;
		Scroller = true;
		CrouchDelay = 600;

		MenuSafeArea[0] = 60;
		MenuSafeArea[1] = 60;

		RainbowLooperSpeed = 300;
		GamertagLooperSpeed = 300;
	}
};

enum SubType_e
{
	SUB_EMPTY,
	SUB_FUNCTIONVOID,
	SUB_FUNCTION,
	SUB_FUNCTIONCHAR,
	SUB_STRINGARRAY,
	SUB_STRINGARRAYCALL,
	SUB_INT,
	SUB_INTCALL,
	SUB_INTPROGRESS,
	SUB_INTPROGRESSMS,
	SUB_FLOAT,
	SUB_FLOATPROGRESS,
	SUB_FLOATPROGRESSRGB,
	SUB_BOOL,
	SUB_BOOLCALL,
	SUB_BRANCH,
	SUB_RGB,
	SUB_TEXTBOX,
	SUB_TEXTBOXCALL
};

union Params
{
	char Byte;
	short Short;
	int Int32;
	float Float;
	bool Boolean;
	const char* String;
};

struct Menu_s
{
	static const int
		MaxSub = 40,
		MaxRows = 21;

	char MenuStatus;
	char CurrentSub;
	char *LastSub[MaxSub];
	char MaxScroll[MaxSub];
	char CurrentScroll[MaxSub];
	char TabScroll;
	char OldTabScroll;
	char TabMenuIDs[5];

	const char *SubTitle[MaxSub];
	char SubText[MaxSub][MaxRows][200];
	char SubExp[MaxSub][MaxRows][500];
	char SubType[MaxSub][MaxRows];

	char SubExpPlayerOptions[MaxRows][500];
	char* NextSub[MaxSub][MaxRows];

	void(*FunctionCallChar[MaxSub][MaxRows])(char*);
	void(*FunctionCall[MaxSub][MaxRows])(Params);
	Params PassParam[MaxSub][MaxRows];
	void(*FunctionCallVoid[MaxSub][MaxRows])();

	bool *BoolToggle[MaxSub][MaxRows];
	void(*BoolCall[MaxSub][MaxRows])();

	int* StrArrayScroll[MaxSub][MaxRows];
	int StrArrayMax[MaxSub][MaxRows];
	const char* StrArray[MaxSub][MaxRows][20];

	float* FloatPointer[MaxSub][MaxRows];
	float FloatAmmount[MaxSub][MaxRows];
	float FloatMin[MaxSub][MaxRows];
	float FloatMax[MaxSub][MaxRows];

	int* IntPointer[MaxSub][MaxRows];
	int IntAmmount[MaxSub][MaxRows];
	int IntMin[MaxSub][MaxRows];
	int IntMax[MaxSub][MaxRows];

	char* Text[MaxSub][MaxRows];
	int MaxLen[MaxSub][MaxRows];
	char Clipboard[0x200];

	RGBA* RGBPointer[MaxSub][MaxRows];
	char RGBReturnSub;

	struct {
		char Main;
		char Aimbot;
		char Visual;
		char Players;
		char Settings;

		char PlayerOptions;
		char DebugSettings;
		char NameMenu;
		char PreSetNames;
		char PresetClan;
		char HVH;
		char RadarSettings;
		char ESPSettings;
		char Visions;
		char ColourSettings;
		char MenuSettings;
		char UserGamertags;
		char GamertagLooperList;
		char RGBEditor;
	} MenuIDs;
};

extern CE_s CE;
extern Menu_s Menu;

void CreateMenu();
void SelectOption(char MenuID, char Scroll, const char* Button);
void ScrollSubs(char MenuID, char Direction);
void ScrollTabs(char Direction);
void DrawMenu();
void DoNonHostWait(int time);
void MenuMonitor();