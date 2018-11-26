#include "stdafx.h"
#include "ESP.h"
#include "Radar.h"
#include "Lib.h"
#include "Structures.h"
#include "Drawing.h"
#include "Functions.h"
#include "Menu.h"

bool WorldToScreen(Vector3& WorldLocation, Vector2& Screen, bool on_screen)
{
	Vector3 Local = (WorldLocation - cg->refdef.ViewOrigin), Transform;
	Transform.x = DotProduct(Local, cg->refdef.ViewAxis[1]);
	Transform.y = DotProduct(Local, cg->refdef.ViewAxis[2]);
	Transform.z = DotProduct(Local, cg->refdef.ViewAxis[0]);

	if (Transform.z < 0.1f)
		return false;

	Screen.x = (cg->refdef.width / 2.0f) * (1 - (Transform.x / cg->refdef.Fov.x / Transform.z));
	Screen.y = (cg->refdef.height / 2.0f) * (1 - (Transform.y / cg->refdef.Fov.y / Transform.z));

	if ((Screen.x < 0 || Screen.x > cg->refdef.width || Screen.y < 0 || Screen.y > cg->refdef.height) && on_screen)
		return false;
	return true;
}
void MakeESPLine(Vector3 Center, float X1, float Y1, float Z1, float X2, float Y2, float Z2, float R, float G, float B, float A)
{
	Vector3 Origin, Origin2;
	Vector2 XY1, XY2;
	Origin.x = (Center.x + X1);
	Origin.y = (Center.y + Y1);
	Origin.z = (Center.z + Z1);
	Origin2.x = (Center.x + X2);
	Origin2.y = (Center.y + Y2);
	Origin2.z = (Center.z + Z2);
	if (WorldToScreen(Origin, XY1) && WorldToScreen(Origin2, XY2))
	{
		DrawLine(XY1.x, XY1.y, XY2.x, XY2.y, 1, R, G, B, A);
	}
}

void DrawCroshair()
{
	if (CE.Crosshair == Plus)
	{
		Dvar_SetBool("cg_drawcrosshair", false);
		float CenterX, CenterY;
		CenterX = (cg->refdef.width / 2); CenterY = (cg->refdef.height / 2);
		float Colour[] = { 1, 1, 1, 1 };

		SetShader("white", (CenterX - 15), (CenterY - 1), 30, 1, 1, 1, 1, 1);
		SetShader("white", (CenterX - 1), (CenterY - 15), 1, 30, 1, 1, 1, 1);
	}
	else if (CE.Crosshair == CrossCrosshair)
	{
		Dvar_SetBool("cg_drawcrosshair", false);
		float CenterX = (cg->refdef.width / 2); float CenterY = (cg->refdef.height / 2);
		DrawLine((CenterX - 15), (CenterY - 15), (CenterX - 4), (CenterY - 4), 1, 1, 1, 1, 1);//top left
		DrawLine((CenterX + 15), (CenterY + 15), (CenterX + 4), (CenterY + 4), 1, 1, 1, 1, 1);//bottom right
		DrawLine((CenterX + 15), (CenterY - 15), (CenterX + 4), (CenterY - 4), 1, 1, 1, 1, 1);//top right
		DrawLine((CenterX - 15), (CenterY + 15), (CenterX - 4), (CenterY + 4), 1, 1, 1, 1, 1);//bottom left
	}
	else if (CE.Crosshair == Spin)
	{
		Dvar_SetBool("cg_drawcrosshair", false);
		float CenterX, CenterY;
		CenterX = (cg->refdef.width / 2); CenterY = (cg->refdef.height / 2);
		float Colour[] = { 1, 1, 1, 1 };

		static float angle = 0.0f;
		angle += 1.0f;
		if (angle > 360.0f)
			angle = 0.0f;

		R_AddCmdDrawStretchPicRotateXY((CenterX - 30), (CenterY - 1), 60, 2, 1, 1, 1, 1, angle, Colour, "white");
		R_AddCmdDrawStretchPicRotateXY((CenterX - 1), (CenterY - 30), 2, 60, 1, 1, 1, 1, angle, Colour, "white");
	}
	else
		Dvar_SetBool("cg_drawcrosshair", true);
}

void DrawHealthBar()
{
	if ((cg->ps.MaxHealth != 0) && CE.HealthBar)
	{
		float CenterX, CenterY; float fMultiplier, HealthBarLength;
		fMultiplier = (200.0f / (float)cg->ps.MaxHealth);
		HealthBarLength = (cg->ps.Health * fMultiplier);

		CenterX = (cg->refdef.width / 2); CenterY = (((cg->refdef.height / 2) - 60) + CE.MenuSafeArea[1]);
		SetShader("white", (CenterX - 100), (CenterY + 300), 204, 20, 0, 0, 0, 0.2);
		SetShader("white", (CenterX - 98), (CenterY + 302), HealthBarLength, 16, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, 0.43);
		SetTextCentered(va("Health (%i/%i)", cg->ps.Health, cg->ps.MaxHealth), "normalfont", CenterX, (CenterY + 302 + TextHeight("normalfont", 0.5)), 0.5, 0.5, 1, 1, 1, 1);
	}
}

void SetESPColor(int Client, float &R, float &G, float &B, float &A, bool snapLine)
{
	A = 0;
	if (!IsEnemy(Client) && CE.ESPSettings.DrawFriendies)
	{
		R = CE.FriendlyColor.R; G = CE.FriendlyColor.G; B = CE.FriendlyColor.B; A = CE.FriendlyColor.A;
	}
	else if (CE.ESPSettings.DrawEnemies)
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
				if (CE.IsKillable[Client] && !CE.CanSeePlayer[Client]) {
					R = CE.KillableColor.R; G = CE.KillableColor.G; B = CE.KillableColor.B;  A = CE.KillableColor.A; return;
				}

				if (CE.CanSeePlayer[Client]) {
					R = CE.VisibleColor.R; G = CE.VisibleColor.G; B = CE.VisibleColor.B;  A = CE.VisibleColor.A; return;
				}

				if (ClientOptions[Client].Prioritized) {
					R = CE.PriorityEnemyColor.R; G = CE.PriorityEnemyColor.G; B = CE.PriorityEnemyColor.B;  A = CE.PriorityEnemyColor.A;
				}
				else {
					R = CE.EnemyColor.R; G = CE.EnemyColor.G; B = CE.EnemyColor.B;  A = CE.EnemyColor.A;
				}
			}
		}
	}
}

void DrawBoneLine(int entity, const char* tag1, const char* tag2, float R, float G, float B, float A)
{
	Vector3 Origin1, Origin2; Vector2 Loc1, Loc2;
	if (GetTagPos(entity, tag1, &Origin1) && GetTagPos(entity, tag2, &Origin2))
	{
		if (WorldToScreen(Origin1, Loc1) && WorldToScreen(Origin2, Loc2))
		{
			DrawLine(Loc1.x, Loc1.y, Loc2.x, Loc2.y, 1, R, G, B, A);
		}
	}
}

void DrawBoneLine(centity_s entity, const char* tag1, const char* tag2, float R, float G, float B, float A)
{
	Vector3 Origin1, Origin2; Vector2 Loc1, Loc2;
	if (GetTagPos(entity.nextState.clientNum, tag1, &Origin1) && GetTagPos(entity.nextState.clientNum, tag2, &Origin2))
	{
		if (WorldToScreen(Origin1, Loc1) && WorldToScreen(Origin2, Loc2))
		{
			DrawLine(Loc1.x, Loc1.y, Loc2.x, Loc2.y, 1, R, G, B, A);
		}
	}
}

bool GetDimentions(int client, float *Width, float *Height, Vector2* Center, bool on_screen)
{
	Vector3 OriginFeet = Entity[client].pose.Origin; Vector3 OriginHead;
	if (!GetTagPos(client, "j_head", &OriginHead))
		return false;
	OriginHead.z += 10;
	OriginFeet.z -= 5;
	Vector2 LocationLeft, LocationRight; Vector2 Location, LocationHead;
	if (WorldToScreen(OriginFeet, Location, on_screen) && WorldToScreen(OriginHead, LocationHead, on_screen))
	{
		*Height = (Location.y - LocationHead.y);
		*Center = Location;
		if (Entity[client].nextState.lerp.eFlags & FLAG_PRONE)
			*Width = *Height / 0.9;
		else
			*Width = *Height / 1.9;
		return true;
	}
	else
		return false;
}

void DrawSnapLine(int Client)
{
	if (CE.ESPSettings.SnapLines)
	{
		float R, G, B, A;
		SetESPColor(Client, R, G, B, A, true);
		float fHeight, fWidth; Vector2 Location;
		if (GetDimentions(Client, &fWidth, &fHeight, &Location, true))
		{
			float BeginX, BeginY;
			if (CE.ESPSettings.SnapLines == SnapTop) {
				BeginX = (cg->refdef.width / 2); BeginY = 0;
				Location.y -= fHeight;
			}
			if (CE.ESPSettings.SnapLines == SnapMiddle) {
				BeginX = (cg->refdef.width / 2); BeginY = (cg->refdef.height / 2);
			}
			else if (CE.ESPSettings.SnapLines == SnapBottom) {
				BeginX = (cg->refdef.width / 2); BeginY = cg->refdef.height;
			}
			DrawLine(Location.x, Location.y, BeginX, BeginY, 1, R, G, B, A);
		}
	}
}

void DrawESPBones(int Client)
{
	float R, G, B, A;
	R = CE.BonesColor.R; G = CE.BonesColor.G; B = CE.BonesColor.B; A = CE.BonesColor.A;
	if (CE.ESPSettings.ESPBones)
	{
		//neck
		DrawBoneLine(Entity[Client], "j_head", "j_neck", R, G, B, A);

		//left arm
		DrawBoneLine(Entity[Client], "j_neck", "j_shoulder_le", R, G, B, A);
		DrawBoneLine(Entity[Client], "j_shoulder_le", "j_elbow_le", R, G, B, A);
		DrawBoneLine(Entity[Client], "j_elbow_le", "j_wrist_le", R, G, B, A);

		//right arm
		DrawBoneLine(Entity[Client], "j_neck", "j_shoulder_ri", R, G, B, A);
		DrawBoneLine(Entity[Client], "j_shoulder_ri", "j_elbow_ri", R, G, B, A);
		DrawBoneLine(Entity[Client], "j_elbow_ri", "j_wrist_ri", R, G, B, A);

		//spine
		DrawBoneLine(Entity[Client], "j_neck", "j_mainroot", R, G, B, A);

		//left leg
		DrawBoneLine(Entity[Client], "j_mainroot", "j_hip_le", R, G, B, A);
		DrawBoneLine(Entity[Client], "j_hip_le", "j_knee_le", R, G, B, A);
		DrawBoneLine(Entity[Client], "j_knee_le", "j_ankle_le", R, G, B, A);

		//right leg
		DrawBoneLine(Entity[Client], "j_mainroot", "j_hip_ri", R, G, B, A);
		DrawBoneLine(Entity[Client], "j_hip_ri", "j_knee_ri", R, G, B, A);
		DrawBoneLine(Entity[Client], "j_knee_ri", "j_ankle_ri", R, G, B, A);
	}
}

float GetBoxHeight(centity_s ent) {
	Vector3 Head, Feet;
	if (GetTagPos(ent.nextState.clientNum, "j_helmet", &Head))
		return GetDistance(Head, ent.pose.Origin) * 55;
	else
		return 0;
}

int iEdgePairs[][2] = { { 0, 1 }, { 2, 3 }, { 0, 2 }, { 1, 3 }, { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 }, { 4, 5 }, { 6, 7 }, { 4, 6 }, { 5, 7 } };

void Draw3DBox(Vector3 Centre, Vector3 Dimensions, Vector3 Angles, float R, float G, float B, float A)
{
	Centre.z += (Dimensions.z / 2);

	Dimensions.x *= 0.5;
	Dimensions.y *= 0.5;
	Dimensions.z *= 0.5;


	Vector3 Nodes[] = {
		Vector3(-Dimensions.x, Dimensions.y, Dimensions.z), // A
		Vector3(Dimensions.x, Dimensions.y, Dimensions.z), // B
		Vector3(-Dimensions.x, Dimensions.y, -Dimensions.z), // C
		Vector3(Dimensions.x, Dimensions.y, -Dimensions.z), // D
		Vector3(-Dimensions.x, -Dimensions.y, Dimensions.z), // E
		Vector3(Dimensions.x, -Dimensions.y, Dimensions.z), // F
		Vector3(-Dimensions.x, -Dimensions.y, -Dimensions.z), // G
		Vector3(Dimensions.x, -Dimensions.y, -Dimensions.z), // H
	};

	float Yaw = DegToRad(Angles.y);
	float SinYaw = sin(Yaw);
	float CosYaw = cos(Yaw);

	float Roll = DegToRad(Angles.z);
	float SinRoll = sin(Roll);
	float CosRoll = cos(Roll);

	float Pitch = DegToRad(-Angles.x);
	float SinPitch = sin(Pitch);
	float CosPitch = cos(Pitch);

	for (int i = 0; i < 8; ++i)
	{
		Vector3 Node = Nodes[i];

		/*Rotate Roll*/
		Nodes[i].x = (Node.x * CosPitch) - (Node.z * SinPitch);
		Nodes[i].z = (Node.z * CosPitch) + (Node.x * SinPitch);

		Node = Nodes[i];

		/*Rotate Pitch*/
		Nodes[i].y = (Node.y * CosRoll) - (Node.z * SinRoll);
		Nodes[i].z = (Node.z * CosRoll) + (Node.y * SinRoll);

		Node = Nodes[i];

		/*Rotate Yaw*/
		Nodes[i].x = (Node.x * CosYaw) - (Node.y * SinYaw);
		Nodes[i].y = (Node.y * CosYaw) + (Node.x * SinYaw);
	}

	for (int i = 0; i < 12; ++i)
		MakeESPLine(Centre, Nodes[iEdgePairs[i][0]].x, Nodes[iEdgePairs[i][0]].y, Nodes[iEdgePairs[i][0]].z, Nodes[iEdgePairs[i][1]].x, Nodes[iEdgePairs[i][1]].y, Nodes[iEdgePairs[i][1]].z, R, G, B, A);
}

void DrawESPBoxAlt(int Client, bool IsDog, const char* Name, float R, float G, float B, float A)
{
	float fHeight, fWidth; Vector2 Location; int distance = GetDistance(Entity[cg->ClientNumber].pose.Origin, Entity[Client].pose.Origin);

	if (GetDimentions(Client, &fWidth, &fHeight, &Location))
	{
		if (CE.ESPType == ESP2D)
		{
			SetShader("white", (Location.x - (fWidth / 2)), (Location.y - fHeight), fWidth, 1, R, G, B, A);//top
			SetShader("white", (Location.x - (fWidth / 2)), Location.y, fWidth, 1, R, G, B, A);//bottom
			SetShader("white", (Location.x - (fWidth / 2)), (Location.y - fHeight), 1, fHeight, R, G, B, A);//left
			SetShader("white", (Location.x + (fWidth / 2)), (Location.y - fHeight), 1, fHeight, R, G, B, A);//right
		}
		if (CE.ESPType == ESP3D)
		{
			Vector3 Angles = Entity[Client].nextState.lerp.apos.trBase;
			Angles.x = 0;
			Angles.z = 0;

			Vector3 Center = Entity[Client].pose.Origin;

			float length = 30.0f, width = 30.0f;
			if (Entity[Client].nextState.lerp.eFlags & FLAG_PRONE)
			{
				length = 90.0f;
				width = 45.0f;

				Angles.y -= 20;

				Vector3 NewAngle = Angles;
				NewAngle.y -= 180;
				Center = AnglesToForward(Center, NewAngle, 10);

				NewAngle.y += 90;
				Center = AnglesToForward(Center, NewAngle, 5);
			}

			Vector3 Dimentions(length, width, GetBoxHeight(Entity[Client]));

			Draw3DBox(Center, Dimentions, Angles, R, G, B, A);
		}
		if (CE.ESPType == FB)
		{
			if (A > 0)
				SetShader("white", (Location.x - (fWidth / 2)), (Location.y - fHeight), fWidth, fHeight, R, G, B, 0.2f);
			SetShader("white", (Location.x - (fWidth / 2)), (Location.y - fHeight), fWidth, 1, 0, 0, 0, A);//top
			SetShader("white", (Location.x - (fWidth / 2)), Location.y, fWidth, 1, 0, 0, 0, A);//bottom
			SetShader("white", (Location.x - (fWidth / 2)), (Location.y - fHeight), 1, fHeight, 0, 0, 0, A);//left
			SetShader("white", (Location.x + (fWidth / 2)), (Location.y - fHeight), 1, fHeight, 0, 0, 0, A);//right
		}
		if (CE.ESPType == Corner)
		{
			SetShader("white", (Location.x - (fWidth / 2)), (Location.y - fHeight), (fWidth / 4), 1, R, G, B, A);//top left
			SetShader("white", ((Location.x + (fWidth / 2)) - (fWidth / 4)), (Location.y - fHeight), (fWidth / 4), 1, R, G, B, A);//top right
			SetShader("white", (Location.x - (fWidth / 2)), Location.y, (fWidth / 4), 1, R, G, B, A);//bottom left
			SetShader("white", ((Location.x + (fWidth / 2)) - (fWidth / 4)), Location.y, (fWidth / 4), 1, R, G, B, A);//bottom right

			SetShader("white", (Location.x - (fWidth / 2)), (Location.y - fHeight), 1, (fHeight / 8), R, G, B, A);//left top
			SetShader("white", (Location.x - (fWidth / 2)), (Location.y - (fHeight / 8)), 1, (fHeight / 8), R, G, B, A);//left bottom
			SetShader("white", (Location.x + (fWidth / 2)), (Location.y - fHeight), 1, (fHeight / 8), R, G, B, A);//right top
			SetShader("white", (Location.x + (fWidth / 2)), (Location.y - (fHeight / 8)), 1, (fHeight / 8), R, G, B, A);//right bottom
		}

		float fFontSize = (0.5 - (GetDistance(Entity[cg->ClientNumber].pose.Origin, Entity[Client].pose.Origin) / 100));

		if (fFontSize < 0.3)
			fFontSize = 0.3;

		if (CE.ESPSettings.ESPName)
			SetTextCenteredWithBackGround(Name, "normalfont", Location.x, ((Location.y + (!CE.ESPSettings.ESPDistance ? TextHeight("normalfont", fFontSize) : 0.0f)) - (fHeight + (TextHeight("normalfont", fFontSize) * 2)) + TextHeight("normalfont", fFontSize)), fFontSize, fFontSize, 1, 1, 1, 1, 0, 0, 0, 0.5);

		if (CE.ESPSettings.ESPDistance)
			SetTextCenteredWithBackGround(va("%im", distance), "normalfont", Location.x, (Location.y - (fHeight + (TextHeight("normalfont", fFontSize) * 1)) + TextHeight("normalfont", fFontSize)), fFontSize, fFontSize, 1, 1, 1, 1, 0, 0, 0, 0.5);

		if (!IsDog)
		{
			if (CE.ESPSettings.ESPWeaponNames)
			{
				const char* WeaponName = GetWeaponName(Entity[Client].nextState.Weapon);
				if (!cuser_strcmp(WeaponName, ""))
					SetTextCenteredWithBackGround(WeaponName, "normalfont", Location.x, ((Location.y + 5) + TextHeight("normalfont", fFontSize)), fFontSize, fFontSize, 1, 1, 1, 1, 0, 0, 0, 0.5);
				else
					SetTextCenteredWithBackGround("none", "normalfont", Location.x, ((Location.y + 5) + TextHeight("normalfont", fFontSize)), fFontSize, fFontSize, 1, 1, 1, 1, 0, 0, 0, 0.5);
			}
			if (CE.ESPSettings.ESPWeaponShaders)
			{
				WeaponDef* weaponDef = BG_GetWeaponDef(Entity[Client].nextState.Weapon);
				Material* shader = GetWeaponShader(Entity[Client].nextState.Weapon);
				float fSize = (40 - (GetDistance(Entity[cg->ClientNumber].pose.Origin, Entity[Client].pose.Origin)));
				if (fSize < 20)
					fSize = 20;
				if (shader && weaponDef)
				{
					if (weaponDef->killIconRatio == WEAPON_ICON_RATIO_1TO1)
						SetShader(shader, (Location.x - (fSize / 2)), (Location.y - (!CE.ESPSettings.ESPWeaponNames ? -(TextHeight("normalfont", fFontSize)) : 0.0f)) + 3.0f, fSize, fSize, 1, 1, 1, 1);
					else if (weaponDef->killIconRatio == WEAPON_ICON_RATIO_2TO1)
						SetShader(shader, (Location.x - ((fSize * 2.0) / 2)), (Location.y - (!CE.ESPSettings.ESPWeaponNames ? -(TextHeight("normalfont", fFontSize)) : 0.0f)) + 3.0f, (fSize * 2.0), fSize, 1, 1, 1, 1);
					else
						SetShader(shader, (Location.x - ((fSize * 2.0) / 2)), (Location.y - (!CE.ESPSettings.ESPWeaponNames ? -(TextHeight("normalfont", fFontSize)) : 0.0f)) + 7.0f, (fSize * 2.0), (fSize * 0.5), 1, 1, 1, 1);
				}
			}
		}
	}
}

void DrawESPBox(int Client)
{
	float R, G, B, A;
	SetESPColor(Client, R, G, B, A);

	float fHeight, fWidth; Vector2 Location; int distance = GetDistance(Entity[cg->ClientNumber].pose.Origin, Entity[Client].pose.Origin);

	if (GetDimentions(Client, &fWidth, &fHeight, &Location))
	{
		if (A)
		{
			if (CE.ESPType == ESP2D)
			{
				SetShader("white", (Location.x - (fWidth / 2)), (Location.y - fHeight), fWidth, 1, R, G, B, A);//top
				SetShader("white", (Location.x - (fWidth / 2)), Location.y, fWidth, 1, R, G, B, A);//bottom
				SetShader("white", (Location.x - (fWidth / 2)), (Location.y - fHeight), 1, fHeight, R, G, B, A);//left
				SetShader("white", (Location.x + (fWidth / 2)), (Location.y - fHeight), 1, fHeight, R, G, B, A);//right
			}
			if (CE.ESPType == ESP3D)
			{
				Vector3 Angles = Entity[Client].nextState.lerp.apos.trBase;
				Angles.x = 0;
				Angles.z = 0;
				
				Vector3 Center = Entity[Client].pose.Origin;
				
				float length = 30.0f, width = 30.0f;
				if (Entity[Client].nextState.lerp.eFlags & FLAG_PRONE) 
				{
					length = 90.0f;
					width = 45.0f;
				
					Angles.y -= 20;
				
					Vector3 NewAngle = Angles;
					NewAngle.y -= 180;
					Center = AnglesToForward(Center, NewAngle, 10);
				
					NewAngle.y += 90;
					Center = AnglesToForward(Center, NewAngle, 5);
				}
				
				Vector3 Dimentions(length, width, GetBoxHeight(Entity[Client]));
				
				Draw3DBox(Center, Dimentions, Angles, R, G, B, A);
			}
			if (CE.ESPType == FB)
			{
				if (A > 0)
					SetShader("white", (Location.x - (fWidth / 2)), (Location.y - fHeight), fWidth, fHeight, R, G, B, 0.2f);
				SetShader("white", (Location.x - (fWidth / 2)), (Location.y - fHeight), fWidth, 1, 0, 0, 0, A);//top
				SetShader("white", (Location.x - (fWidth / 2)), Location.y, fWidth, 1, 0, 0, 0, A);//bottom
				SetShader("white", (Location.x - (fWidth / 2)), (Location.y - fHeight), 1, fHeight, 0, 0, 0, A);//left
				SetShader("white", (Location.x + (fWidth / 2)), (Location.y - fHeight), 1, fHeight, 0, 0, 0, A);//right
			}
			if (CE.ESPType == Corner)
			{
				SetShader("white", (Location.x - (fWidth / 2)), (Location.y - fHeight), (fWidth / 4), 1, R, G, B, A);//top left
				SetShader("white", ((Location.x + (fWidth / 2)) - (fWidth / 4)), (Location.y - fHeight), (fWidth / 4), 1, R, G, B, A);//top right
				SetShader("white", (Location.x - (fWidth / 2)), Location.y, (fWidth / 4), 1, R, G, B, A);//bottom left
				SetShader("white", ((Location.x + (fWidth / 2)) - (fWidth / 4)), Location.y, (fWidth / 4), 1, R, G, B, A);//bottom right

				SetShader("white", (Location.x - (fWidth / 2)), (Location.y - fHeight), 1, (fHeight / 8), R, G, B, A);//left top
				SetShader("white", (Location.x - (fWidth / 2)), (Location.y - (fHeight / 8)), 1, (fHeight / 8), R, G, B, A);//left bottom
				SetShader("white", (Location.x + (fWidth / 2)), (Location.y - fHeight), 1, (fHeight / 8), R, G, B, A);//right top
				SetShader("white", (Location.x + (fWidth / 2)), (Location.y - (fHeight / 8)), 1, (fHeight / 8), R, G, B, A);//right bottom
			}

			float fFontSize = (0.5 - (GetDistance(Entity[cg->ClientNumber].pose.Origin, Entity[Client].pose.Origin) / 100));

			if (fFontSize < 0.3)
				fFontSize = 0.3;

			if (CE.ESPSettings.ESPName)
			{
				Dvar_SetBool("cg_drawFriendlyNames", false);
				Dvar_SetBool("cg_drawCrosshairNames", false);
				int Addition = 0;
				if (!CE.ESPSettings.ESPName)
					Addition = TextHeight("normalfont", fFontSize);
				if (!cuser_strcmp(cg->clientinfo[Client].clanAbbrev, ""))
					SetTextCenteredWithBackGround(va("[%s]%s", cg->clientinfo[Client].clanAbbrev, cg->clientinfo[Client].Name), "normalfont", Location.x, ((Location.y + Addition) - (fHeight + (TextHeight("normalfont", fFontSize) * 2)) + TextHeight("normalfont", fFontSize)), fFontSize, fFontSize, 1, 1, 1, 1, 0, 0, 0, 0.5);
				else
					SetTextCenteredWithBackGround(va("%s", cg->clientinfo[Client].Name), "normalfont", Location.x, ((Location.y + Addition) - (fHeight + (TextHeight("normalfont", fFontSize) * 2)) + TextHeight("normalfont", fFontSize)), fFontSize, fFontSize, 1, 1, 1, 1, 0, 0, 0, 0.5);
			}
			else {
				Dvar_SetBool("cg_drawFriendlyNames", true);
				Dvar_SetBool("cg_drawCrosshairNames", true);
			}

			if (CE.ESPSettings.ESPDistance)
				SetTextCenteredWithBackGround(va("%im", distance), "normalfont", Location.x, (Location.y - (fHeight + (TextHeight("normalfont", fFontSize) * 1)) + TextHeight("normalfont", fFontSize)), fFontSize, fFontSize, 1, 1, 1, 1, 0, 0, 0, 0.5);

			if (CE.ESPSettings.ESPWeaponNames)
			{
				const char* WeaponName = GetWeaponName(Entity[Client].nextState.Weapon);
				if (!cuser_strcmp(WeaponName, ""))
					SetTextCenteredWithBackGround(WeaponName, "normalfont", Location.x, ((Location.y + 5) + TextHeight("normalfont", fFontSize)), fFontSize, fFontSize, 1, 1, 1, 1, 0, 0, 0, 0.5);
				else
					SetTextCenteredWithBackGround("none", "normalfont", Location.x, ((Location.y + 5) + TextHeight("normalfont", fFontSize)), fFontSize, fFontSize, 1, 1, 1, 1, 0, 0, 0, 0.5);
			}

			if (CE.ESPSettings.ESPWeaponShaders)
			{
				int Subtractor = 0;
				if (!CE.ESPSettings.ESPWeaponNames)
					Subtractor = 0;
				else
					Subtractor = -(TextHeight("normalfont", fFontSize));

				WeaponDef* weaponDef = BG_GetWeaponDef(Entity[Client].nextState.Weapon);
				Material* shader = GetWeaponShader(Entity[Client].nextState.Weapon);
				float fSize = (40 - (GetDistance(Entity[cg->ClientNumber].pose.Origin, Entity[Client].pose.Origin)));
				if (fSize < 20)
					fSize = 20;
				if (shader && weaponDef)
				{
					if (weaponDef->killIconRatio == WEAPON_ICON_RATIO_1TO1)
						SetShader(shader, (Location.x - (fSize / 2)), (Location.y - Subtractor) + 3.0f, fSize, fSize, 1, 1, 1, 1);
					else if (weaponDef->killIconRatio == WEAPON_ICON_RATIO_2TO1)
						SetShader(shader, (Location.x - ((fSize * 2.0) / 2)), (Location.y - Subtractor) + 3.0f, (fSize * 2.0), fSize, 1, 1, 1, 1);
					else
						SetShader(shader, (Location.x - ((fSize * 2.0) / 2)), (Location.y - Subtractor) + 7.0f, (fSize * 2.0), (fSize * 0.5), 1, 1, 1, 1);
				}
			}
		}
	}
}

void DrawWeaponsOnScreen(int i)
{
	Vector3 Origin = Entity[i].pose.Origin; Vector2 Location;
	if ((Entity[i].nextState.Weapon > 0) && (Entity[i].pose.eType & 1) && WorldToScreen(Origin, Location))
	{
		WeaponDef* weaponDef = BG_GetWeaponDef(Entity[i].nextState.Weapon);
		Material* shader = GetWeaponShader(Entity[i].nextState.Weapon);
		float fSize = (40 - (GetDistance(Entity[cg->ClientNumber].pose.Origin, Entity[i].pose.Origin)));
		if (fSize < 0)
			fSize = 0;

		if ((shader != NULL) && CE.ESPSettings.DrawWeapons)
		{
			if (weaponDef->killIconRatio == WEAPON_ICON_RATIO_1TO1)
				SetShader(shader, (Location.x - (fSize / 2)), ((Location.y - fSize) + (fSize / 2)), fSize, fSize, 1, 1, 1, 1);
			else if (weaponDef->killIconRatio == WEAPON_ICON_RATIO_2TO1)
				SetShader(shader, (Location.x - ((fSize * 2.0) / 2)), ((Location.y - fSize) + (fSize / 2)), (fSize * 2.0), fSize, 1, 1, 1, 1);
			else
				SetShader(shader, (Location.x - ((fSize * 2.0) / 2)), ((Location.y - fSize) + ((fSize * 0.5) / 2)), (fSize * 2.0), (fSize * 0.5), 1, 1, 1, 1);
		}
		else if (CE.ESPSettings.DrawScavengerPacks && BG_HasPerk("specialty_scavenger") && cuser_strcmp(GetWeaponName(Entity[i].nextState.Weapon), "Scavenger"))
		{
			float fSizeAlt = (35 - (GetDistance(Entity[cg->ClientNumber].pose.Origin, Entity[i].pose.Origin)));
			if (fSizeAlt < 0)
				fSizeAlt = 0;

			SetShader("icon_perks_scavenger", (Location.x - (fSizeAlt / 2)), ((Location.y - fSizeAlt) + (fSizeAlt / 2)), fSizeAlt, fSizeAlt, 1, 1, 1, 0.8);
		}
	}
}

void DrawExplosivesOnScreen(int i)
{
	Vector3 Origin = Entity[i].pose.Origin; Vector2 Location;
	if (WorldToScreen(Origin, Location))
	{
		if (Entity[i].nextState.Type == ET_SCRIPTMOVER)
		{
			if ((Entity[i].nextState.Weapon > 0) && (Entity[i].pose.eType & 0x6))
			{
				WeaponDef* weaponDef = BG_GetWeaponDef(Entity[i].nextState.Weapon);
				Material* shader = GetWeaponShader(Entity[i].nextState.Weapon);
				WeaponVariantDef* WeaponDef = BG_GetWeaponVariantDef(Entity[i].nextState.Weapon);
				float fSize = (40 - (GetDistance(Entity[cg->ClientNumber].pose.Origin, Entity[i].pose.Origin)));
				if (fSize < 0)
					fSize = 0;
				if (shader != NULL)
				{
					if (weaponDef->killIconRatio == WEAPON_ICON_RATIO_1TO1)
						SetShader(shader, (Location.x - (fSize / 2)), ((Location.y - fSize) + (fSize / 2)), fSize, fSize, 1, 1, 1, 1);
					else if (weaponDef->killIconRatio == WEAPON_ICON_RATIO_2TO1)
						SetShader(shader, (Location.x - ((fSize * 2.0) / 2)), ((Location.y - fSize) + (fSize / 2)), (fSize * 2.0), fSize, 1, 1, 1, 1);
					else
						SetShader(shader, (Location.x - ((fSize * 2.0) / 2)), ((Location.y - fSize) + ((fSize * 0.5) / 2)), (fSize * 2.0), (fSize * 0.5), 1, 1, 1, 1);
				}
			}
		}
		else if (Entity[i].nextState.Type == ET_MISSILE)
		{
			if ((Entity[i].nextState.Weapon > 0) && ((Entity[i].pose.eType & 4) || (Entity[i].pose.eType == 4)))
			{
				WeaponDef* weaponDef = BG_GetWeaponDef(Entity[i].nextState.Weapon);
				Material* shader = GetWeaponShader(Entity[i].nextState.Weapon);
				WeaponVariantDef* WeaponDef = BG_GetWeaponVariantDef(Entity[i].nextState.Weapon);
				float fSize = (40 - (GetDistance(Entity[cg->ClientNumber].pose.Origin, Entity[i].pose.Origin)));
				if (fSize < 0)
					fSize = 0;
				if (shader != NULL)
				{
					if (cuser_strcmp(GetWeaponName(Entity[i].nextState.Weapon), "Smoke Grenade"))
						SetShader("hud_us_smokegrenade", (Location.x - (fSize / 2)), ((Location.y - fSize) + (fSize / 2)), fSize, fSize, 1, 1, 1, 1);
					else
					{
						if (weaponDef->killIconRatio == WEAPON_ICON_RATIO_1TO1)
							SetShader(shader, (Location.x - (fSize / 2)), ((Location.y - fSize) + (fSize / 2)), fSize, fSize, 1, 1, 1, 1);
						else if (weaponDef->killIconRatio == WEAPON_ICON_RATIO_2TO1)
							SetShader(shader, (Location.x - ((fSize * 2.0) / 2)), ((Location.y - fSize) + (fSize / 2)), (fSize * 2.0), fSize, 1, 1, 1, 1);
						else
							SetShader(shader, (Location.x - ((fSize * 2.0) / 2)), ((Location.y - fSize) + ((fSize * 0.5) / 2)), (fSize * 2.0), (fSize * 0.5), 1, 1, 1, 1);
					}
				}
				else if (cuser_strcmp(GetWeaponName(Entity[i].nextState.Weapon), "Flash Grenade"))
					SetShader("hud_us_flashgrenade", (Location.x - (fSize / 2)), ((Location.y - fSize) + (fSize / 2)), fSize, fSize, 1, 1, 1, 1);
			}
		}
	}
}

void DrawPlayerPointer(int i)
{
	if (CE.ESPSettings.PlayerIndicator)
	{
		float R, G, B, A;
		SetESPColor(i, R, G, B, A);

		Vector2 cCenter;
		cCenter.x = cg->refdef.width / 2;
		cCenter.y = cg->refdef.height / 2;

		float fRadarDist = 20;
		int RadarX, RadarY;

		RadarX = Entity[cg->ClientNumber].pose.Origin.x - Entity[i].pose.Origin.x;
		RadarY = Entity[cg->ClientNumber].pose.Origin.y - Entity[i].pose.Origin.y;

		float Angle = cg->refdefViewAngles.y / 180 * 3.141;

		float RotateX = RadarY * cos(Angle) - RadarX * sin(Angle);
		float RotateY = RadarX * cos(Angle) + RadarY * sin(Angle);

		float FinalX = cCenter.x + RotateX / fRadarDist;
		float FinalY = cCenter.y + RotateY / fRadarDist;

		float angle = atan(RotateX / RotateY) * (180 / 3.14159265358979323846);
		float FinalA = FinalY > cCenter.y ? -angle - 180.0f : -angle;

		if (FinalA < 0)
			FinalA += 360.0f;

		double radians = (3.14159265358979323846 / 180)*(FinalA - 90.0f);
		float CircleX = cCenter.x + (cos(radians) * (float)CE.ESPSettings.PlayerIndicatorDiameter);
		float CircleY = cCenter.y + (sin(radians) * (float)CE.ESPSettings.PlayerIndicatorDiameter);

		float Colour[4] = { R, G, B, 0.6 };
		const char* PlayerIndicatorIconArray[] = { "hudscoreboardscroll_uparrow", "compassping_player", "demo_timeline_arrow", "hud_offscreenobjectivepointer", "hud_grenadepointer", "waypoint_circle_arrow" };
		R_AddCmdDrawStretchPicRotateXY(CircleX - CE.ESPSettings.PlayerIndicatorSize[0] / 2, CircleY - CE.ESPSettings.PlayerIndicatorSize[1] / 2, CE.ESPSettings.PlayerIndicatorSize[0], CE.ESPSettings.PlayerIndicatorSize[1], 0, 0, 0, 0, (CE.ESPSettings.PlayerIndicatorIcon == 5) ? (FinalA - 180.0f) : FinalA, Colour, PlayerIndicatorIconArray[CE.ESPSettings.PlayerIndicatorIcon]);
	}
}

void DrawESP()
{
	if (!CE.IsMenuOpen)
	{
		DrawRadar();

		for (int i = 36; i < 2048; i++)
		{
			if (Entity[i].nextState.Type == ET_ITEM)
			{
				if(CE.ESPSettings.DrawWeapons)
					DrawWeaponsOnScreen(i);

				if (CE.ExternalRadar)
					DrawWeaponsOnRadar(i);
			}
			else if (Entity[i].nextState.Type == ET_MISSILE)
			{
				if(CE.ESPSettings.DrawExplosives)
					DrawExplosivesOnScreen(i);

				if (CE.ExternalRadar)
					DrawExplosivesOnRadar(i);
			}
		}

		if (!Menu.MenuStatus)
		{
			int count = 0; float Y = ((170 - 60) + CE.MenuSafeArea[1]);
			if (CE.VisibleWarning)
			{
				int t;
				if (isAnyVis(cg->ClientNumber, &t)) {
					SetTextCentered(va("%s is Visible (%.0f%m)", cg->clientinfo[t].Name, GetDistance(cg->refdef.ViewOrigin, Entity[t].pose.Origin), "%"), "normalFont", (cg->refdef.width / 2), Y + (count * 25), 0.5, 0.5, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, 1);
					count++;
				}
			}

			if (CE.AimWarning)
			{
				int t; float degree;
				if (isAimingAtMe(&t, &degree)) {
					SetTextCentered(va("%s is aiming at you (%.0f%s)", cg->clientinfo[t].Name, 100 - (degree / 0.5), "%"), "normalFont", (cg->refdef.width / 2), Y + (count * 25), 0.5, 0.5, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, 1);
					count++;
				}
			}

			if (CE.ProximityWarning) {
				int t;
				if (CloseToMe(&t)) {
					SetTextCentered(va("%s is near (%.01fm)", cg->clientinfo[t].Name, GetDistance(cg->refdef.ViewOrigin, Entity[t].pose.Origin)), "normalFont", (cg->refdef.width / 2), Y + (count * 25), 0.5, 0.5, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, 1);
					count++;
				}
			}
		}

		for (int i = 0; i < 36; i++)
		{
			if (i >= 12)
			{
				if (i == cg->ClientNumber)
					continue;

				CompassActor* compassActor = CG_CompassGetActor(0, i);

				if (!compassActor)
					continue;

				if (compassActor->lastUpdate > cg->time)
					compassActor->lastUpdate = 0;

				if (compassActor->lastUpdate <= (cg->time + -0x1F4))
					continue;

				if (!(Entity[i].pose.eType & 1))
					continue;

				if ((!cg->characterinfo[i].infoValid))
					continue;

				if (Entity[i].nextState.Type != ET_AGENT)
					continue;

				if (!(compassActor->flags & 1)) {
					if (!CE.ESPSettings.DrawEnemies)
						continue;
				}
				else {
					if (!CE.ESPSettings.DrawFriendies)
						continue;
				}


			}
			else
			{
				if (i == cg->ClientNumber)
					continue;

				if (!(Entity[i].nextState.pad & 1))
					continue;

				if ((!cg->characterinfo[i].infoValid))
					continue;

				if ((cg->clientinfo[i].Name == NULL))
					continue;

				if (cuser_strcmp(cg->clientinfo[i].Name, ""))
					continue;

				if (Entity[i].nextState.Type != ET_PLAYER)
					continue;

				if (IsEnemy(i)) {
					if (!CE.ESPSettings.DrawEnemies)
						continue;
				}
				else {
					if (!CE.ESPSettings.DrawFriendies)
						continue;
				}

				if (!Menu.MenuStatus)
					DrawPlayerPointer(i);

				DrawSnapLine(i);
				DrawESPBones(i);
				DrawESPBox(i);
			}
		}

		if (CE.ExternalRadar)
		{
			if (CE.RadarSettings.DrawEnemies)
				CG_CompassDrawEnemies();

			if (CE.RadarSettings.DrawFriendies)
				CG_CompassDrawFriendlies();

			if (CE.RadarSettings.DrawKillStreaks)
			{
				CG_CompassDrawMissiles();
				CG_CompassDrawTurrets();
				VehicleHud_DrawCompass();
				CG_CompassDrawPlanes();
			}
			CG_CompassDrawPlayerPointers_MP();
			CG_CompassDrawPlayer();
		}

		DrawCroshair();
		DrawHealthBar();
	}
}