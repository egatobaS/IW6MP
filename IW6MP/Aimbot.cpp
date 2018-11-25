#include "stdafx.h"
#include "Menu.h"
#include "Functions.h"
#include "Drawing.h"
#include "Aimbot.h"
#include "ESP.h"
#include "XAM_191.h"

float DistanceReturn;
float Get3dDistance(Vector3 &c1, Vector3 &c2)
{
	Vector3 disttemp = c2 - c1;
	DistanceReturn = sqrt((float)((disttemp.x * disttemp.x) + (disttemp.y * disttemp.y) + (disttemp.z * disttemp.z)));
	return DistanceReturn;
}

int MostViableClient;
int GetTargetClosest(bool Priority)
{
	MostViableClient = -1;
	float MaxDistance = 99999999;
	for (int i = 0; i < 12; i++)
	{
		if (i == cg->ClientNumber)
			continue;

		if (!ClientOptions[i].Prioritized && Priority)
			continue;

		if (ClientOptions[i].Ignored)
			continue;

		if (!(Entity[i].pose.eType & 1))
			continue;

		if ((!cg->characterinfo[i].infoValid))
			continue;

		if ((cg->clientinfo[i].Name == NULL))
			continue;

		if (cuser_strcmp(cg->clientinfo[i].Name, ""))
			continue;

		if (!(Entity[i].nextState.Type == ET_PLAYER))
			continue;

		if (!IsEnemy(i))
			continue;

		float CurDistance = Get3dDistance(Entity[cg->ClientNumber].pose.Origin, Entity[i].pose.Origin);
		if (CurDistance < MaxDistance) {
			MaxDistance = CurDistance;
			MostViableClient = i;
		}
	}

	return MostViableClient;
}

int GetVisibleAndKillableTarget(bool Priority, bool AutoWall)
{
	MostViableClient = -1;
	float MaxDistance = 99999999;
	for (int i = 0; i < 12; i++)
	{
		if (i == cg->ClientNumber)
			continue;

		if (!ClientOptions[i].Prioritized && Priority)
			continue;

		if (ClientOptions[i].Ignored)
			continue;

		if (!(Entity[i].pose.eType & 1))
			continue;

		if ((!cg->characterinfo[i].infoValid))
			continue;

		if ((cg->clientinfo[i].Name == NULL))
			continue;

		if (cuser_strcmp(cg->clientinfo[i].Name, ""))
			continue;

		if (!(Entity[i].nextState.Type == ET_PLAYER))
			continue;

		if (!IsEnemy(i))
			continue;

		if (!CE.CanSeePlayer[i] && !AutoWall)
			continue;

		if (!CE.IsKillable[i] && AutoWall)
			continue;

		float CurDistance = Get3dDistance(Entity[cg->ClientNumber].pose.Origin, Entity[i].pose.Origin);
		if (CurDistance < MaxDistance) {
			MaxDistance = CurDistance;
			MostViableClient = i;
		}
	}

	return MostViableClient;
}

int GetTargetOnScreen(bool Priority)
{
	MostViableClient = -1;
	float MaxDistance = 99999999;
	for (int i = 0; i < 12; i++)
	{
		if (i == cg->ClientNumber)
			continue;

		if (!ClientOptions[i].Prioritized && Priority)
			continue;

		if (ClientOptions[i].Ignored)
			continue;

		if (!(Entity[i].pose.eType & 1))
			continue;

		if ((!cg->characterinfo[i].infoValid))
			continue;

		if ((cg->clientinfo[i].Name == NULL))
			continue;

		if (cuser_strcmp(cg->clientinfo[i].Name, ""))
			continue;

		if (!(Entity[i].nextState.Type == ET_PLAYER))
			continue;

		if (!IsEnemy(i))
			continue;

		if (!CE.CanSeePlayer[i])
			continue;

		Vector3 Tagpos;
		if (!GetTagPos(i, CE.AimbotTag[i], &Tagpos))
			continue;

		Vector2 Location;
		if (!WorldToScreen(Tagpos, Location, true))
			continue;

		float CurDistance = Get3dDistance(Entity[cg->ClientNumber].pose.Origin, Entity[i].pose.Origin);
		if (CurDistance < MaxDistance) {
			MaxDistance = CurDistance;
			MostViableClient = i;
		}
	}

	return MostViableClient;
}

int GetMostViableClient()
{
	if (CE.TargetType == Closest)
		return GetTargetClosest();
	else if (CE.TargetType == Visible)
	{
		int hold = GetVisibleAndKillableTarget(true, false);
		if (hold == -1)
			return GetVisibleAndKillableTarget(false, false);
		else
			return hold;
	}
	else if (CE.TargetType == Logical)
	{
		int hold = GetVisibleAndKillableTarget(true, false);
		if (hold == -1)
		{
			hold = GetVisibleAndKillableTarget(false, false);
			if (hold == -1)
			{
				hold = GetVisibleAndKillableTarget(true, true);
				if (hold == -1)
					return GetVisibleAndKillableTarget(false, true);
			}
		}
		return hold;
	}
	else if (CE.TargetType == OnScreen)
	{
		int hold = GetTargetOnScreen(true);
		if (hold == -1)
			return GetTargetOnScreen(false);
		return hold;
	}
	return -1;
}

void SetViewAngles()
{
	if (CE.AimbotStatus)
	{
		if (CE.AimbotType == Normal)
		{
			if ((CE.AimbotTarget != -1))
			{
				Vector3 TagPos;
				if (!GetTagPos(CE.AimbotTarget, ClientOptions[CE.AimbotTarget].AimAtRoot ? "j_mainroot" : ((CE.AimbotTag[CE.AimbotTarget] > 0) ? CE.AimbotTag[CE.AimbotTarget] : TagList[CE.AimTag - 1]), &TagPos, true))
					return;

				if (CE.AimPrediction)
					PosPrediction(&Entity[CE.AimbotTarget], TagPos, (cg->snap->ping / 20), true);

				if (CE.PosPrediction) {
					TagPos = VelocityPrediction(&Entity[CE.AimbotTarget], TagPos, CE.PosPredictionScale); // 0.0 - 0.75
				}

				ClientActive_t->viewAngle = vectorToAngles(TagPos - cg->refdef.ViewOrigin) - ClientActive_t->baseAngle;
			}
		}
		else if (CE.AimbotType == Snap)
		{
			if ((CE.AimbotTarget != -1))
			{
				Vector3 TagPos;
				if (!GetTagPos(CE.AimbotTarget, ClientOptions[CE.AimbotTarget].AimAtRoot ? "j_mainroot" : ((CE.AimbotTag[CE.AimbotTarget] > 0) ? CE.AimbotTag[CE.AimbotTarget] : TagList[CE.AimTag - 1]), &TagPos, true))
					return;

				if (CE.AimPrediction) {
					PosPrediction(&Entity[CE.AimbotTarget], TagPos, (cg->snap->ping / 20), true);
				}

				if (CE.PosPrediction) {
					TagPos = VelocityPrediction(&Entity[CE.AimbotTarget], TagPos, CE.PosPredictionScale); // 0.0 - 0.75
				}

				if (!CE.LockedOnToTarget) {
					CE.OldAngles = ClientActive_t->viewAngle;
					CE.LockedOnToTarget = true;
				}

				ClientActive_t->viewAngle = vectorToAngles(TagPos - cg->refdef.ViewOrigin) - ClientActive_t->baseAngle;
			}
			else
			{
				if (CE.LockedOnToTarget)
				{
					ClientActive_t->viewAngle = CE.OldAngles;
					CE.LockedOnToTarget = false;
				}
			}
		}
	}
}

void DoSetViewAngles()
{
	if ((GetAsyncKeyState(XINPUT_GAMEPAD_LEFT_SHOULDER) && (CE.AimKey == LB)) || (GetAsyncKeyState(XINPUT_GAMEPAD_RIGHT_SHOULDER) && (CE.AimKey == RB)) || (GetAsyncKeyState(KEY_LT) && (CE.AimKey == LT)) || (GetAsyncKeyState(KEY_RT) && (CE.AimKey == RT)) || (CE.AimKey == NoAimKey))
		SetViewAngles();
}

bool bFirstShot = true;

void SetSilentAngles(usercmd_s* cur, usercmd_s* old, usercmd_s* New)
{
	if (CE.AimbotStatus)
	{
		if ((CE.AimbotTarget != -1))
		{

			if (CE.ShouldRunXAM_191 && CE.AimbotStatus && (CE.AimbotTarget != -1))
			{
				if (CE.AutoShoot && (bFirstShot || (cg->time < CE.StoredTimeForShotDelay) || ((cg->time - CE.StoredTimeForShotDelay) > CE.DelayTimeFix)))
				{
					CE.StoredTimeForShotDelay = cg->time;
					if (!strstr(GetWeaponName(Entity[cg->ClientNumber].nextState.Weapon), "Riot Shield") || (BG_GetAmmoInClip(&cg->ps, cg->ps.Weapon) > 0)) {

						old->buttons |= 1;

						bFirstShot = false;
					}
				}
			}


			if (CE.AimbotType == Silent && old->buttons & AUTO_SHOOT)
			{
				Vector3 TagPos;
				if (!GetTagPos(CE.AimbotTarget, ClientOptions[CE.AimbotTarget].AimAtRoot ? "j_mainroot" : ((CE.AimbotTag[CE.AimbotTarget] > 0) ? CE.AimbotTag[CE.AimbotTarget] : TagList[CE.AimTag - 1]), &TagPos, true))
					return;

				if (CE.AimPrediction) {
					PosPrediction(&Entity[CE.AimbotTarget], TagPos, (cg->snap->ping / 20), true);
				}

				if (CE.PosPrediction) {
					TagPos = VelocityPrediction(&Entity[CE.AimbotTarget], TagPos, CE.PosPredictionScale); // 0.0 - 0.75
				}

				Vector3 ViewOrigin;
				if (!CG_GetPlayerViewOrigin(0, &cg->ps, &ViewOrigin))
					return;

				Vector3 FinalAngle = vectorToAngles(TagPos - ViewOrigin) - ClientActive_t->baseAngle;
				float OldAngle = SHORT2ANGLE(old->viewAngles[1]);
				old->viewAngles[0] = ANGLE2SHORT(FinalAngle.x);
				old->viewAngles[1] = ANGLE2SHORT(FinalAngle.y);
				FixMovement(old, SHORT2ANGLE(old->viewAngles[1]), OldAngle, (float)old->fDir, (float)old->rDir);
			}

			else bFirstShot = true;

			if (CE.NoSpread && (Entity[cg->ClientNumber].nextState.Weapon != 0))
				NoSpread(old);
		}
	}
}

void DoSilentAimbot(usercmd_s* cur, usercmd_s* old, usercmd_s* New)
{
	if ((GetAsyncKeyState(XINPUT_GAMEPAD_LEFT_SHOULDER) && (CE.AimKey == LB)) || (GetAsyncKeyState(XINPUT_GAMEPAD_RIGHT_SHOULDER) && CE.AimKey == RB) || (GetAsyncKeyState(KEY_LT) && CE.AimKey == LT) || (GetAsyncKeyState(KEY_RT) && CE.AimKey == RT) || (CE.AimKey == NoAimKey))
		SetSilentAngles(cur, old, New);
}

int PreviousTarget;
void SetAimbotTarget()
{
	if (Dvar_GetBool("cl_ingame") && GetStructs() && (cg->ps.clientNum == cg->ClientNumber) && (cg->ps.Health > 0))
	{
		CE.AimbotTarget = GetMostViableClient();

		if ((CE.AimbotTarget != -1) && (PreviousTarget != CE.AimbotTarget) && CE.GamertagStealer)
		{
			SetName(cg->clientinfo[CE.AimbotTarget].Name);
			PreviousTarget = CE.AimbotTarget;
		}
	}
	else
		PreviousTarget = -1;
}