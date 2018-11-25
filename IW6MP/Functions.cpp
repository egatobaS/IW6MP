#include "stdafx.h"
#include "Functions.h"
#include "Lib.h"
#include "Menu.h"
#include "Addresses.h"
#include "AutoWall.h"
#include "R_AddCmdDrawStretchPic.h"
#include "CL_WritePacket.h"

struct KeyBoard_s
{
	int ThreadCallBackPtr;
	int KeyboardMax;
	LPCWSTR KeyboardTitle;
	LPCWSTR KeyboardDesc;
	LPCWSTR KeyboardDefaultValue;
};

KeyBoard_s KeyBoard;

char VisionSet[100] = { 0 };

#define VectorSubtract(a, b, c) ((c)[0] = (a)[0] - (b)[0], (c)[1] = (a)[1] - (b)[1], (c)[2] = (a)[2] - (b)[2])

bool bCreateThread = 0; int ThreadCallBackPtr = 0, KeyboardMax = 0; LPCWSTR KeyboardTitle = 0, KeyboardDesc = 0;

void CG_GameMessage(int localclient, const char* fmt, ...)
{
	if (Dvar_GetBool("cl_ingame") && GetStructs())
	{
		char* conBuffer = (char*)malloc(2000);
		va_list ap;
		va_start(ap, fmt);

		vsprintf(conBuffer, fmt, ap);

		va_end(ap);
		void(*CG_GameMessage)(int localclient, const char* msg) = (void(*)(int, const char*))addr->CG_GameMessage;
		CG_GameMessage(localclient, conBuffer);
		free(conBuffer);
	}
}

void CG_BoldGameMessage(int localclient, const char* fmt, ...)
{
	if (Dvar_GetBool("cl_ingame") && GetStructs())
	{
		char* conBuffer = (char*)malloc(2000);
		va_list ap;
		va_start(ap, fmt);
		vsprintf(conBuffer, fmt, ap);
		va_end(ap);

		void(*CG_BoldGameMessage)(int localclient, const char* msg) = (void(*)(int, const char*))addr->CG_BoldGameMessage;
		CG_BoldGameMessage(localclient, conBuffer);
		free(conBuffer);
	}
}

void Cbuf_AddText(int localclient, const char* fmt, ...)
{
	char* conBuffer = (char*)malloc(2000);
	va_list ap;
	va_start(ap, fmt);

	vsprintf(conBuffer, fmt, ap);

	va_end(ap);
	void(*Cbuf_AddText)(int LocalClient, char* cmd) = (void(*)(int, char*))addr->Cbuf_AddText;
	Cbuf_AddText(localclient, conBuffer);
	free(conBuffer);
}

void CL_AddReliableCommand(int localclient, const char* fmt, ...)
{

	if (Dvar_GetBool("cl_ingame") && GetStructs())
	{
		char conBuffer[2000];
		va_list ap;
		va_start(ap, fmt);

		vsprintf(conBuffer, fmt, ap);

		va_end(ap);
		void(*CL_AddReliableCommand)(int LocalClient, char* cmd) = (void(*)(int, char*))addr->CL_AddReliableCommand;
		CL_AddReliableCommand(localclient, conBuffer);
	}
}

void ConsoleCMD(char* cmd)
{
	Cbuf_AddText(0, cmd);
	CG_GameMessage(0, "Command %s Sent!", cmd);
}

void CG_SetVision(int unk, int unk1, int unk2, char* vision, int duration)
{
	if (Dvar_GetBool("cl_ingame"))
	{
		user_strcpy(VisionSet, vision);
		void(*CG_VisionSetStartLerp_To)(int r3, int r4, int r5, char* vision, int duration) = (void(*)(int, int, int, char*, int))addr->CG_VisionSetStartLerp_To;
		CG_VisionSetStartLerp_To((*(int*)(addr->cg_s) + 0xC6448), unk1, unk2, vision, duration);
	}
}

void SetVision(char* Vision)
{
	CG_SetVision(0, 0, 3, Vision, 1337);
	CG_GameMessage(0, "Vision %s Set!", Vision);
}

void ResetSetVision()
{
	CG_SetVision(0, 0, 3, (char*)Dvar_GetString("ui_mapname"), 1337);
	user_strcpy(VisionSet, "ResetSetVision");
	CG_GameMessage(0, "Default Vision Set!");
}

void SetName(char* name)
{
	if (cuser_strcmp(CE.OriginalName, ""))
		user_strcpy(CE.OriginalName, (char*)(addr->NameAddress));

	if (Dvar_GetBool("cl_ingame")) {
		CL_AddReliableCommand(0, "userinfo \"\\clanAbbrev\\%s\\name\\%s\";", (char*)addr->ClantagAddress, name);
		CG_GameMessage(0, "Name %s Set!", name);
	}

	user_strcpy((char*)addr->NameAddress, name);
	user_strcpy((char*)CE.SetGamertag, name);
}

void Setname(char* fmt, ...)
{
	char conBuffer[2000];
	va_list ap;
	va_start(ap, fmt);

	vsprintf(conBuffer, fmt, ap);

	va_end(ap);

	SetName(conBuffer);
}

void SetNameColour(char* colour, char* name)
{
	if (cuser_strcmp(CE.OriginalName, ""))
		user_strcpy(CE.OriginalName, (char*)(addr->NameAddress));

	if (Dvar_GetBool("cl_ingame")) {
		CL_AddReliableCommand(0, "userinfo \"\\clanAbbrev\\%s\\name\\%s\";", colour, name);
	}

	user_strcpy((char*)addr->NameAddress, va("%s%s", colour, name));
	user_strcpy((char*)CE.SetGamertag, name);
}

void SetClantag(char* tag)
{
	if (Dvar_GetBool("cl_ingame")) {
		CL_AddReliableCommand(0, "userinfo \"\\clanAbbrev\\%s\\name\\%s\";", tag, (char*)addr->NameAddress);
		CG_GameMessage(0, "Clantag %s Set!", tag);
	}

	user_strcpy((char*)addr->ClantagAddress, tag);
}

void DoKeyboardWork(LPVOID r3)
{
	KeyBoard_s* KeyBoard = (KeyBoard_s*)r3;
	void(*CallBack) (...) = NULL;
	(void*&)(CallBack) = (void*&)KeyBoard->ThreadCallBackPtr;
	int err;
	const char* ReturnValue = ReturnXShowKeyboardUI(KeyBoard->KeyboardDefaultValue, KeyBoard->KeyboardTitle, KeyBoard->KeyboardDesc, KeyBoard->KeyboardMax, err);
	if (err == 0)
	{
		if (ReturnValue)
			CallBack(ReturnValue);
		else
			CG_GameMessage(0, "Keyboard blank");
	}
	else
		CG_GameMessage(0, "Keyboard canceled");

	DWORD Thr_Exit = 0;
	ExitThread(Thr_Exit);
}

void ThreadKeyBoardAction(int CallBackPtr, LPCWSTR DefaultValue, LPCWSTR Title, LPCWSTR Desc, int MaxVal)
{
	KeyBoard.ThreadCallBackPtr = CallBackPtr;
	KeyBoard.KeyboardDefaultValue = DefaultValue;
	KeyBoard.KeyboardTitle = Title;
	KeyBoard.KeyboardDesc = Desc;
	KeyBoard.KeyboardMax = MaxVal;

	DWORD hThreadID;
	HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DoKeyboardWork, (LPVOID)&KeyBoard, 3, &hThreadID);
	XSetThreadProcessor(hThread, 1);
	ResumeThread(hThread);
	CloseHandle(hThread);
}

bool IsEnemy(int client)
{
	if (client == cg->ClientNumber)
		return false;

	return ((CE.InverseTeams || ClientOptions[client].InvertTeam) ? (cg->characterinfo[client].Team == cg->characterinfo[cg->ClientNumber].Team || cg->characterinfo[client].Team == 0) : (cg->characterinfo[client].Team != cg->characterinfo[cg->ClientNumber].Team || cg->characterinfo[client].Team == 0));
}

bool IsFriendly(int client)
{
	if (client == cg->ClientNumber)
		return true;

	return ((CE.InverseTeams || ClientOptions[client].InvertTeam) ? (cg->characterinfo[client].Team != cg->characterinfo[cg->ClientNumber].Team || cg->characterinfo[client].Team != 0) : (cg->characterinfo[client].Team == cg->characterinfo[cg->ClientNumber].Team || cg->characterinfo[client].Team != 0));
}

bool GetStructs()
{
	if (bDisConnected)
		return false;

#if defined(DEVKIT)

	clientUIActive = (clientUIActive_t*)addr->clientUIActive;
	cgs = *(cgs_t**)addr->cgs_t;
	cg = *(cg_s**)addr->cg_s;
	Entity = *(centity_s**)addr->centity_s;
	ClientActive_t = *(clientactive_t**)addr->clientactive_t;
	ClientNetwork = (ClientNetwork_s*)addr->ClientNetwork_s;

	if (!*(int*)addr->cgs_t || !*(int*)addr->cg_s || !*(int*)addr->centity_s || !*(int*)addr->clientactive_t) return false;
	return true;

#else
	return  getStructures((int**)&cgs, (int**)&cg, (int**)&Entity, (int**)&ClientActive_t, (int**)&ClientNetwork, (int**)&clientUIActive);
#endif	

}

const char* UI_GetMapName(const char* map)
{
	const char*(*UI_GetMapName)(const char* mapname) = (const char*(*)(const char*))addr->UI_GetMapName;
	return UI_GetMapName(map);
}

const char* UI_GetGameTypeName(const char* gametype)
{
	const char*(*UI_GetGameTypeName)(const char* gametype) = (const char*(*)(const char*))addr->UI_GetGameTypeName;
	return UI_GetGameTypeName(gametype);
}

WeaponDef* BG_GetWeaponDef(int Weapon)
{
	return ((WeaponDef*(*)(...))addr->WeaponDef)(Weapon, 0);
}

WeaponVariantDef* BG_GetWeaponVariantDef(int Weapon)
{
	return ((WeaponVariantDef*(*)(...))addr->WeaponVariantDef)(Weapon, 0);
}

const char *GetWeaponName(int Weapon)
{
	WeaponVariantDef* WeaponVariantDef = BG_GetWeaponVariantDef(Weapon);
	if (WeaponVariantDef)
	{
		char *szRawName = WeaponVariantDef->szModeName;
		if (szRawName) {
			const char *szName = SEH_StringEd_GetString(szRawName);

			if (szName != NULL)
				return szName;
		}
	}

	return "none";
}

Material* GetWeaponShader(int Weapon)
{
	WeaponVariantDef* WeaponVariantDef = BG_GetWeaponVariantDef(Weapon);
	if (WeaponVariantDef)
		return WeaponVariantDef->killIcon;
	return NULL;
}

bool _GetTagPos(int EntNum, const char* Tag, Vector3* Pos) {
	bool(*CG_DObjGetWorldTagPos)(centity_s* Centity, int dobj, unsigned int target, Vector3* pos) = (bool(*)(centity_s*, int, unsigned int, Vector3*))addr->CG_DObjGetWorldTagPos;
	int(*Com_GetClientDObj)(int handle, int localClientNum) = (int(*)(int, int))addr->Com_GetClientDObj;

	int DObj = Com_GetClientDObj(EntNum, 0);
	unsigned int String = SL_GetString(Tag, 0);

	if (DObj && String)
	{
		if (CG_DObjGetWorldTagPos(&Entity[EntNum], DObj, String, Pos))
			return true;
	}

	printf("Failed to find tag %s on player %i\n", Tag, EntNum);
	return false;
}

struct CachedTag_s
{
	Vector3 Origin;
	bool Sucess;
};

CachedTag_s CachedTags[23][18];
const char* Cached_TagList[] = { "j_helmet", "j_head", "j_neck", "j_shoulder_le", "j_shoulder_ri", "j_elbow_le", "j_elbow_ri", "j_wrist_le", "j_wrist_ri", "j_mainroot", "j_spineupper", "j_spinelower", "j_spine4", "j_hip_le", "j_hip_ri", "j_knee_le", "j_knee_ri", "j_ankle_le", "j_ankle_ri", "j_ball_le", "j_ball_ri" };

int GetTagIndexForName(const char* Tag)
{
	for (int i = 0; i < ARRAYSIZE(Cached_TagList); i++)
	{
		if (cuser_strcmp(Tag, Cached_TagList[i]))
			return i;
	}
	return -1;
}

void CacheTagOrigins(int client)
{
	for (int i = 0; i < ARRAYSIZE(Cached_TagList); i++)
		CachedTags[i][client].Sucess = _GetTagPos(client, Cached_TagList[i], &CachedTags[i][client].Origin);
}

bool GetTagPos(int EntNum, const char* Tag, Vector3* Pos, bool pointscale) {
	int index = GetTagIndexForName(Tag);
	if (index == -1)
	{
		printf("Failed to Find tag '%s' as it was not cached\n", Tag);
		return false;
	}

	Pos->x = 0; Pos->y = 0; Pos->z = 0;

	if (CachedTags[index][EntNum].Sucess)
	{
		Vector3 Tagpos = CachedTags[index][EntNum].Origin;

		if (CE.PosAdjustment && pointscale)
			Tagpos.z += (0.10f * CE.PosAdjustmentScale);

		memcpy(Pos, &Tagpos, 0xC);

		return true;
	}
	return false;
}

void FixMovement(usercmd_s* pCmd, float CurAngle, float OldAngle, float fOldForward, float fOldSidemove)
{
	float deltaView = CurAngle - OldAngle, f1, f2;

	if (OldAngle < 0.f)
		f1 = 360.0f + OldAngle;
	else
		f1 = OldAngle;

	if (CurAngle < 0.0f)
		f2 = 360.0f + CurAngle;
	else
		f2 = CurAngle;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;

	pCmd->fDir = ClampChar_r((int)(cosf(DEG2RAD(deltaView)) * fOldForward + cosf(DEG2RAD(deltaView + 90.f)) * fOldSidemove));
	pCmd->rDir = ClampChar_r((int)(sinf(DEG2RAD(deltaView)) * fOldForward + sinf(DEG2RAD(deltaView + 90.f)) * fOldSidemove));
}


void PosPrediction(centity_s* TargetEntity, Vector3& BonePos, int Scale, bool NotAllowZ)
{
	Vector3 EnemyVelocityTemp;
	Vector3 EnemyVelocity;

	EnemyVelocityTemp = (TargetEntity->nextState.lerp.pos.trBase - TargetEntity->prevState.pos.trBase);

	EnemyVelocity.x = EnemyVelocityTemp[0];
	EnemyVelocity.y = EnemyVelocityTemp[1];
	EnemyVelocity.z = EnemyVelocityTemp[2];

	Vector3 CalcVec = EnemyVelocity.Normalize();

	CalcVec.x *= Scale;
	CalcVec.y *= Scale;
	CalcVec.z *= Scale;

	float TempOld = BonePos[2];

	BonePos[0] += CalcVec.x;
	BonePos[1] += CalcVec.y;
	BonePos[2] += CalcVec.z;

	if (NotAllowZ)
		BonePos[2] = TempOld;
}

void BG_EvaluateTrajectory(trajectory_t* tr, int atTime, Vector3* result)
{
	void(*BG_EvaluateTrajectory)(trajectory_t* tr, int atTime, Vector3* result) = (void(*)(trajectory_t*, int, Vector3*))addr->BG_EvaluateTrajectory;
	BG_EvaluateTrajectory(tr, atTime, result);
}

void CG_GetPlayerVelocity(int LocalClientNum, centity_s* cent, Vector3* velocity_out)
{
	Vector3 Current;
	Vector3 Next;
	float g_speed = cg->ps.speed;

	if ((cent->prevState.pos.trType == TR_STATIONARY) || (cent->prevState.pos.trType == TR_INTERPOLATE) || (cent->prevState.pos.trType == TR_RAGDOLL_INTERPOLATE))
		Current = cent->prevState.pos.trBase;
	else if (cent->prevState.pos.trType != TR_PHYSICS)
		BG_EvaluateTrajectory(&cent->prevState.pos, cg->snap->serverTime, &Current);

	if ((cent->nextState.lerp.pos.trType == TR_STATIONARY) || (cent->nextState.lerp.pos.trType == TR_INTERPOLATE) || (cent->nextState.lerp.pos.trType == TR_RAGDOLL_INTERPOLATE))
		Next = cent->nextState.lerp.pos.trBase;
	else if (cent->nextState.lerp.pos.trType != TR_PHYSICS)
		BG_EvaluateTrajectory(&cent->nextState.lerp.pos, cg->nextSnap->serverTime, &Next);

	*velocity_out = (Next - Current);

	if ((cg->nextSnap->serverTime - cg->snap->serverTime) != 0)
	{
		*velocity_out *= (1000.0f / ((cg->nextSnap->serverTime - cg->snap->serverTime) & 0xFF)); //scale

		float f31 = sqrtf((velocity_out->x * velocity_out->x) + (velocity_out->z * velocity_out->z) + (velocity_out->y * velocity_out->y));

		if (((-f31 == -0.0f) ? 0.0f : -f31) < 0.0f)
			f31 = f31;
		else
			f31 = 1.0f;

		*velocity_out *= (1.0f / f31);

		if (f31 > g_speed)
			f31 = g_speed;

		*velocity_out *= f31;
	}
	else
		*velocity_out = 0.0f;
}

int CG_GetVelocity(int localclient, int Client, Vector3* VelocityOut)
{
	int(*CG_GetVelocity)(int localclient, int Client, Vector3* VelocityOut) = (int(*)(int, int, Vector3*))addr->CG_GetVelocity;
	return CG_GetVelocity(localclient, Client, VelocityOut);
}

Vector3 VelocityPrediction(centity_s* TargetEntity, Vector3 BonePos, int Scale) // 0.0 - 0.75
{
	Vector3 Velocity;
	CG_GetVelocity(0, TargetEntity->nextState.clientNum, &Velocity);

	return (BonePos + (Velocity * Scale));
}

bool CG_GetPlayerViewOrigin(int localClientNum, playerState_s *ps, Vector3 *origin)
{
	bool(*CG_GetPlayerViewOrigin)(int localClientNum, playerState_s *ps, Vector3 *origin) = (bool(*)(int, playerState_s *, Vector3 *))addr->CG_GetPlayerViewOrigin;
	return CG_GetPlayerViewOrigin(localClientNum, ps, origin);
}

unsigned short Trace_GetEntityHitId(trace_t* trace)
{
	unsigned short(*Trace_GetEntityHitId)(trace_t* trace) = (unsigned short(*)(trace_t*))addr->Trace_GetEntityHitId;
	return Trace_GetEntityHitId(trace);
}

const char* RiotBoxesNoHead[] = { "j_head", "j_spine4", "j_hip_le", "j_hip_ri", "j_knee_ri", "j_knee_le", "j_ball_le", "j_ankle_le", "j_ball_ri", "j_ankle_ri" };

const char* RiotBoxesLe[] = { "j_ball_le", "j_ankle_le" };
const char* RiotBoxesRi[] = { "j_ball_ri", "j_ankle_ri" };

const char* RiotBoxesRiHipLeg[] = { "j_hip_ri", "j_knee_ri", "j_ankle_ri", "j_ball_ri" };
const char* RiotBoxesLeLeg[] = { "j_knee_le", "j_ankle_le", "j_ball_le" };
const char* RiotBoxesRiLeg[] = { "j_knee_ri", "j_ankle_ri", "j_ball_ri" };
const char* RiotBoxesAnkles[] = { "j_ankle_ri", "j_ankle_le" };

const char* RiotBoxesRiElbowknee[] = { "j_knee_ri", "j_elbow_ri" };
const char* RiotBoxesLeHipLeg[] = { "j_hip_le", "j_knee_le", "j_ankle_le", "j_ball_le" };

//const char* RiotBoxesRiLeg[] = { "j_ball_ri", "j_ankle_ri", "j_knee_ri" };
const char* RiotBoxesHip[] = { "j_hip_ri", "j_hip_le" };
const char* RiotBoxesFeet[] = { "j_ball_ri", "j_ankle_ri", "j_ball_le", "j_ankle_le" };
const char* RiotBoxesLegs[] = { "j_knee_ri", "j_knee_le", "j_ball_ri", "j_ankle_ri", "j_ball_le", "j_ankle_le" };
const char* RiotBoxesHipLegs[] = { "j_hip_ri", "j_hip_le", "j_knee_ri", "j_knee_le", "j_ball_ri", "j_ankle_ri", "j_ball_le", "j_ankle_le" };

float GetDistance2(Vector3 c1, Vector3 c2)
{
	Vector3 Sub = c1 - c2;
	return (sqrt((float)((Sub.x * Sub.x) + (Sub.y * Sub.y) + (Sub.z * Sub.z))));
}

PlayerSide_t SideOfPlayer(int client)
{
	Vector3 LeftFoot = Vector3(0, 0, 0), RightFoot = Vector3(0, 0, 0);

	if (GetTagPos(client, "j_hip_le", &LeftFoot, false) && GetTagPos(client, "j_hip_ri", &RightFoot, false))
	{
		float DistanceLeft = GetDistance2(cg->refdef.ViewOrigin, LeftFoot);
		float DistanceRight = GetDistance2(cg->refdef.ViewOrigin, RightFoot);
		float Diff;

		if (DistanceRight > DistanceLeft)
			Diff = (DistanceRight - DistanceLeft);
		else
			Diff = (DistanceLeft - DistanceRight);

		if (Diff < 2.7)
			return CenterSide;

		if (DistanceLeft < DistanceRight)
			return LeftSide;
		else
			return RightSide;
	}
	return fucked;
}


bool HasShield(int client)
{
	return (cuser_strcmp(GetWeaponName(Entity[client].nextState.Weapon), "Riot Shield") || (strstr(cg->characterinfo[client].attachModelNames[1], "weapon_riot_shield_mp") && !cuser_strcmp(cg->characterinfo[client].attachModelNames[1], "")));
}

bool ShieldEquiped(int client)
{
	return cuser_strcmp(GetWeaponName(Entity[client].nextState.Weapon), "Riot Shield");
}

bool ShieldStowed(int client)
{
	return ((cuser_strcmp(cg->characterinfo[client].attachModelNames[1], "weapon_riot_shield_mp") && !cuser_strcmp(cg->characterinfo[client].attachModelNames[1], "")) && !cuser_strcmp(GetWeaponName(Entity[client].nextState.Weapon), "Riot Shield"));
}

enum RIOT_BONE_SET
{
	BONE_SET_NONE,
	BONE_SET_ALL,
	BONE_SET_NO_HEAD,
	BONE_SET_LEFT,
	BONE_SET_RIGHT,
	BONE_SET_RIGHT_HIP_LEG,
	BONE_SET_LEFT_LEG,
	BONE_SET_RIGHT_LEG,
	BONE_SET_ANKLES,
	BONE_SET_RIGHT_ELBOW_KNEE,
	BONE_SET_LEFT_HIP_LEG,
	BONE_SET_HIP,
	BONE_SET_FEET,
	BONE_SET_LEGS,
	BONE_SET_HIP_LEGS,
	BONE_SET_NECK,
};

RIOT_BONE_SET CurrentRiotBone[18];

void RunRiotChecks(int i)
{
	CurrentRiotBone[i] = BONE_SET_NONE;
	if (HasShield(i))
	{
		float Pitch = Entity[i].nextState.lerp.apos.trBase.x;
		float Yaw = ((GetAimDegreeA(i) / (WallDepthA(Entity[cg->ClientNumber].pose.Origin, Entity[i].pose.Origin))) * 100);
		float Distance = GetDistance(Entity[cg->ClientNumber].pose.Origin, Entity[i].pose.Origin);
		PlayerSide_t PlayerSide = SideOfPlayer(i);

		if (ShieldEquiped(i))
		{
			if (Entity[i].prevState.eFlags & FLAG_CROUCHED)
			{
				if ((Distance < 1.5) && (Pitch >= 0.0f) && ((Entity[cg->ClientNumber].pose.Origin.z - Entity[i].pose.Origin.z) > 10))
					CurrentRiotBone[i] = BONE_SET_ALL;
				else
				{
					if ((Yaw < 47.0f))
					{
						if (!(Entity[cg->ClientNumber].prevState.eFlags & FLAG_CROUCHED) && !(Entity[cg->ClientNumber].prevState.eFlags & FLAG_PRONE) && (Distance < 1) && (Pitch > 24.0f))
							CurrentRiotBone[i] = BONE_SET_ALL;
						else if ((Pitch <= 70.0f) && (Pitch >= 40))
							CurrentRiotBone[i] = BONE_SET_ALL;
						else if ((Pitch < -(25 / Distance)))
							CurrentRiotBone[i] = BONE_SET_LEFT;
						else
							CurrentRiotBone[i] = BONE_SET_NONE;
					}
					else if ((Yaw < 60.0f) && (Yaw >= 47.0f))
						CurrentRiotBone[i] = BONE_SET_RIGHT;
					else if ((Yaw < 80.0f) && (Yaw >= 60.0f))
						CurrentRiotBone[i] = BONE_SET_HIP;
					else if ((Yaw < 120.0f) && (Yaw >= 80.0f))
						CurrentRiotBone[i] = BONE_SET_NO_HEAD;
					else if ((Yaw >= 120.0f))
						CurrentRiotBone[i] = BONE_SET_ALL;
					else
						CurrentRiotBone[i] = BONE_SET_NONE;
				}
			}
			else if (!(Entity[i].nextState.lerp.eFlags & FLAG_PRONE))
			{
				if ((Yaw < 65.0f))
				{
					if (((Distance < 1.5) && (Pitch >= 0.0f) && ((Entity[cg->ClientNumber].pose.Origin.z - Entity[i].pose.Origin.z) > 10)) || ((Distance < 1.5) && (Pitch >= 40.0f)))
						CurrentRiotBone[i] = BONE_SET_ALL;
					else if ((Distance < 1.5) && (Distance > 1.0) && (Pitch <= -15.0f) && (Pitch > -43.0f))
						CurrentRiotBone[i] = BONE_SET_LEFT;
					else if ((Distance < 1.5) && (Distance > 1.0) && (Pitch <= -43.0f) && (Pitch > -52.0f))
						CurrentRiotBone[i] = BONE_SET_LEGS;
					else if ((Distance < 1.5) && (Distance > 1.0) && (Pitch <= -52.0f) && (Pitch > -67.0f))
						CurrentRiotBone[i] = BONE_SET_HIP_LEGS;
					else if ((Distance < 1.5) && (Distance > 1.0) && (Pitch <= -67.0f))
						CurrentRiotBone[i] = BONE_SET_ALL;
					else if (Distance >= 4.11f)
					{
						if ((Pitch > 69.9f))
							CurrentRiotBone[i] = BONE_SET_ALL;
						else if ((Pitch <= -5.9f) && (Pitch > -27.9f))
							CurrentRiotBone[i] = BONE_SET_LEGS;
						else if ((Pitch <= -27.9f) && (Pitch > -53.9f))
							CurrentRiotBone[i] = BONE_SET_HIP_LEGS;
						else if ((Pitch <= -53.9f) && (Pitch > -63.0f))
							CurrentRiotBone[i] = BONE_SET_NO_HEAD;
						else if (Pitch <= -63.0f)
							CurrentRiotBone[i] = BONE_SET_ALL;
						else
							CurrentRiotBone[i] = BONE_SET_FEET;
					}
					else
					{
						if ((Pitch > 49.9f))
							CurrentRiotBone[i] = BONE_SET_ALL;
						else if ((Pitch <= -5.9f) && (Pitch > -25.9f))
							CurrentRiotBone[i] = BONE_SET_FEET;
						else if ((Pitch <= -25.9f) && (Pitch > -40.0f))
							CurrentRiotBone[i] = BONE_SET_LEGS;
						else if ((Pitch <= -40.0f) && (Pitch > -61.0f))
							CurrentRiotBone[i] = BONE_SET_HIP_LEGS;
						else if ((Pitch <= -61.0f) && (Pitch > -67.0f))
							CurrentRiotBone[i] = BONE_SET_NO_HEAD;
						else if ((Pitch <= -67.0f))
							CurrentRiotBone[i] = BONE_SET_ALL;
						else
							CurrentRiotBone[i] = BONE_SET_LEFT;
					}
				}
				else if ((Yaw < 95.0f) && (Yaw >= 65.0f))
					CurrentRiotBone[i] = BONE_SET_HIP;
				else if ((Yaw < 120.0f) && (Yaw >= 95.0f))
					CurrentRiotBone[i] = BONE_SET_NO_HEAD;
				else if ((Yaw >= 120.0f))
					CurrentRiotBone[i] = BONE_SET_ALL;
				else
					CurrentRiotBone[i] = BONE_SET_NONE;
			}
			else
				CurrentRiotBone[i] = BONE_SET_NONE;
		}
		else if (ShieldStowed(i))
		{
			if (Entity[i].prevState.eFlags & FLAG_CROUCHED)
			{
				if (((Yaw >= 184.0f) && (PlayerSide == LeftSide)) || ((Yaw >= 175.0f) && (PlayerSide == CenterSide)))
					CurrentRiotBone[i] = BONE_SET_NONE;
				else if ((Yaw < 200.0f) && (Yaw > 193.0f) && (PlayerSide == RightSide))
					CurrentRiotBone[i] = BONE_SET_RIGHT_ELBOW_KNEE;
				else if (((Yaw < 193.0f) && (PlayerSide == RightSide)) || ((Yaw <= 127.0f) && (PlayerSide == LeftSide)) || ((Yaw < 140.0f) && (PlayerSide == CenterSide)))
					CurrentRiotBone[i] = BONE_SET_ALL;
				else if ((Yaw < 178.0f) && (Yaw > 156.0f) && (PlayerSide == LeftSide))
					CurrentRiotBone[i] = BONE_SET_LEFT_LEG;
				else if ((Yaw < 156.0f) && (Yaw > 127.0f) && (PlayerSide == LeftSide))
					CurrentRiotBone[i] = BONE_SET_LEFT_HIP_LEG;
				else
					CurrentRiotBone[i] = BONE_SET_NONE;
			}
			else if (Entity[i].prevState.eFlags & FLAG_PRONE)
			{
				if ((Yaw > 46.0f) && (Yaw < 140.0f) && ((Entity[cg->ClientNumber].pose.Origin.z - Entity[i].pose.Origin.z) > 5))
					CurrentRiotBone[i] = BONE_SET_NONE;
				else if (((Yaw >= 200.0f) && (PlayerSide == RightSide)) || ((Yaw >= 175.0f) && (PlayerSide == LeftSide)) || ((Yaw >= 175.0f) && (PlayerSide == CenterSide)))
				{
					if (Distance <= 1.0)
						CurrentRiotBone[i] = BONE_SET_NONE;
					else if (Distance <= 5.5)
						CurrentRiotBone[i] = BONE_SET_ANKLES;
					else
						CurrentRiotBone[i] = BONE_SET_LEGS;
				}
				else if ((Yaw >= 196.0f) && (Yaw <= 200.0f) && (PlayerSide == RightSide))
					CurrentRiotBone[i] = BONE_SET_RIGHT;
				else if ((Yaw < 175.0f) && (Yaw >= 150.0f) && (PlayerSide == LeftSide))
					CurrentRiotBone[i] = BONE_SET_LEFT_LEG;
				else if (((Yaw <= 184.0f) && (PlayerSide == RightSide)) || ((Yaw < 150.0f) && (PlayerSide == LeftSide)) || ((Yaw < 90.0f) && (PlayerSide == CenterSide)))
					CurrentRiotBone[i] = BONE_SET_NECK;
				else
					CurrentRiotBone[i] = BONE_SET_NONE;
			}
			else
			{
				if (((Yaw >= 200.0f) && (PlayerSide == RightSide)) || ((Yaw >= 175.0f) && (PlayerSide == LeftSide)) || ((Yaw >= 175.0f) && (PlayerSide == CenterSide)))
				{
					if (Distance <= 1.0)
						CurrentRiotBone[i] = BONE_SET_NONE;
					else if (Distance <= 5.5)
						CurrentRiotBone[i] = BONE_SET_ANKLES;
					else
						CurrentRiotBone[i] = BONE_SET_LEGS;
				}
				else if ((Yaw > 184.0f) && (Yaw < 196.0f) && (PlayerSide == RightSide))
					CurrentRiotBone[i] = BONE_SET_RIGHT_HIP_LEG;
				else if ((Yaw >= 196.0f) && (Yaw <= 200.0f) && (PlayerSide == RightSide))
					CurrentRiotBone[i] = BONE_SET_RIGHT;
				else if ((Yaw < 175.0f) && (Yaw >= 150.0f) && (PlayerSide == LeftSide))
					CurrentRiotBone[i] = BONE_SET_LEFT_LEG;
				else if (((Yaw <= 184.0f) && (PlayerSide == RightSide)) || ((Yaw < 150.0f) && (PlayerSide == LeftSide)) || ((Yaw < 90.0f) && (PlayerSide == CenterSide)))
					CurrentRiotBone[i] = BONE_SET_ALL;
				else
					CurrentRiotBone[i] = BONE_SET_NONE;
			}
		}
	}
}

void Sys_EnterCriticalSection(int CriticalSection)
{
	void(*Sys_EnterCriticalSection)(int CriticalSection) = (void(*)(int))addr->Sys_EnterCriticalSection;
	Sys_EnterCriticalSection(CriticalSection);
}

void Sys_LeaveCriticalSection(int CriticalSection)
{
	void(*Sys_LeaveCriticalSection)(int CriticalSection) = (void(*)(int))addr->Sys_LeaveCriticalSection;
	Sys_LeaveCriticalSection(CriticalSection);
}

bool CheckVis(int client, Vector3 Pos)
{
	Sys_EnterCriticalSection(0xB);
	Sys_EnterCriticalSection(0xC);

	trace_t trace;
	Vector3 nullvec;
	void(*CG_LocationalTrace)(trace_t *results, Vector3 *start, Vector3 *end, int passEntityNum, int contentMask) = (void(*)(trace_t *, Vector3 *, Vector3 *, int, int))addr->CG_LocationalTrace;
	CG_LocationalTrace(&trace, &cg->refdef.ViewOrigin, &Pos, cg->ClientNumber, 0x803003);

	Sys_LeaveCriticalSection(0xB);
	Sys_LeaveCriticalSection(0xC);

	return (trace.fraction >= 1.00f);
}

bool CheckWalls(int client, Vector3 Pos)
{
	bool ret = false;

	Sys_EnterCriticalSection(0xB);
	Sys_EnterCriticalSection(0xC);

	if (CE.Autowall == NormalAutoWall || ((CE.Autowall == pAutoNormal) && !ClientOptions[client].Prioritized))
		ret = BasicBulletHitPlayer(Pos, Entity[client]);

	if (CE.Autowall == pAutoWall || ((CE.Autowall == pAutoNormal) && ClientOptions[client].Prioritized))
		ret = BulletHitPlayer(Pos, Entity[client]);

	Sys_LeaveCriticalSection(0xB);
	Sys_LeaveCriticalSection(0xC);

	return ret;
}

inline void CheckVisAndWalls(int client, const char* Bones[], int number)
{
	for (int i = 0; i < number; i++)
	{
		Vector3 TagPos;
		if (GetTagPos(client, Bones[i], &TagPos, true))
		{
			CE.IsKillable[client] = CheckWalls(client, TagPos);
			CE.CanSeePlayer[client] = CheckVis(client, TagPos);

			if (CE.CanSeePlayer[client] || CE.IsKillable[client]) {
				user_strcpy((char*)CE.AimbotTag[client], Bones[i]);
				return;
			}
		}
	}

	user_strcpy((char*)CE.AimbotTag[client], "j_head");
	CE.IsKillable[client] = false;
	CE.CanSeePlayer[client] = false;
}

inline void CheckVisAndWalls(int client, const char* Bone)
{
	Vector3 TagPos;
	if (GetTagPos(client, Bone, &TagPos, true))
	{
		CE.IsKillable[client] = CheckWalls(client, TagPos);
		CE.CanSeePlayer[client] = CheckVis(client, TagPos);

		if (CE.CanSeePlayer[client] || CE.IsKillable[client]) {
			user_strcpy((char*)CE.AimbotTag[client], Bone);
			return;
		}
	}

	user_strcpy((char*)CE.AimbotTag[client], "j_head");
	CE.IsKillable[client] = false;
	CE.CanSeePlayer[client] = false;
}

void DoClientChecks()
{
	for (int i = 0; i < 12; i++)
	{
		if ((cg->ClientNumber == cg->ps.clientNum) && (cg->ps.Health > 0) && GoodEnemy(i))
		{
			if (HasShield(i))
			{
				switch (CurrentRiotBone[i])
				{
				case BONE_SET_ALL:
					CheckVisAndWalls(i, HitBoxesP, ARRAYSIZE(HitBoxesP));
					break;
				case BONE_SET_NO_HEAD:
					CheckVisAndWalls(i, RiotBoxesNoHead, ARRAYSIZE(RiotBoxesNoHead));
					break;
				case BONE_SET_LEFT:
					CheckVisAndWalls(i, RiotBoxesLe, ARRAYSIZE(RiotBoxesLe));
					break;
				case BONE_SET_RIGHT:
					CheckVisAndWalls(i, RiotBoxesRi, ARRAYSIZE(RiotBoxesRi));
					break;
				case BONE_SET_RIGHT_HIP_LEG:
					CheckVisAndWalls(i, RiotBoxesRiHipLeg, ARRAYSIZE(RiotBoxesRiHipLeg));
					break;
				case BONE_SET_LEFT_LEG:
					CheckVisAndWalls(i, RiotBoxesLeLeg, ARRAYSIZE(RiotBoxesLeLeg));
					break;
				case BONE_SET_RIGHT_LEG:
					CheckVisAndWalls(i, RiotBoxesRiLeg, ARRAYSIZE(RiotBoxesRiLeg));
					break;
				case BONE_SET_ANKLES:
					CheckVisAndWalls(i, RiotBoxesAnkles, ARRAYSIZE(RiotBoxesAnkles));
					break;
				case BONE_SET_RIGHT_ELBOW_KNEE:
					CheckVisAndWalls(i, RiotBoxesRiElbowknee, ARRAYSIZE(RiotBoxesRiElbowknee));
					break;
				case BONE_SET_LEFT_HIP_LEG:
					CheckVisAndWalls(i, RiotBoxesLeHipLeg, ARRAYSIZE(RiotBoxesLeHipLeg));
					break;
				case BONE_SET_HIP:
					CheckVisAndWalls(i, RiotBoxesHip, ARRAYSIZE(RiotBoxesHip));
					break;
				case BONE_SET_FEET:
					CheckVisAndWalls(i, RiotBoxesFeet, ARRAYSIZE(RiotBoxesFeet));
					break;
				case BONE_SET_LEGS:
					CheckVisAndWalls(i, RiotBoxesLegs, ARRAYSIZE(RiotBoxesLegs));
					break;
				case BONE_SET_HIP_LEGS:
					CheckVisAndWalls(i, RiotBoxesHipLegs, ARRAYSIZE(RiotBoxesHipLegs));
					break;
				case BONE_SET_NECK:
					CheckVisAndWalls(i, "j_neck");
					break;
				case BONE_SET_NONE:
					user_strcpy((char*)CE.AimbotTag[i], "j_head");
					CE.IsKillable[i] = false;
					CE.CanSeePlayer[i] = false;
					break;
				}
			}
			else if (ClientOptions[i].Prioritized)
				CheckVisAndWalls(i, HitBoxesP, ARRAYSIZE(HitBoxesP));
			else if (CE.AimTag == 0)
				CheckVisAndWalls(i, HitBoxes, ARRAYSIZE(HitBoxes));
			else
				CheckVisAndWalls(i, TagList[CE.AimTag - 1]);
		}
	}
}

bool Mutex;
jmp_buf env;

LONG WINAPI OurCrashHandler(EXCEPTION_POINTERS * ExceptionInfo)
{
	if (Mutex)
	{
		Mutex = false;

		printf("Thread Crashed at 0x%X\n", ExceptionInfo->ExceptionRecord->ExceptionAddress);

		int(*Com_Error)(int imageTrack, const char * name) = (int(*)(int imageTrack, const char * name))addr->Com_Error;
		Com_Error(1, va("Error found at 0x%X\nPlease contact ^5xbOnline^7 support", ExceptionInfo->ExceptionRecord->ExceptionAddress));
	}

	return  EXCEPTION_EXECUTE_HANDLER;
}

clientMigState_t CL_GetLocalClientMigrationState(int localclient)
{
	return clientUIActive->migrationState;
}

int TLS_Pointer = 0;
unsigned int Get_BGS()
{
	return (*(int*)(addr->cg_s) + 0x7A418);
}

unsigned int Get_BGS_Pointer()
{
	return *(unsigned int*)(TLS_Pointer + 0x08);
}

void Set_BGS_Pointer(unsigned int bgs)
{
	*(unsigned int*)(TLS_Pointer + 0x08) = bgs;
}

DWORD WINAPI ExecuteAutoWall(LPVOID Params)
{
starThread:

	//static va_info_t va_buffer;
	//static cbrush_t box_brush;
	//static cmodel_t box_model;
	//static PhysGeomList* geoms;
	//static unsigned short partitions[0x39e6];
	//static unsigned short brushes[0x4888];
	//static TraceThreadInfo threadInfo = { { 0, partitions, brushes }, &box_brush, &box_model, &geoms };
	//static CmdArgs cmd_args;
	//static tls_t tls = { 0, &va_buffer, &env, &threadInfo, &cmd_args };
	//
	//int tls_ptr = 0;
	//__asm mr tls_ptr, r13;
	//TLS_Pointer = *(int*)(tls_ptr);
	//
	//*(int*)(TLS_Pointer + 0x70) = (int)&tls;
	////*(int*)(r29 + 0x08) = addr->_0x82F0CC88; //bgs
	//
	//int ret = setjmp(env);
	//
	//Mutex = true;
	//
	//cmd_args.nesting = -1;
	//cmd_args.totalUsedArgvPool = 0;
	//cmd_args.totalUsedTextPool = 0;

	while (!KillThread)
	{
		__try
		{
			if ((XamGetCurrentTitleID() != 0x415608CB))
				break;

			if (ShouldHookRun())
			{
				if (Get_BGS() != Get_BGS_Pointer())
					Set_BGS_Pointer(Get_BGS());

				
			}
		}
		__except (filter("ExecuteThread", GetExceptionCode(), GetExceptionInformation()))
		{
			break;
		}

		Sleep(10);
	}

	if (KillThread != true)
		goto starThread;

	DWORD Thr_Exit = 0;
	ExitThread(Thr_Exit);
}

void InitializeAutoWallThreads()
{
	HANDLE AutoWallThread = 0; DWORD AutoWallThreadID = 0;
	AutoWallThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ExecuteAutoWall, NULL, 3, &AutoWallThreadID);
	XSetThreadProcessor(AutoWallThread, 1);
	ResumeThread(AutoWallThread);
	CloseHandle(AutoWallThread);
}

void BG_GetSpreadForWeapon(playerState_s* PlayerState, int Weapon, float *minSpread, float *maxSpread)
{
	void(*BG_GetSpreadForWeapon)(playerState_s* PlayerState, int Weapon, float *minSpread, float *maxSpread) = (void(*)(playerState_s*, int, float*, float*))addr->BG_GetSpreadForWeapon;
	BG_GetSpreadForWeapon(PlayerState, Weapon, minSpread, maxSpread);
}

int TransformSeed(int *pHoldrand)
{
	int(*TransformSeed)(int *pHoldrand) = (int(*)(int*))addr->TransformSeed;
	return TransformSeed(pHoldrand);
}

void RandomBulletDir(int* randSeed, float *x, float *y)
{
	void(*RandomBulletDir)(int* randSeed, float *x, float *y) = (void(*)(int* randSeed, float *x, float *y))addr->RandomBulletDir;
	RandomBulletDir(randSeed, x, y);
}

void GetWeaponSpread(float* Spread)
{
	float MinSpread = 0.0f;
	float MaxSpread = 0.0f;

	BG_GetSpreadForWeapon(&cg->ps, Entity[cg->ClientNumber].nextState.Weapon, &MinSpread, &MaxSpread);
	*Spread = (((MaxSpread - MinSpread) * (cg->ps.SpreadMultiplier / 255.0f)) + MinSpread);
}

void NoSpread(usercmd_s *cmd)
{
	if (!ClientActive_t->flZoomProgress)
	{
		float Spread = 0.0f;
		GetWeaponSpread(&Spread);

		int ServerTime = cmd->time;
		TransformSeed(&ServerTime);

		float SpreadX, SpreadY;
		RandomBulletDir(&ServerTime, &SpreadX, &SpreadY);

		SpreadX *= Spread;
		SpreadY *= Spread;

		cmd->viewAngles[0] += ANGLE2SHORT(SpreadY);
		cmd->viewAngles[1] += ANGLE2SHORT(SpreadX);
	}
}

float WallDepth(float *p1, float *p2)
{
	Vector3 v;
	VectorSubtract(p2, p1, v);
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float WallDepthA(Vector3 c1, Vector3 c2)
{
	Vector3 Sub = c1 - c2;
	return sqrt((Sub.x * Sub.x) + (Sub.y * Sub.y));
}

float GetAimDegree(int t)
{
	Vector3 EnemyPos;
	if (!GetTagPos(t, "j_head", &EnemyPos))
		return 0;

	return WallDepth(AnglesToForward(EnemyPos, Entity[t].prevState.apos.trBase, WallDepth(cg->refdef.ViewOrigin, EnemyPos)), cg->refdef.ViewOrigin);
}

float GetAimDegreeA(int t)
{
	Vector3 ClientAngles = Entity[t].nextState.lerp.apos.trBase;
	ClientAngles.x = 0;
	return WallDepthA(AnglesToForward(Entity[t].pose.Origin, ClientAngles, WallDepthA(Entity[cg->ClientNumber].pose.Origin, Entity[t].pose.Origin)), Entity[cg->ClientNumber].pose.Origin);
}

bool isAimingAtMe(int *t, float *a)
{
	for (int i = 0; i < 12; i++) {
		if (i == cg->ClientNumber)
			continue;

		if (!(Entity[i].pose.eType & 1))
			continue;;

		if (!cg->characterinfo[i].infoValid)
			continue;

		if (cuser_strcmp(cg->clientinfo[i].Name, ""))
			continue;

		if (!IsEnemy(i))
			continue;

		float Degree = GetAimDegree(i);
		if (Degree > 50.0f)
			continue;

		*a = Degree;
		*t = i;
		return true;
	}

	return false;
}


bool CloseToMe(int *outId)
{
	for (int i = 0; i < 12; i++)
	{
		if (i == cg->ClientNumber)
			continue;

		if (!(Entity[i].pose.eType & 1))
			continue;

		if (!cg->characterinfo[i].infoValid)
			continue;

		if (cuser_strcmp(cg->clientinfo[i].Name, ""))
			continue;

		if (!IsEnemy(i))
			continue;

		if (GetDistance(Entity[i].pose.Origin, cg->refdef.ViewOrigin) < 10) {
			if (outId != NULL)
				*outId = i;
			return true;
		}
	}
	return false;
}

bool isAnyVis(int mClient, int *outId)
{
	for (int i = 0; i < 12; ++i)
	{
		if (i == mClient)
			continue;

		if (!IsEnemy(i))
			continue;

		if (!(Entity[i].pose.eType & 1))
			continue;

		if (!cg->characterinfo[i].infoValid)
			continue;

		if (cuser_strcmp(cg->clientinfo[i].Name, ""))
			continue;

		if (!CE.CanSeePlayer[i])
			continue;

		if (outId != NULL)
			*outId = i;

		return true;
	}
	return false;
}

bool GoodEnemy(int client)
{
	if (client == cg->ClientNumber)
		return false;

	if (!IsEnemy(client))
	{
		if (!ClientOptions[client].Prioritized)
			return false;
	}

	if (!(Entity[client].pose.eType & 1))
		return false;

	if (!cg->characterinfo[client].infoValid)
		return false;

	if (cuser_strcmp(cg->clientinfo[client].Name, ""))
		return false;

	return true;
}

bool GoodPlayer(int client)
{
	if (client == cg->ClientNumber)
		return false;

	if (!(Entity[client].pose.eType & 1))
		return false;

	if (!cg->characterinfo[client].infoValid)
		return false;

	if (cuser_strcmp(cg->clientinfo[client].Name, ""))
		return false;

	return true;
}

void CG_PlayRumbleOnClient(char* rumblename)
{
	if (Dvar_GetBool("cl_ingame") && GetStructs() && CE.ControllerVibration)
	{
		static void(*CG_PlayRumbleOnClient)(int LocalClientNumber, char* rumbleName) = (void(*)(int, char*))addr->CG_PlayRumbleOnClient;
		CG_PlayRumbleOnClient(0, rumblename);
	}
}

void UI_PlaySound(const char* snd)
{
	if (CE.MenuSounds)
	{
		static void(*UI_PlaySound)(int context, const char *snd, int alias) = (void(*)(int, const char *, int))addr->UI_PlaySound;
		UI_PlaySound(0, snd, 0);
	}
}

bool BulletTrace(int localClientNum, BulletFireParams* bp, centity_s* cent, BulletTraceResults* br, int lastSurfaceType)
{
	bool(*BulletTrace)(int localClientNum, BulletFireParams* bp, centity_s* cent, BulletTraceResults* br, int lastSurfaceType) = (bool(*)(int, BulletFireParams*, centity_s*, BulletTraceResults*, int))addr->BulletTrace;
	return BulletTrace(localClientNum, bp, cent, br, lastSurfaceType);
}

bool BG_AdvanceTrace(BulletFireParams *bp, BulletTraceResults *br, float dist)
{
	bool(*BG_AdvanceTrace)(BulletFireParams *bp, BulletTraceResults *br, float dist) = (bool(*)(BulletFireParams*, BulletTraceResults*, float))addr->BG_AdvanceTrace;
	return BG_AdvanceTrace(bp, br, dist);
}

float BG_GetSurfacePenetrationDepth(int penetrateType, bool isAlternate, int surfaceFlags)
{
	float(*BG_GetSurfacePenetrationDepth)(int penetrateType, bool isAlternate, int surfaceFlags) = (float(*)(int, bool, int))addr->BG_GetSurfacePenetrationDepth;
	return BG_GetSurfacePenetrationDepth(penetrateType, isAlternate, surfaceFlags);
}

int BG_GetPerkIndexForName(const char *perkName)
{
	int(*BG_GetPerkIndexForName)(const char *perkName) = (int(*)(const char *))addr->BG_GetPerkIndexForName;
	return BG_GetPerkIndexForName(perkName);
}

int BG_GetAmmoInClip(playerState_s *ps, int weapon)
{
	int(*BG_GetAmmoInClip)(playerState_s *ps, int weapon, bool isAlternate, int unk) = (int(*)(playerState_s *, int, bool, int))addr->BG_GetAmmoInClip;
	return BG_GetAmmoInClip(ps, weapon, 0, 0);
}

bool CL_IsServerRestarting()
{
	bool(*CL_IsServerRestarting)() = (bool(*)())addr->CL_IsServerRestarting;
	return CL_IsServerRestarting();
}

bool CG_IsFullyInitialized()
{
	if (cg)
		return (cg->nextSnap != 0);
	else
		return false;
}

bool BG_HasPerk(const char *perk)
{
	int r30 = BG_GetPerkIndexForName(perk);
	if (r30 != 0x35)
	{
		if ((*(int*)((int)&cg->snap->ps + (((r30 >> 5) + 0x28D) << 2)) & (1 << (r30 & 0x1F))) <= 0)
			return ((*(int*)((int)&cg->snap->ps + (((r30 >> 5) + 0x28D) << 2)) & (1 << (r30 & 0x1F))) - 0);
		else
			return ((*(int*)((int)&cg->snap->ps + (((r30 >> 5) + 0x28D) << 2)) & (1 << (r30 & 0x1F))) - ((*(int*)((int)&cg->snap->ps + (((r30 >> 5) + 0x28D) << 2)) & (1 << (r30 & 0x1F))) + -1));
	}
	return false;
}

bool CG_IsSelectingLocation(int localClientNum)
{
	bool(*CG_IsSelectingLocation)(int localClientNum) = (bool(*)(int))addr->CG_IsSelectingLocation;
	return CG_IsSelectingLocation(localClientNum);
}

//TODO: Figure out why this was not returning correctly
bool ShouldHookRun()
{
	return (Dvar_GetBool("cl_ingame") && GetStructs() && CG_IsFullyInitialized() && (cg->ps.pm_type != 7) /*&& !CL_IsServerRestarting() && CL_GetLocalClientMigrationState(0) == CMSTATE_INACTIVE && cg->renderScreen && (cg->ps.clientNum == cg->ClientNumber) && (cg->ps.Health > 0) && (clientUIActive->connectionState == CA_ACTIVE) && clientUIActive->active && clientUIActive->isRunning && clientUIActive->cgameInitCalled && clientUIActive->cgameInitialized*/);
}

bool CReadFile(const char* FileName, MemoryBuffer &pBuffer)
{
	HANDLE hFile;  DWORD dwFileSize, dwNumberOfBytesRead;
	hFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return FALSE;
	}

	dwFileSize = GetFileSize(hFile, NULL);
	unsigned char* lpBuffer = (unsigned char*)malloc(dwFileSize);
	if (lpBuffer == NULL) {
		CloseHandle(hFile);
		return FALSE;
	}

	if (ReadFile(hFile, lpBuffer, dwFileSize, &dwNumberOfBytesRead, NULL) == FALSE) {
		free(lpBuffer);
		CloseHandle(hFile);
		return FALSE;
	}
	else if (dwNumberOfBytesRead != dwFileSize) {
		free(lpBuffer);
		CloseHandle(hFile);
		return FALSE;
	}

	CloseHandle(hFile);
	pBuffer.Add(lpBuffer, dwFileSize);
	free(lpBuffer);
	return TRUE;
}