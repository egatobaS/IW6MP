#include "stdafx.h"
#include "Radar.h"
#include "Drawing.h"
#include "Functions.h"
#include "Menu.h"
#include "ESP.h"

float RadarSize = 130;
//Vector2 RadarCenter = { RadarSize + 15, RadarSize + 9 };
Vector2 RadarCenter = { RadarSize + 15, RadarSize + 9 };
float Red[4] = { 1, 0, 0, 1 };
float Green[4] = { 0, 1, 0, 1 };

bool CG_WorldPosToCompass(int compassType, cg_s *cgameGlob, rectDef_s *mapRect, const float *north, const float *playerWorldPos, const float *in, float *out, float *outClipped)
{
	bool result;
	if (compassType == 0)
	{
		float compassMaxRange = Dvar_GetFloat("compassMaxRange");
		compassMaxRange *= 1.5;
		float scale = mapRect->h / compassMaxRange;
		outClipped[0] = ((north[1] * ((in[0] - playerWorldPos[0]) * scale)) - (north[0] * ((in[1] - playerWorldPos[1]) * scale)));
		outClipped[1] = -((north[0] * ((in[0] - playerWorldPos[0]) * scale)) - -(north[1] * ((in[1] - playerWorldPos[1]) * scale)));
	}

	if (out)
	{
		out[0] = outClipped[0];
		out[1] = outClipped[1];
	}

	if (outClipped && (mapRect->w > 0.0f) && (mapRect->h > 0.0f))
	{
		if (outClipped[0] <= (mapRect->w * 0.5f))
		{
			if (outClipped[0] <= -(mapRect->w * 0.5f))
			{
				float scale = (-(mapRect->w * 0.5f) / outClipped[0]);
				outClipped[0] = (scale * outClipped[0]);
				outClipped[1] = (scale * outClipped[1]);
				result = true;
			}
		}
		else
		{
			float scale = ((mapRect->w * 0.5) / outClipped[0]);
			outClipped[0] = (scale * outClipped[0]);
			outClipped[1] = (scale * outClipped[1]);
			result = true;
		}

		if (outClipped[1] <= (mapRect->h * 0.5f))
		{
			if (outClipped[1] <= -(mapRect->h * 0.5f))
			{
				float scale = (-(mapRect->h * 0.5f) / outClipped[1]);
				outClipped[0] = (scale * outClipped[0]);
				outClipped[1] = (scale * outClipped[1]);
				result = true;
			}
		}
		else
		{
			float scale = ((mapRect->h * 0.5) / outClipped[1]);
			outClipped[0] = (scale * outClipped[0]);
			outClipped[1] = (scale * outClipped[1]);
			result = true;
		}
	}

	return result;
}

void SetRadarColor(int Client, float &R, float &G, float &B, float &A)
{
	A = 0;
	if (!IsEnemy(Client) && CE.RadarSettings.DrawFriendies)
	{
		R = CE.FriendlyColor.R; G = CE.FriendlyColor.G; B = CE.FriendlyColor.B; A = CE.FriendlyColor.A;
	}
	else if (CE.RadarSettings.DrawEnemies)
	{
		if (ClientOptions[Client].Ignored)
		{
			R = CE.FriendlyColor.R; G = CE.FriendlyColor.G; B = CE.FriendlyColor.B; A = CE.FriendlyColor.A;
		}
		else
		{
			if (Client == CE.AimbotTarget && CE.ShowAimbotTarget) {
				R = CE.AimbotTargetColor.R; G = CE.AimbotTargetColor.G; B = CE.AimbotTargetColor.B; A = CE.AimbotTargetColor.A;
			}
			else
			{
				if (ClientOptions[Client].Prioritized)
				{
					R = CE.PriorityEnemyColor.R; G = CE.PriorityEnemyColor.G; B = CE.PriorityEnemyColor.B;  A = CE.PriorityEnemyColor.A;
				}
				else
				{
					R = CE.EnemyColor.R; G = CE.EnemyColor.G; B = CE.EnemyColor.B;  A = CE.EnemyColor.A;
					if (CE.IsKillable[Client]) {
						R = CE.KillableColor.R; G = CE.KillableColor.G; B = CE.KillableColor.B;  A = CE.KillableColor.A;
					}
					if (CE.CanSeePlayer[Client]) {
						R = CE.VisibleColor.R; G = CE.VisibleColor.G; B = CE.VisibleColor.B;  A = CE.VisibleColor.A;
					}
				}
			}
		}
	}
}

void DrawOnRadar(const char* Material, float w, float h, float R, float G, float B, float A, bool DrawAngles, int EntityIndex)
{
	float Colour[4] = { R, G, B, A };

	rectDef_s rect = { RadarCenter.x - (RadarSize * 2 / 2), RadarCenter.y - (RadarSize * 2 / 2), RadarSize * 2, RadarSize * 2, 0, 0 };

	float centerX = (float)(rect.w * 0.5f) + rect.x;
	float centerY = (float)(rect.h * 0.5f) + rect.y;

	float north[2];
	((void(*)(...))addr->YawVectors2D)(cg->refdefViewAngles.y, north, 0);//YawVectors2D

	Vector3 LocationClipped;

	CG_WorldPosToCompass(0, cg, &rect, north, cg->refdef.ViewOrigin, Entity[EntityIndex].pose.Origin, 0, LocationClipped); //CG_WorldPosToCompass

	LocationClipped.x += centerX;
	LocationClipped.y += centerY;

	R_AddCmdDrawStretchPicRotateXY(LocationClipped.x - (w / 2), LocationClipped.y - (h / 2), w, h, 0, 0, 0, 0, DrawAngles ? (cg->refdefViewAngles.y - Entity[EntityIndex].prevState.apos.trBase.y) : 0, Colour, Material);
}

void DrawOnRadar(const char* Material, float w, float h, float R, float G, float B, float A, float Yaw, Vector3 Location)
{
	float Colour[4] = { R, G, B, A };

	rectDef_s rect = { RadarCenter.x - (RadarSize * 2 / 2), RadarCenter.y - (RadarSize * 2 / 2), RadarSize * 2, RadarSize * 2, 0, 0 };

	float centerX = (float)(rect.w * 0.5f) + rect.x;
	float centerY = (float)(rect.h * 0.5f) + rect.y;

	float north[2];
	((void(*)(...))addr->YawVectors2D)(cg->refdefViewAngles.y, north, 0);//YawVectors2D 

	Vector3 LocationClipped;
	CG_WorldPosToCompass(0, cg, &rect, north, cg->refdef.ViewOrigin, Location, 0, LocationClipped); //CG_WorldPosToCompass

	LocationClipped.x += centerX;
	LocationClipped.y += centerY;

	R_AddCmdDrawStretchPicRotateXY(LocationClipped.x - (w / 2), LocationClipped.y - (h / 2), w, h, 0, 0, 0, 0, Yaw ? (cg->refdefViewAngles.y - Yaw) : 0, Colour, Material);
}

void DrawOnRadarYaw(const char* Material, float w, float h, float R, float G, float B, float A, float Yaw, Vector3 Location)
{
	float Colour[4] = { R, G, B, A };

	rectDef_s rect = { RadarCenter.x - (RadarSize * 2 / 2), RadarCenter.y - (RadarSize * 2 / 2), RadarSize * 2, RadarSize * 2, 0, 0 };

	float centerX = (float)(rect.w * 0.5f) + rect.x;
	float centerY = (float)(rect.h * 0.5f) + rect.y;

	float north[2];
	((void(*)(...))addr->YawVectors2D)(cg->refdefViewAngles.y, north, 0);//YawVectors2D 

	Vector3 LocationClipped;
	CG_WorldPosToCompass(0, cg, &rect, north, cg->refdef.ViewOrigin, Location, 0, LocationClipped); //CG_WorldPosToCompass

	LocationClipped.x += centerX;
	LocationClipped.y += centerY;

	R_AddCmdDrawStretchPicRotateXY(LocationClipped.x - (w / 2), LocationClipped.y - (h / 2), w, h, 0, 0, 0, 0, Yaw, Colour, Material);
}

void DrawOnRadarYaw(Material* material, float w, float h, float R, float G, float B, float A, float Yaw, Vector3 Location)
{
	float Colour[4] = { R, G, B, A };

	rectDef_s rect = { RadarCenter.x - (RadarSize * 2 / 2), RadarCenter.y - (RadarSize * 2 / 2), RadarSize * 2, RadarSize * 2, 0, 0 };

	float centerX = (float)(rect.w * 0.5f) + rect.x;
	float centerY = (float)(rect.h * 0.5f) + rect.y;

	float north[2];
	((void(*)(...))addr->YawVectors2D)(cg->refdefViewAngles.y, north, 0);//YawVectors2D 

	Vector3 LocationClipped;
	CG_WorldPosToCompass(0, cg, &rect, north, cg->refdef.ViewOrigin, Location, 0, LocationClipped); //CG_WorldPosToCompass

	LocationClipped.x += centerX;
	LocationClipped.y += centerY;

	CG_DrawRotatedPicPhysical(addr->ScreenPlacement, LocationClipped.x - (w / 2), LocationClipped.y - (h / 2), w, h, Yaw, Colour, material);
}

void DrawOnRadar(const char* Text, Vector3 Location)
{
	rectDef_s rect = { RadarCenter.x - (RadarSize * 2 / 2), RadarCenter.y - (RadarSize * 2 / 2), RadarSize * 2, RadarSize * 2, 0, 0 };

	float centerX = (float)(rect.w * 0.5f) + rect.x;
	float centerY = (float)(rect.h * 0.5f) + rect.y;

	float north[2];
	((void(*)(...))addr->YawVectors2D)(cg->refdefViewAngles.y, north, 0);//YawVectors2D

	Vector3 LocationClipped;
	CG_WorldPosToCompass(0, cg, &rect, north, cg->refdef.ViewOrigin, Location, 0, LocationClipped); //CG_WorldPosToCompass

	LocationClipped.x += centerX;
	LocationClipped.y += centerY;

	//R_AddCmdDrawStretchPicRotateXY(LocationClipped.x - (w / 2), LocationClipped.y - (h / 2), w, h, 0, 0, 0, 0, Yaw ? (cg->refdefViewAngles.y - Yaw) : 0, Colour, Material);
	SetTextCentered(Text, "normalFont", LocationClipped.x, LocationClipped.y, 0.8, 0.8, 1, 1, 1, 1);
}

void DrawMapOnRadar()
{
	float Colour[4] = { 1, 1, 1, 0.8 };

	float f5 = (((cg->refdef.ViewOrigin.x - cg->compassMapUpperLeft[0]) * cg->compassNorth[1]) + ((cg->refdef.ViewOrigin.y - cg->compassMapUpperLeft[1]) * -cg->compassNorth[0])) / cg->compassMapWorldSize[0];
	float f6 = (((cg->refdef.ViewOrigin.x - cg->compassMapUpperLeft[0]) * -cg->compassNorth[0]) + ((cg->refdef.ViewOrigin.y - cg->compassMapUpperLeft[1]) * -cg->compassNorth[1])) / cg->compassMapWorldSize[1];

	float f7, f8;
	float compassMaxRange = Dvar_GetFloat("compassMaxRange");

	compassMaxRange *= 1.5;

	if (cg->compassMapWorldSize[0] < cg->compassMapWorldSize[1])
	{
		f7 = (compassMaxRange / cg->compassMapWorldSize[1]) * (float)0.5f;
		f8 = (cg->compassMapWorldSize[1] / cg->compassMapWorldSize[0]) * f7;
	}
	else
	{
		f7 = (compassMaxRange / cg->compassMapWorldSize[0]) * (float)0.5f;
		f8 = (cg->compassMapWorldSize[0] / cg->compassMapWorldSize[1]) * f7;
	}

	CL_DrawStretchPicRotatedST(addr->_0x82732100,
		RadarCenter.x - (RadarSize * 2 / 2),
		RadarCenter.y - (RadarSize * 2 / 2),
		RadarSize * 2,
		RadarSize * 2,
		0,
		0,
		f5,
		f6,
		f7,
		f8,
		0.0f,
		1.0f,
		0.0f,
		1.0f,
		-(cg->refdefViewAngles.y - cg->compassNorthYaw),
		Colour,
		cg->compassMapMaterial);
}

void DrawRadar()
{
	if (CE.ExternalRadar)
	{
		RadarCenter.x = (((RadarSize + 15) + 60) - CE.MenuSafeArea[0]);
		RadarCenter.y = (((RadarSize + 9) + 60) - CE.MenuSafeArea[1]);

		DrawBox("white", RadarCenter.x - (RadarSize * 2 / 2), RadarCenter.y - (RadarSize * 2 / 2), RadarSize * 2, RadarSize * 2, 2, 0, 0, 0, 0.4, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, 0.43);
		DrawMapOnRadar();

		float Fov = Dvar_GetFloat("cg_fov");
		float fAdsZoomFov = BG_GetWeaponVariantDef(cg->ps.Weapon)->fAdsZoomFov;
		DrawLine(RadarCenter.x - (((ClientActive_t->flZoomProgress ? fAdsZoomFov : Fov) / 2) * 2), RadarCenter.y - RadarSize * 2 / 2, RadarCenter.x, RadarCenter.y, 1, 1, 1, 1, 0.7);
		DrawLine(RadarCenter.x + (((ClientActive_t->flZoomProgress ? fAdsZoomFov : Fov) / 2) * 2), RadarCenter.y - RadarSize * 2 / 2, RadarCenter.x, RadarCenter.y, 1, 1, 1, 1, 0.7);

		SetShader("white", RadarCenter.x - RadarSize, RadarCenter.y, RadarSize * 2, 1, 1, 1, 1, 0.7); // horz
		SetShader("white", RadarCenter.x, RadarCenter.y - RadarSize * 2 / 2, 1, RadarSize * 2, 1, 1, 1, 0.7); // vert
	}
}

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

CompassActor* CG_CompassGetActor(int localclientnum, int Index)
{
	return (CompassActor*)(addr->_0x82AC1188 + (localclientnum * addr->_0x00000D10) + (Index * addr->_0x0000004C));
}

bool CL_ClientIsInMyParty(int localclientnum, int Client)
{
	bool(*CL_ClientIsInMyParty)(int localclientnum, int Client) = (bool(*)(int, int))addr->CL_ClientIsInMyParty;
	return CL_ClientIsInMyParty(localclientnum, Client);
}

void CalcCompassFriendlySize(int CompassType, float* W, float* H)
{
	void(*CalcCompassFriendlySize)(int CompassType, float* W, float* H) = (void(*)(int, float*, float*))addr->CalcCompassFriendlySize;
	CalcCompassFriendlySize(CompassType, W, H);
}

void CG_CompassDrawFriendlies()
{
	for (int i = 0; i < 36; i++)
	{
		CompassActor* compassActor = CG_CompassGetActor(0, i);
		const char* mat;

		if (!compassActor)
			continue;

		if (compassActor->lastUpdate > cg->time)
			compassActor->lastUpdate = 0;

		if (compassActor->lastUpdate <= (cg->time + -0x1F4))
			continue;

		if (cg->ps.clientNum == i)
			continue;

		//if (!(Entity[i].pose.eType & 1))
		//	continue;

		if ((compassActor->flags & 1))
			continue;

		float W, H;
		CalcCompassFriendlySize(0, &W, &H);

		W *= 2.0;
		H *= 2.0;

		if (Entity[i].nextState.Type == ET_AGENT)
		{
			if (compassActor->beginFadeTime && ((compassActor->beginFadeTime + (Dvar_GetInt("compassSoundPingFadeTime") * 1000)) >= cg->time))
				DrawOnRadarYaw("cb_compassping_friendlyfiring_party_mp", W, H, 1, 1, 1, 1, cg->refdefViewAngles.y - compassActor->lastYaw, (Vector3)compassActor->lastPos);
			else
				DrawOnRadarYaw("cb_compassping_friendly_party_mp", W, H, 1, 1, 1, 1, cg->refdefViewAngles.y - compassActor->lastYaw, (Vector3)compassActor->lastPos);
		}
		else
		{
			if (CL_ClientIsInMyParty(0, i))
			{
				if (compassActor->perks[1] & addr->_0x00040000)
					DrawOnRadarYaw((Material*)(addr->_0x82ACCD00 + 0x3BC), W, H, 1, 1, 1, 1, cg->refdefViewAngles.y - compassActor->lastYaw, (Vector3)compassActor->lastPos);
				else
				{
					if (compassActor->beginFadeTime && ((compassActor->beginFadeTime + (Dvar_GetInt("compassSoundPingFadeTime") * 1000)) >= cg->time))
						DrawOnRadarYaw("cb_compassping_friendlyfiring_party_mp", W, H, 1, 1, 1, 1, cg->refdefViewAngles.y - compassActor->lastYaw, (Vector3)compassActor->lastPos);
					else if (compassActor->beginVoiceFadeTime && ((compassActor->beginVoiceFadeTime + (Dvar_GetInt("compassSoundPingFadeTime") * 1000)) >= cg->time))
						DrawOnRadarYaw("cb_compassping_friendlyyelling_party_mp", W, H, 1, 1, 1, 1, cg->refdefViewAngles.y - compassActor->lastYaw, (Vector3)compassActor->lastPos);
					else
						DrawOnRadarYaw("cb_compassping_friendly_party_mp", W, H, 1, 1, 1, 1, cg->refdefViewAngles.y - compassActor->lastYaw, (Vector3)compassActor->lastPos);
				}
			}
			else
			{
				if (compassActor->perks[1] & addr->_0x00040000)
					DrawOnRadarYaw((Material*)(addr->_0x82ACCD00 + addr->_0x000003BC), W, H, 1, 1, 1, 1, cg->refdefViewAngles.y - compassActor->lastYaw, (Vector3)compassActor->lastPos);
				else
				{
					if (compassActor->beginFadeTime && ((compassActor->beginFadeTime + (Dvar_GetInt("compassSoundPingFadeTime") * 1000)) >= cg->time))
						DrawOnRadarYaw("cb_compassping_friendlyfiring_mp", W, H, 1, 1, 1, 1, cg->refdefViewAngles.y - compassActor->lastYaw, (Vector3)compassActor->lastPos);
					else if (compassActor->beginVoiceFadeTime && ((compassActor->beginVoiceFadeTime + (Dvar_GetInt("compassSoundPingFadeTime") * 1000)) >= cg->time))
						DrawOnRadarYaw("cb_compassping_friendlyyelling_mp", W, H, 1, 1, 1, 1, cg->refdefViewAngles.y - compassActor->lastYaw, (Vector3)compassActor->lastPos);
					else
						DrawOnRadarYaw("cb_compassping_friendly_mp", W, H, 1, 1, 1, 1, cg->refdefViewAngles.y - compassActor->lastYaw, (Vector3)compassActor->lastPos);
				}
			}
		}
	}
}

void CG_CompassDrawEnemies()
{
	for (int i = 0; i < 36; i++)
	{
		CompassActor* compassActor = CG_CompassGetActor(0, i);

		if (!compassActor)
			continue;

		if (compassActor->lastUpdate > cg->time)
			compassActor->lastUpdate = 0;

		if (compassActor->lastUpdate < (cg->time + -0x1F4))
			continue;

		if (cg->ps.clientNum == i)
			continue;

		if (!(Entity[i].pose.eType & 1))
			continue;

		if (!(compassActor->flags & 1))
			continue;

		float W, H;
		CalcCompassFriendlySize(0, &W, &H);

		W *= 2.0;
		H *= 2.0;

		if (compassActor->perks[1] & addr->_0x00040000)
			DrawOnRadarYaw((Material*)(0x82ACCD00 + addr->_0x000003B8), W, H, 1, 1, 1, 1, cg->refdefViewAngles.y - compassActor->lastYaw, (Vector3)compassActor->lastPos);
		else
			DrawOnRadarYaw("cb_compassping_enemyfacingdirection", W, H, 1, 1, 1, 1, cg->refdefViewAngles.y - compassActor->lastYaw, (Vector3)compassActor->lastPos);
	}
}

void CG_CompassDrawPlayer()
{
	float W, H;
	CalcCompassFriendlySize(0, &W, &H);

	W *= 2.0;
	H *= 2.0;

	DrawOnRadar("compassping_player", W, H, 1, 1, 1, 1, false, cg->ClientNumber);
}

void DrawWeaponsOnRadar(int i)
{
	if ((Entity[i].nextState.Weapon > 0) && (Entity[i].pose.eType & 1) && CE.ExternalRadar)
	{
		WeaponDef* weaponDef = BG_GetWeaponDef(Entity[i].nextState.Weapon);
		Material* shader = GetWeaponShader(Entity[i].nextState.Weapon);
		WeaponVariantDef* WeaponDef = BG_GetWeaponVariantDef(Entity[i].nextState.Weapon);

		if ((shader != NULL) && CE.ESPSettings.DrawWeapons)
		{
			if (weaponDef->killIconRatio == WEAPON_ICON_RATIO_1TO1)
				DrawOnRadar(shader->Name, 17, 17, 1, 1, 1, 1, false, i);
			else if (weaponDef->killIconRatio == WEAPON_ICON_RATIO_2TO1)
				DrawOnRadar(shader->Name, (17 * 2.0), 17, 1, 1, 1, 1, false, i);
			else
				DrawOnRadar(shader->Name, (17 * 2.0), (17 * 0.5), 1, 1, 1, 1, false, i);
		}
		else if (CE.ESPSettings.DrawScavengerPacks && cuser_strcmp(GetWeaponName(Entity[i].nextState.Weapon), "Scavenger") && BG_HasPerk("specialty_scavenger"))
			DrawOnRadar("icon_perks_scavenger", 25, 20, 1, 1, 1, 1, false, i); //scavenger_pickup  specialty_scavenger  
	}
}

void DrawExplosivesOnRadar(int i)
{
	if (Entity[i].nextState.Type == ET_SCRIPTMOVER)
	{
		if ((Entity[i].nextState.Weapon > 0) && (Entity[i].pose.eType & 0x6) && CE.ExternalRadar)
		{
			WeaponDef* weaponDef = BG_GetWeaponDef(Entity[i].nextState.Weapon);
			Material* shader = GetWeaponShader(Entity[i].nextState.Weapon);
			WeaponVariantDef* WeaponDef = BG_GetWeaponVariantDef(Entity[i].nextState.Weapon);

			if (shader != NULL)
			{
				if (weaponDef->killIconRatio == WEAPON_ICON_RATIO_1TO1)
					DrawOnRadar(shader->Name, 17, 17, 1, 1, 1, 1, false, i);
				else if (weaponDef->killIconRatio == WEAPON_ICON_RATIO_2TO1)
					DrawOnRadar(shader->Name, (17 * 2.0), 17, 1, 1, 1, 1, false, i);
				else
					DrawOnRadar(shader->Name, (17 * 2.0), (17 * 0.5), 1, 1, 1, 1, false, i);
			}
		}
	}
	else if (Entity[i].nextState.Type == ET_MISSILE)
	{
		if (((Entity[i].nextState.Weapon > 0) && (Entity[i].pose.eType & 4) || (Entity[i].pose.eType == 4)) && CE.ExternalRadar)
		{
			WeaponDef* weaponDef = BG_GetWeaponDef(Entity[i].nextState.Weapon);
			Material* shader = GetWeaponShader(Entity[i].nextState.Weapon);
			WeaponVariantDef* WeaponDef = BG_GetWeaponVariantDef(Entity[i].nextState.Weapon);

			if (shader != NULL)
			{
				if (cuser_strcmp(GetWeaponName(Entity[i].nextState.Weapon), "Smoke Grenade"))
					DrawOnRadar("hud_us_smokegrenade", 17, 17, 1, 1, 1, 1, false, i);
				else
				{
					if (weaponDef->killIconRatio == WEAPON_ICON_RATIO_1TO1)
						DrawOnRadar(shader->Name, 17, 17, 1, 1, 1, 1, false, i);
					else if (weaponDef->killIconRatio == WEAPON_ICON_RATIO_2TO1)
						DrawOnRadar(shader->Name, (17 * 2.0), 17, 1, 1, 1, 1, false, i);
					else
						DrawOnRadar(shader->Name, (17 * 2.0), (17 * 0.5), 1, 1, 1, 1, false, i);
				}
			}
			else if (cuser_strcmp(GetWeaponName(Entity[i].nextState.Weapon), "Flash Grenade"))
				DrawOnRadar("hud_us_flashgrenade", 17, 17, 1, 1, 1, 1, false, i);
		}
	}
}

struct CompassPlane
{
	int entityNum; //0x0
	int lastUpdate; //0x4
	float lastPos[3]; //0x8
	float lastyaw; //0x14
	int ownerNum; //0x18
	Material *compassFriendlyIcon; //0x1C
	Material *compassEnemyIcon; //0x20
	Material *cb_compassFriendlyIcon; //0x24
	Material *cb_compassEnemyIcon; //0x28
}; //0x2C

CompassPlane* CG_GetCompassPlane(int localclient, int index)
{
	return (CompassPlane*)(addr->_0x82AC0EC8 + (localclient *  addr->_0x00000160) + (index *  addr->_0x0000002C));
}

void CG_CompassDrawPlanes()
{
	for (int i = 0; i < 8; i++)
	{
		CompassPlane* CompassPlane = CG_GetCompassPlane(0, i);

		if (!CompassPlane)
			continue;

		centity_s* Plane = (centity_s*)&Entity[CompassPlane->entityNum];

		if (!Plane)
			continue;

		if (Plane->nextState.Type != ET_PLANE)
			continue;

		if (CompassPlane->lastUpdate > cg->time)
			CompassPlane->lastUpdate = 0;

		if (CompassPlane->lastUpdate < (cg->time - 0x1F4))
			continue;

		DrawOnRadarYaw((IsEnemy(CompassPlane->ownerNum) ? CompassPlane->compassEnemyIcon->Name : CompassPlane->compassFriendlyIcon->Name), (Dvar_GetFloat("compassSize") * 20.0f), (Dvar_GetFloat("compassSize") * 20.0f), 1, 1, 1, 1, cg->refdefViewAngles.y - CompassPlane->lastyaw, (Vector3)CompassPlane->lastPos);
	}
}

struct CompassMissile
{
	int entityNum; //0x00
	int lastUpdateTime; //0x04
	float pos[3]; //0x08
	float yaw; //0x14
	int ownerNum; //0x18
}; //0x1C

CompassMissile* CG_GetCompassMissile(int localclient, int index)
{
	return (CompassMissile*)(addr->_0x82AC0008 + (localclient *  addr->_0x000000E0) + (index *  addr->_0x0000001C));
}

void CG_CompassDrawMissiles()
{
	for (int i = 0; i < 8; i++)
	{
		CompassMissile* CompassMissile = CG_GetCompassMissile(0, i);

		if (!CompassMissile)
			continue;

		centity_s* Missile = (centity_s*)&Entity[CompassMissile->entityNum];

		if (!Missile)
			continue;

		if (Missile->nextState.Type != ET_MISSILE)
			continue;

		if (CompassMissile->lastUpdateTime > cg->time)
			CompassMissile->lastUpdateTime = 0;

		if (CompassMissile->lastUpdateTime < (cg->time - 0x1F4))
			continue;

		DrawOnRadarYaw((IsEnemy(CompassMissile->ownerNum) ? "cb_remotemissile_target_hostile" : "cb_remotemissile_target_friendly"), (Dvar_GetFloat("compassSize") * 50.0f), (Dvar_GetFloat("compassSize") * 50.0f), 1, 1, 1, 1, cg->refdefViewAngles.y - CompassMissile->yaw, (Vector3)CompassMissile->pos);
	}
}

enum VehicleEngineState
{
	VEH_ENGINE_STATE_OFF = 0x0,
	VEH_ENGINE_STATE_STARTUP = 0x1,
	VEH_ENGINE_STATE_IDLE = 0x2,
	VEH_ENGINE_STATE_RAMPUP = 0x3,
	VEH_ENGINE_STATE_SUSTAIN = 0x4,
	VEH_ENGINE_STATE_RAMPDOWN = 0x5,
};

struct VehicleAudioState
{
	float entOrigin[3]; //0x00
	bool throttle; //0x0C
	int stateTime;//0x10
	int spawnTime; //0x14
	int spawnTimeDelay; //0x18
	float spawnVolumeScale; //0x1C
	float inAirPitch; //0x20
	VehicleEngineState engineState; //0x24
	int engineRampUpTime; //0x28
	bool engineRampUpDelay; //0x2C
	float engineRampUpDelayFrac; //0x30
	int engineRampDownTime; //0x34
	bool turretMoving; //0x38
	int lastImpactTime; //0x3C
	float lastImpactScale; //0x40
	char surfaceType; //0x44
}; //0x48

struct VehicleClient
{
	int index; //0x00
	int defIndex; //0x04
	int entIndex; //0x08
	int localClientNum; //0x0C
	int frameTime; //0x10
	int time; //0x14
	int fireBarrel; //0x18
	unsigned int flags; //0x1C
	float localSpeed[3]; //0x20
	float rotateSpeed[3]; //0x2C
	float materialTime; //0x38
	int playerIndex; //0x3C
	VehicleAudioState audioState; //0x40
}; //0x88

enum VehicleType
{
	VEH_WHEELS_4 = 0x0,
	VEH_TANK = 0x1,
	VEH_PLANE = 0x2,
	VEH_BOAT = 0x3,
	VEH_ARTILLERY = 0x4,
	VEH_HELICOPTER = 0x5,
	VEH_SNOWMOBILE = 0x6,
	VEH_SUBMARINE = 0x7,
	VEH_UGV = 0x8,
	VEH_TYPE_COUNT = 0x9,
};

struct VehicleDef
{
	const char *name; //0x0
	VehicleType type; //0x04
	const char *useHintString; //0x08
	int health; //0xC
	char _0x0010[0x380]; //0x10
	Material *compassFriendlyIcon; //0x390
	Material *compassEnemyIcon; //0x394
	Material *cb_compassFriendlyIcon; //0x398
	Material *cb_compassEnemyIcon; //0x39C
	int compassIconWidth; //0x3A0
	int compassIconHeight; //0x3A4
};

VehicleClient* VehicleCl_GetClientFromIndex(int localclient, int index)
{
	return (VehicleClient*)(addr->_0x840F07D0 + (((localclient << 3) + index) *  addr->_0x00000088));
}

bool VehicleCl_IsClientValid(int localclient, VehicleClient* VC)
{
	bool(*VehicleCl_IsClientValid)(int localclient, VehicleClient* VC) = (bool(*)(int, VehicleClient*))addr->VehicleCl_IsClientValid;
	return VehicleCl_IsClientValid(localclient, VC);
}

VehicleDef* VehicleCl_GetClientDef(VehicleClient* VC)
{
	VehicleDef*(*VehicleCl_GetClientDef)(VehicleClient* VC) = (VehicleDef*(*)(VehicleClient*))addr->VehicleCl_GetClientDef;
	return VehicleCl_GetClientDef(VC);
}

void VehicleHud_DrawCompass()
{
	for (int i = 0; i < 8; i++)
	{
		VehicleClient* VC = VehicleCl_GetClientFromIndex(0, i);

		if (!VehicleCl_IsClientValid(0, VC))
			continue;

		VehicleDef* VehDef = VehicleCl_GetClientDef(VC);

		if (!VehDef)
			continue;

		centity_s* Vehicle = (centity_s*)&Entity[VC->entIndex];

		if (!Vehicle)
			continue;

		if (!VehDef->compassFriendlyIcon || !VehDef->compassEnemyIcon)
			continue;

		DrawOnRadarYaw((IsEnemy(VC->playerIndex) ? VehDef->compassEnemyIcon->Name : VehDef->compassFriendlyIcon->Name), (VehDef->compassIconWidth * 2), (VehDef->compassIconHeight * 2), 1, 1, 1, 1, cg->refdefViewAngles.y - Vehicle->pose.Angles.y, (Vector3)Vehicle->pose.Origin);
	}
}

struct CompassTurret
{
	int entityNum; //0x00
	int lastUpdateTime; //0x04
	int firePingTime; //0x08
	int radarPingTime; //0x0C
	float positionCurrent[3]; //0x10
	float positionLastKnown[3]; //0x1C
	float yawCurrent; //0x28
	float yawLastKnown; //0x2C
	bool isFriendly; //0x30
}; //0x34

CompassTurret* CG_GetCompassTurret(int localclient, int index)
{
	return (CompassTurret*)(((localclient *  addr->_0x00000680) + addr->_0x82AC01C8) + (index *  addr->_0x00000034));
}

void CG_CompassDrawTurrets()
{
	for (int i = 0; i < 32; i++)
	{
		CompassTurret* CompassTurret = CG_GetCompassTurret(0, i);

		if (!CompassTurret)
			continue;

		centity_s* Turret = (centity_s*)&Entity[CompassTurret->entityNum];

		if (!Turret)
			continue;

		if (CompassTurret->lastUpdateTime > cg->time)
			CompassTurret->lastUpdateTime = 0;

		if (CompassTurret->lastUpdateTime < (cg->time - 0x1F4))
			continue;

		DrawOnRadarYaw((CompassTurret->firePingTime >= cg->time) ? ((CompassTurret->isFriendly) ? "cb_compassping_sentry_friendlyfiring" : "cb_compassping_sentry_enemyfiring") : ((CompassTurret->isFriendly) ? "cb_compassping_sentry_friendly" : "cb_compassping_sentry_enemy"), 28.0f, 25.0f, 1, 1, 1, 1, cg->refdefViewAngles.y - CompassTurret->yawCurrent, (Vector3)CompassTurret->positionCurrent);
	}
}

void DrawObjective(objective_t* objective)
{
	if (objective->state == OBJST_CURRENT || objective->state == OBJST_ACTIVE)
	{
		float w, h;
		((void(*)(...)) addr->_0x8225FB08)(0, &w, &h); //CalcCompassPointerSize

		w *= 1.5;
		h *= 1.5;

		Material* material = ((Material*(*)(...)) addr->_0x8228B8F0)(0, objective->icon, 0); //CG_ObjectiveIcon(int localClientNum, int icon, int type)

		if (!material)
			return;

		if (objective->entNum == 0x7FF)
			DrawOnRadar(material->Name, w, h, 1, 1, 1, 1, 0, objective->Origin);
		else
			DrawOnRadar(material->Name, w, h, 1, 1, 1, 1, true, objective->entNum);
	}
}

void CG_CompassDrawPlayerPointers_MP()
{
	for (int i = 0; i < 32; i++)
	{
		DrawObjective(&cg->ps.objective[i]);
	}
}