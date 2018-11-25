#include "stdafx.h"
#include "R_AddCmdDrawStretchPic.h"
#include "Menu.h"
#include "Drawing.h"


bool isSpawned = false;


int ColourIndex;
float R = 0, G = 0, B = 255;

void DoColors()
{
	if (ColourIndex == 0)
	{

		if (B != 0)
			B--;
		R++;
		if (R == 255)
			ColourIndex = 1;
	}
	if (ColourIndex == 1)
	{
		if (R != 0)
			R--;
		G++;
		if (G == 255)
			ColourIndex = 2;
	}
	if (ColourIndex == 2)
	{
		if (G != 0)
			G--;
		B++;
		if (B == 255)
			ColourIndex = 0;
	}

	//Menu
	if (CE.MenuPrimaryColour.Rainbow) {
		CE.MenuPrimaryColour.R = (R / 255); CE.MenuPrimaryColour.G = (G / 255); CE.MenuPrimaryColour.B = (B / 255);
	}
	if (CE.MenuSecondaryColour.Rainbow) {
		CE.MenuSecondaryColour.R = (R / 255); CE.MenuSecondaryColour.G = (G / 255); CE.MenuSecondaryColour.B = (B / 255);
	}
	if (CE.TextColour.Rainbow) {
		CE.TextColour.R = (R / 255); CE.TextColour.G = (G / 255); CE.TextColour.B = (B / 255);
	}

	//ESP
	if (CE.AimbotTargetColor.Rainbow) {
		CE.AimbotTargetColor.R = (R / 255); CE.AimbotTargetColor.G = (G / 255); CE.AimbotTargetColor.B = (B / 255);
	}
	if (CE.EnemyColor.Rainbow) {
		CE.EnemyColor.R = (R / 255); CE.EnemyColor.G = (G / 255); CE.EnemyColor.B = (B / 255);
	}
	if (CE.PriorityEnemyColor.Rainbow) {
		CE.PriorityEnemyColor.R = (R / 255); CE.PriorityEnemyColor.G = (G / 255); CE.PriorityEnemyColor.B = (B / 255);
	}
	if (CE.FriendlyColor.Rainbow) {
		CE.FriendlyColor.R = (R / 255); CE.FriendlyColor.G = (G / 255); CE.FriendlyColor.B = (B / 255);
	}
	if (CE.KillableColor.Rainbow) {
		CE.KillableColor.R = (R / 255); CE.KillableColor.G = (G / 255); CE.KillableColor.B = (B / 255);
	}
	if (CE.VisibleColor.Rainbow) {
		CE.VisibleColor.R = (R / 255); CE.VisibleColor.G = (G / 255); CE.VisibleColor.B = (B / 255);
	}
	if (CE.BonesColor.Rainbow) {
		CE.BonesColor.R = (R / 255); CE.BonesColor.G = (G / 255); CE.BonesColor.B = (B / 255);
	}

	//if (Dvar_GetBool("cl_ingame") && GetStructs() && CG_IsFullyInitialized() && !CL_IsServerRestarting() && (CL_GetLocalClientMigrationState(0) == CMSTATE_INACTIVE) && cg->renderScreen && cg->predictedPlayerState.Health > 0)
	//	SetFogRGBD(R, G, B, 100.f);
	//cg->serverFogSettings.r = R; cg->serverFogSettings.g = G; cg->serverFogSettings.b = B; cg->serverFogSettings.density = 100.0f;
}

