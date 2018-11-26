#pragma once
#include "setjmp.h"

#define FLAG_CROUCHED 0x04
#define FLAG_PRONE 0x08

struct Material
{
	const char* Name;//0x00
};

enum entityType_t
{
	ET_GENERAL = 0x0,
	ET_PLAYER = 0x1,
	ET_PLAYER_CORPSE = 0x2,
	ET_ITEM = 0x3,
	ET_MISSILE = 0x4,
	ET_INVISIBLE = 0x5,
	ET_SCRIPTMOVER = 0x6,
	ET_SOUND_BLEND = 0x7,
	ET_FX = 0x8,
	ET_LOOP_FX = 0x9,
	ET_PRIMARY_LIGHT = 0xA,
	ET_TURRET = 0xB,
	ET_HELICOPTER = 0xC,
	ET_PLANE = 0xD,
	ET_VEHICLE = 0xE,
	ET_VEHICLE_COLLMAP = 0xF,
	ET_VEHICLE_CORPSE = 0x10,
	ET_VEHICLE_SPAWNER = 0x11,
	ET_AGENT = 0x12,
	ET_AGENT_CORPSE = 0x13,
	ET_EVENTS = 0x14,
};

union DvarValue
{
	bool enabled;
	int integer;
	unsigned int unsignedInt;
	float value;
	float vector[4];
	const char *string;
	char color[4];
};

struct dvar_t
{
	const char *name; //0x00
	unsigned int flags; //0x04
	char type; //0x08
	char _0x0009[0x3];
	DvarValue current; //0x0C
};

struct usercmd_s
{
	int time; //0x00
	int buttons; //0x04
	int viewAngles[3]; //0x08
	int weapon; //0x14
	int offHand; //0x18
	char fDir; //0x1C
	char rDir; //0x1D
	char _0x001E[0x1E];
}; //0x3C

enum TraceHitType
{
	TRACE_HITTYPE_NONE = 0x0,
	TRACE_HITTYPE_ENTITY = 0x1,
	TRACE_HITTYPE_DYNENT_MODEL = 0x2,
	TRACE_HITTYPE_DYNENT_BRUSH = 0x3,
	TRACE_HITTYPE_GLASS = 0x4,
};

struct trace_t
{
	float fraction; //0x00
	Vector3 normal; //0x04
	int surfaceFlags; //0x10
	int contents; //0x14
	TraceHitType hitType; //0x18
	short hitId; //0x1C
	short modelIndex; //0x1E
	short partName; //0x20
	short partGroup; //0x22
	bool allsolid; //0x24
	bool startsolid; //0x25
	bool walkable; //0x26
	bool getPenetration; //0x27
	bool removePitchAndRollRotations; //0x28
}; //0x2C

struct BulletTraceResults
{
	trace_t trace; //0x00
	int *hitEnt; //0x2C
	float hitPos[3]; //0x30
	bool ignoreHitEnt; //0x3C
	int depthSurfaceType; //0x40
	int hitClientNum; //0x44
}; //0x48

enum meansOfDeath_t
{
	MOD_UNKNOWN = 0x0,
	MOD_PISTOL_BULLET = 0x1,
	MOD_RIFLE_BULLET = 0x2,
	MOD_EXPLOSIVE_BULLET = 0x3,
	MOD_GRENADE = 0x4,
	MOD_GRENADE_SPLASH = 0x5,
	MOD_PROJECTILE = 0x6,
	MOD_PROJECTILE_SPLASH = 0x7,
	MOD_MELEE = 0x8,
	MOD_HEAD_SHOT = 0x9,
	MOD_MELEE_DOG = 0xA,
	MOD_MELEE_ALIEN = 0xB,
	MOD_CRUSH = 0xC,
	MOD_FALLING = 0xD,
	MOD_SUICIDE = 0xE,
	MOD_TRIGGER_HURT = 0xF,
	MOD_EXPLOSIVE = 0x10,
	MOD_IMPACT = 0x11,
	MOD_NUM = 0x12,
};

struct BulletFireParams
{
	int weaponEntIndex; //0x00
	int ignoreEntIndex; //0x04
	float damageMultiplier; //0x08
	meansOfDeath_t methodOfDeath; //0x0C
	bool noRicochet; //0x10
	Vector3 origStart; //0x14
	Vector3 start; //0x20
	Vector3 end; //0x2C
	Vector3 dir; //0x38
}; //0x44

enum StanceState
{
	CL_STANCE_STAND = 0x0,
	CL_STANCE_CROUCH = 0x1,
	CL_STANCE_PRONE = 0x2,
	CL_STANCE_DIVE_TO_PRONE = 0x3,
};

struct clientactive_t
{
	char _0x0000[0x9C];
	Vector3 Origin; //0x9C 
	char _0x00A8[0x3C];
	Vector3 baseAngle; //0xE4
	char _0x00F0[0x3B0];
	float flZoomProgress; //0x4A0
	char _0x4A4[0x2D68];
	StanceState stance; //0x320C
	char _0x3210[0xC0];
	Vector3 viewAngle; //0x32D0
	usercmd_s UserCommands[128]; //0x32DC
	int CommandNumber; //0x50E0

	usercmd_s* GetUserCommand(int CommandNumber)
	{
		return &UserCommands[CommandNumber & 0x7F];
	}
};

enum trType_t
{
	TR_STATIONARY = 0x0,
	TR_INTERPOLATE = 0x1,
	TR_LINEAR = 0x2,
	TR_LINEAR_STOP = 0x3,
	TR_SINE = 0x4,
	TR_GRAVITY = 0x5,
	TR_LOW_GRAVITY = 0x6,
	TR_ACCELERATE = 0x7,
	TR_DECELERATE = 0x8,
	TR_PHYSICS = 0x9,
	TR_ANIMATED_MOVER = 0xA,
	TR_FIRST_RAGDOLL = 0xB,
	TR_RAGDOLL = 0xB,
	TR_RAGDOLL_GRAVITY = 0xC,
	TR_RAGDOLL_INTERPOLATE = 0xD,
	TR_LAST_RAGDOLL = 0xD,
	NUM_TRTYPES = 0xE,
};

struct trajectory_t
{
	trType_t trType; //0x0000
	int trTime; //0x0004
	int trDuration; //0x0008
	Vector3 trBase; //0x000C
	Vector3 trDelta; //0x0018
}; //0x24

#pragma region LerpEntityState
struct LerpEntityStateTurret
{
	float gunAngles[3];
	int lastBarrelRotChangeTime;
	short lastBarrelRotChangeRate;
	short lastHeatChangeLevel;
	int lastHeatChangeTime;
	bool isBarrelRotating;
	bool isOverheat;
	bool isHeatingUp;
};

struct LerpEntityStateLoopFx
{
	float cullDist;
	int period;
};

struct LerpEntityStatePrimaryLight
{
	char colorAndExp[4];
	float intensity;
	float radius;
	float cosHalfFovOuter;
	float cosHalfFovInner;
};

struct LerpEntityStatePlayer
{
	int movementDir;
	float lerpLean;
	float torsoPitch;
	float waistPitch;
	int offhandWeapon;
	int stowedWeapon;
	int lastSpawnTime;
};

struct LerpEntityStateAgentCorpse
{
	int modelIndex;
	int attachModels[3];
	int attachTags[3];
};

struct LerpEntityStateVehicle
{
	unsigned int indices;
	unsigned int flags;
	float bodyPitch;
	float bodyRoll;
	float steerYaw;
	float gunPitch;
	float gunYaw;
};

struct LerpEntityStatePlane
{
	int ownerNum;
	int enemyIcon;
	int friendIcon;
};

struct LerpEntityStateMissile
{
	int launchTime;
	int flightMode;
	int flags;
};

struct LerpEntityStateSoundBlend
{
	float lerp;
};

struct LerpEntityStateBulletHit
{
	float start[3];
};

struct LerpEntityStateEarthquake
{
	float scale;
	float radius;
	int duration;
};

struct LerpEntityStateCustomExplode
{
	int startTime;
};

struct LerpEntityStateExplosion
{
	float innerRadius;
	float outerRadius;
	float magnitude;
};

struct LerpEntityStateExplosionJolt
{
	float innerRadius;
	float outerRadius;
	float impulse[3];
};

struct LerpEntityStatePhysicsJitter
{
	float innerRadius;
	float outerRadius;
	float minDisplacement;
	float maxDisplacement;
};

struct LerpEntityStatePhysicsImpactSnd
{
	int sndClass;
	float impactMomentum;
};

struct LerpEntityStateRadiusDamage
{
	float range;
	int damageMin;
	int damageMax;
};

struct LerpEntityStateScriptMover
{
	int entToTakeMarksFrom;
	int xModelIndex;
	int animIndex;
	int animTime;
};

struct LerpEntityStateAnonymous
{
	int data[7];
};

#pragma endregion

struct LerpEntityState
{
	int eFlags; //0x00
	trajectory_t pos; //0x04
	trajectory_t apos; //0x28
	union
	{
		LerpEntityStateTurret turret;
		LerpEntityStateLoopFx loopFx;
		LerpEntityStatePrimaryLight primaryLight;
		LerpEntityStatePlayer player;
		LerpEntityStateAgentCorpse agentCorpse;
		LerpEntityStateVehicle vehicle;
		LerpEntityStatePlane plane;
		LerpEntityStateMissile missile;
		LerpEntityStateSoundBlend soundBlend;
		LerpEntityStateBulletHit bulletHit;
		LerpEntityStateEarthquake earthquake;
		LerpEntityStateCustomExplode customExplode;
		LerpEntityStateExplosion explosion;
		LerpEntityStateExplosionJolt explosionJolt;
		LerpEntityStatePhysicsJitter physicsJitter;
		LerpEntityStatePhysicsImpactSnd physicsImpactSnd;
		LerpEntityStateRadiusDamage radiusDamage;
		LerpEntityStateScriptMover scriptMover;
		LerpEntityStateAnonymous anonymous;
	}u; //0x4C - size(0x1C)
}; //0x68

struct cpose_t
{
	unsigned short lightingHandle; //0x00
	char eType; //0x02
	char cullIn; //0x03
	char isRagdoll; //0x04
	int ragdollHandle; //0x08
	int killcamRagdollHandle; //0x0C
	int physObjId; //0x10
	Vector3 Origin; //0x14 
	Vector3 Angles; //0x20
	char _0x002C[0x30];
}; //0x5C

#pragma region StaticEntityState
struct StaticEntityStatePlayer
{
	int playerFlags;
};

struct StaticEntityStateVehicle
{
	int playerIndex;
};

struct StaticEntityStateTurret
{
	int isBeingCarried;
};

struct StaticEntityStateMover
{
	unsigned int scriptableIndex;
};

struct StaticEntityStateAgentCorpse
{
	int stowedWeapon;
};

struct StaticEntityStateFX
{
	int killEffectDef;
};

struct StaticEntityStateAnonymous
{
	int data[1];
};
#pragma endregion

struct animInfo_t
{
	int animTime; //0x00
	unsigned int animData; //0x04
};

struct clientLinkInfo_t
{
	int _bf0;
};

union HudData
{
	unsigned int data;
};

struct entityState_s
{
	int number; //0x00
	int Type; //0x04
	LerpEntityState lerp; //0x08
	union
	{
		StaticEntityStatePlayer player;
		StaticEntityStateVehicle vehicle;
		StaticEntityStateTurret turret;
		StaticEntityStateMover mover;
		StaticEntityStateAgentCorpse agentCorpse;
		StaticEntityStateFX fx;
		StaticEntityStateAnonymous anonymous;
	}; //0x70
	int time2; //0x74
	int otherEntityNum; //0x78
	int attackerEntityNum; //0x7C
	int groundEntityNum; //0x80
	int loopSound; //0x84
	int surfType; //0x88
	union
	{
		int brushModel;
		int triggerModel;
		int xmodel;
		int primaryLight;
	}; //0x8C
	int clientNum; //0x90
	int iHeadIcon; //0x94
	HudData hudData; //0x98
	int solid; //0x9C
	char _0x00A0[0x24];
	int Weapon; //0xC4
	char _0x00C8[0x4];
	int inAltWeaponMode; //0xCC
	animInfo_t animInfo; //0xD0
	union
	{
		int eventParm2;
		unsigned int hintString;
		int fxId;
	}; //0xD8
	union
	{
		int hintType;
		int vehicleXModel;
		float animRate;
	}; //0xE0
	clientLinkInfo_t clientLinkInfo; //0xE4
	unsigned int partBits[6]; //0xE8
	int clientMask; //0xFC
	unsigned int pad; //0x100
}; //0x104

#pragma pack(push, 2)
struct centity_s
{
	cpose_t pose;//0x0
	LerpEntityState prevState; //0x5C
	entityState_s nextState; //0xC4
	char _0x01C8[0x24];
}; //0x1EC


#pragma pack(pop)

struct refdef_s
{
	int x; //0x00
	int y; //0x04
	int width; //0x08
	int height; //0x0C 
	Vector2 Fov; //0x10 
	Vector3 ViewOrigin; //0x18 
	Vector3 ViewAxis[3]; //0x24
	//6C blur ammount 70 enable
}; //0x48

struct characterInfo_t
{
	int entityNum; //0x00
	int infoValid; //0x04
	int nextValid; //0x08
	int Team; //0x0C
	int oldteam; //0x10
	unsigned int perks[2]; //0x14
	char model[64]; //0x1C
	char attachModelNames[6][64]; //0x5C
	char attachTagNames[6][64]; //0x1DC
	char _0x035C[0x240];
}; //0x59C

struct ClientCustomizationInfo
{
	unsigned int modelIndex[3];
};

struct clientInfo_t
{
	int clientNum; //0x00
	char Name[32]; //0x04
	int rank_mp; //0x24
	int prestige_mp; //0x28
	int rank_alien; //0x2C
	int prestige_alien; //0x30
	char clanAbbrev[8]; //0x34
	int location; //0x3C
	int health; //0x40
	unsigned int playerCardPatch; //0x44
	unsigned int playerCardBackground; //0x48
	char _0x004C[0x20];
	Material *rankIconHandle; //0x6C
	const char *rankDisplayLevel; //0x70
}; //0x74

struct bgs_t
{
	char _0x0000[0x3EFE0];
	int time; //0x3EFE0
	int deltaTime; //0x3EFE4
	int latestSnapshotTime; //0x3EFE8
	int frametime; //0x3EFEC
	int anim_user; //0x3EFF0
	char _0x3EFF4[0x24];
	characterInfo_t characterinfo[36]; //0x3F018
	clientInfo_t clientinfo[12]; //0x4BA08
}; //0x4BF78

struct score_t
{
	int client; //0x00
	int status; //0x04
	int score; //0x08
	int ping; //0x0C
	int deaths; //0x10
	int team; //0x14
	int kills; //0x18
	int rank_mp; //0x1C
	int prestige_mp; //0x20
	const char *rankDisplayLevel; //0x24
	int assists; //0x28
	int extrascore0; //0x2C
	int extrascore1; //0x30
	int adrenaline; //0x34
	Material *hStatusIcon; //0x38
	Material *hRankIcon; //0x3C
}; //0x40

struct cgs_t
{
	int viewX; //0x00
	int viewY; //0x04
	int viewWidth; //0x08
	int viewHeight; //0x0C
	float viewAspect; //0x10
	float viewMult; //0x14
	int serverCommandSequence; //0x18
	int processedSnapshotNum; //0x1C
	int localServer; //0x20
	char gametype[32]; //0x24
	char szHostName[256]; //0x44 
	bool hardcore; //0x144
	int maxclients; //0x148
	int privateClients; //0x14C
	char MapName[64]; //0x0150 
	int gameEndTime; //0x190
};

enum objectiveState_t
{
	OBJST_EMPTY = 0x0,
	OBJST_ACTIVE = 0x1,
	OBJST_INVISIBLE = 0x2,
	OBJST_DONE = 0x3,
	OBJST_CURRENT = 0x4,
	OBJST_FAILED = 0x5,
	OBJST_NUMSTATES = 0x6,
};

struct objective_t
{
	objectiveState_t state;//0x00
	Vector3 Origin;//0x4
	int entNum;//0x10
	int shouldRotate; //0x14
	char _0x0018[0xC];
	int icon; //0x24
}; //0x28

struct playerState_s
{
	int Command; //0x0
	int pm_type; //0x4
	int pm_time; //0x8
	int pm_flags; //0xC
	int otherFlags; //0x10
	int linkFlags; //0x14
	int bobCycle; //0x18
	Vector3 origin; //0x1C
	Vector3 velocity; //0x28
	int grenadeTimeLeft; //0x34
	int throwbackGrenadeOwner; //0x38
	int throwbackGrenadeTimeLeft; //0x3C
	int throwbackWeapon; //0x40
	int movingPlatformEntity; //0x44
	int remoteEyesEnt; //0x48
	int remoteEyesTagname; //0x4C
	int remoteControlEnt; //0x50
	int remoteTurretEnt; //0x54
	int gravity; //0x58
	int speed; //0x5C
	float delta_angles[3]; //0x60
	int groundEntityNum; //0x6C
	float vLadderVec[3]; //0x70
	int jumpTime; //0x7C
	float jumpOriginZ; //0x80
	char _0x0084[0xF8];
	int clientNum; //0x17C
	int viewmodelIndex; //0x180
	float viewangles[3]; //0x184
	int viewHeightTarget; //0x190
	float viewHeightCurrent; //0x194
	int viewHeightLerpTime; //0x194
	int viewHeightLerpTarget; //0x19C
	int viewHeightLerpDown; //0x1A0
	float viewAngleClampBase[2]; //0x1A4
	float viewAngleClampRange[2]; //0x1AC
	int damageEvent; //0x1B4
	int damageYaw; //0x1B8
	int damagePitch; //0x1BC
	int damageCount; //0x1C0
	int damageFlags; //0x1C4
	//int stats[4]; //0x1C8
	int Health; //0x1C8
	char _0x1CC[0x4];
	int MaxHealth; //0x1D0
	char _0x1D4[0x248];
	int Weapon; //0x41C
	char _0x420[0x8];
	float SpreadMultiplier; //0x428
	char _0x42C[0x69C];
	objective_t objective[32];//0xAC8
	char _0x0FC8[0x1FB8];
}; //0x2F80

struct snapshot_s
{
	playerState_s ps; //0x00
	char _0x2F80[0x8E0];
	int snapFlags; //0x3860
	int ping; //0x3864
	int serverTime; //0x3868
	int numEntities; //0x386C
	int numClients; //0x3870
	entityState_s entities[768]; //0x3874
};

struct cg_s
{
	playerState_s ps; //0x00 predictedPlayerState
	centity_s *predictedPlayerEntity; //0x2F80
	char _0x2F84[0x30];
	int ClientNumber;//0x2FB4
	int localClientNum; //0x2FB8
	char _0x2FBC[0x10];
	int renderScreen; //0x2FCC
	int latestSnapshotNum; //0x2FD0
	int latestSnapshotTime; //0x2FD4
	snapshot_s* snap; //0x2FD8
	snapshot_s* nextSnap; //0x2FDC
	char _0x2FE0[0x6A520];
	float frameInterpolation; //0x6D500
	int frametime; //0x6D504
	int time; //0x6D508
	int oldTime; //0x6D50C
	int physicsTime; //0x6D510
	int groundEntityTime; //0x6D514
	unsigned int lastGroundSurfaceType; //0x6D518
	int mapRestart; //0x6D51C
	int spectatingThirdPerson; //0x6D520
	int renderingThirdPerson; //0x6D524
	float landChange; //0x6D528
	int landTime; //0x6D52C
	int lastMantleflags; //0x6D530
	int lastProneCrawlInputTime; //0x6D534
	float heightToCeiling;//0x6D538
	refdef_s refdef; //0x6D53C
	char _0x6D584[0x4B34];
	Vector3 refdefViewAngles;//0x720B8
	char _0x720C4[0x817C];
	float compassNorthYaw; //0x7A240
	float compassNorth[2]; //0x7A244
	Material* compassMapMaterial;//0x7A24C
	float compassMapUpperLeft[2]; //0x7A250
	float compassMapWorldSize[2]; //0x7A258
	char _0x7A260[0x3F198];
	int bgs_time; //0xB93F8
	int bgs_deltaTime; //0xB93FC
	int bgs_latestSnapshotTime; //0xB9400
	int bgs_frametime; //0xB9404
	int bgs_anim_user; //0xB9408
	char _0xB940C[0x24];
	characterInfo_t characterinfo[36]; //0xB9430
	clientInfo_t clientinfo[12]; //0xC5E20
	char _0xC6390[0x1450];
	float SpreadMultiplier; //0xC77E0
}; //0xC8880

struct UiContext
{
	char _0x0000[0x20];
	int screenWidth;//0x20
	int screenHeight;//0x24
	char _0x0028[0x4];
	float FPS;//0x2C
};

enum weaponIconRatioType_t
{
	WEAPON_ICON_RATIO_1TO1 = 0x0,
	WEAPON_ICON_RATIO_2TO1 = 0x1,
	WEAPON_ICON_RATIO_4TO1 = 0x2,
	WEAPON_ICON_RATIO_COUNT = 0x3,
};

enum weapFireType_t
{
	WEAPON_FIRETYPE_FULLAUTO = 0x0,
	WEAPON_FIRETYPE_SINGLESHOT = 0x1,
	WEAPON_FIRETYPE_BURSTFIRE2 = 0x2,
	WEAPON_FIRETYPE_BURSTFIRE3 = 0x3,
	WEAPON_FIRETYPE_BURSTFIRE4 = 0x4,
	WEAPON_FIRETYPE_DOUBLEBARREL = 0x5,
	WEAPON_FIRETYPECOUNT = 0x6,
	WEAPON_FIRETYPE_BURSTFIRE_FIRST = 0x2,
	WEAPON_FIRETYPE_BURSTFIRE_LAST = 0x4,
};

struct WeaponDef
{
	char _0x0000[0x5C];
	weapFireType_t fireType; //0x5C
	char _0x0060[0x4C8];
	weaponIconRatioType_t killIconRatio; //0x528
};

struct WeaponVariantDef
{
	char* szInternalName;//0x00
	WeaponDef* weaponDef; //0x04
	char* szModeName;//0x08
	char _0x000C[0x78];
	Material* killIcon; //0x84
	char _0x0088[0x764];
	float fAdsZoomFov;//0x7EC
};

struct Network_s
{
	char Active; //0x00
	char _0x0002[0x52];
	char gamertag[0x20]; //0x54
	char clantag[0x8]; //0x74
	char _0x007C[0xBC];
	unsigned char InternalIP[0x4]; //0x138
	unsigned char IP[0x4]; //0x13C
	unsigned short Port; //0x140
	unsigned char Mac[0x6]; //0x142
	char _0x0148[0x8];
	unsigned char MachineID[0x8]; //0x150
	char _0x0158[0x30];
	unsigned char Xuid[0x8]; //0x188
	char _0x0190[0x30];
}; //0x1C0

struct ClientNetwork_s
{
	char _0x0000[0x8];
	Network_s Net[12];
};

struct ClientOptions_s
{
	char Gamertag[32];
	unsigned char Xuid[8];
	bool Prioritized;
	bool Ignored;
	bool AimAtRoot;
	bool InvertTeam;
	bool LoadedFromCFG;

	void reset()
	{
		user_memset(Xuid, 0, 8);
		Prioritized = false;
		Ignored = false;
		AimAtRoot = false;
		LoadedFromCFG = false;
	}
};

struct DObj_s
{

};

struct va_info_t
{
	char va_string[4][0x400];
	int index;
};

#define CMD_MAX_NESTING	8
struct CmdArgs
{
	int nesting; //0x0
	int localClientNum[CMD_MAX_NESTING]; //0x4
	int controllerIndex[CMD_MAX_NESTING]; //0x24
	void *itemDef[CMD_MAX_NESTING]; //0x44
	int argshift[CMD_MAX_NESTING]; //0x64
	int argc[CMD_MAX_NESTING]; //0x84
	const char **argv[CMD_MAX_NESTING]; //0xA4
	char textPool[0x2000]; //0xC4
	const char* argvPool[0x200]; //0x20C4
	int usedTextPool[CMD_MAX_NESTING]; //0x28C4
	int totalUsedArgvPool; //0x28E4
	int totalUsedTextPool;//0x28E8
};

struct TraceCheckCount
{
	unsigned short global;
	unsigned short *partitions;
	unsigned short *brushes;
};

typedef char cbrush_t[0xB0];
typedef char cmodel_t[0x4C];

struct PhysGeomList;

struct TraceThreadInfo
{
	TraceCheckCount checkcount;
	cbrush_t * box_brush;
	cmodel_t * box_model;
	PhysGeomList ** geoms;
};

struct tls_t
{
	int zero;
	va_info_t* va;
	jmp_buf* env;
	TraceThreadInfo* data;
	CmdArgs* cmd;
};

struct rectDef_s
{
	float x; //0x0
	float y; //0x4
	float w; //0x8
	float h; //0xC
	int horzAlign;
	int vertAlign;
};

enum Bit
{
	AUTO_SHOOT = 1,
};

enum clientMigState_t
{
	CMSTATE_INACTIVE = 0x0,
	CMSTATE_OLDHOSTLEAVING = 0x1,
	CMSTATE_LIMBO = 0x2,
	CMSTATE_NEWHOSTCONNECT = 0x3,
	CMSTATE_COUNT = 0x4,
};

enum connstate_t
{
	CA_DISCONNECTED = 0x0,
	CA_CINEMATIC = 0x1,
	CA_LOGO = 0x2,
	CA_CONNECTING = 0x3,
	CA_CHALLENGING = 0x4,
	CA_CONNECTED = 0x5,
	CA_SENDINGSTATS = 0x6,
	CA_SYNCHRONIZING_DATA = 0x7,
	CA_LOADING = 0x8,
	CA_PRIMED = 0xA,
	CA_ACTIVE = 0xB,
};

struct clientUIActive_t
{
	char active; //0x00
	char isRunning; //0x01
	char cgameInitialized; //0x02
	char cgameInitCalled; //0x03
	clientMigState_t migrationState; //0x04
	char _0x0008[0x64];
	int OpenMenu; //0x6C is 0x40 when the pause menu is open
	connstate_t connectionState; //0x70
};

struct CompassActor
{
	int lastUpdate; //0x00
	float lastPos[3]; //0x04
	float lastEnemyPos[3]; //0x10
	float lastEnemyYaw; //0x1C
	float lastVel; //0x20
	float lastYaw; //0x24
	int pingTime; //0x28
	int beginFadeTime; //0x2C
	int beginRadarFadeTime; //0x30
	int beginVoiceFadeTime; //0x34
	int flags; //0x38
	unsigned int perks[2]; //0c3C
	bool hadPerkRedBlip; //0x44
	bool wasSighted; //0x45
	int ownerNum; //0x48
}; //0x4C

extern clientUIActive_t* clientUIActive;
extern cgs_t* cgs;
extern cg_s* cg;
extern centity_s* Entity;
extern clientactive_t* ClientActive_t;
extern ClientNetwork_s* ClientNetwork;
extern ClientOptions_s ClientOptions[12];