#include "stdafx.h"
#include "Debug.h"
#include "Lib.h"
#include "Structures.h"
#include "Drawing.h"
#include "Menu.h"
#include "Functions.h"

char* StanceToText(int client)
{
	if (Entity[client].prevState.eFlags & FLAG_CROUCHED)
		return "Crouched";
	else if (Entity[client].prevState.eFlags & FLAG_PRONE)
		return "Prone";
	else
		return "Standing";
}

int CurrentIndex;
void DrawDebug()
{
	UiContext uicontext = *(UiContext*)addr->_0x83D51E20;
	int CurrentIndex = 0;
	char Buffer[0x200];

	#ifdef DEVKIT
	if (Dvar_GetBool("cl_ingame"))
	{
		for (int i = 0; i < 12; i++)
		{
			if ((cg->ClientNumber == cg->ps.clientNum) && (cg->ps.Health > 0) && GoodEnemy(i))
			{
				if ((strstr(GetWeaponName(Entity[i].nextState.Weapon), "Riot Shield") || HasShieldAttached(i)))
				{
					float Yaw = GetAimDegreeA(i) / (WallDepthA(Entity[cg->ClientNumber].pose.Origin, Entity[i].pose.Origin));
					float Distance = GetDistance(Entity[cg->ClientNumber].pose.Origin, Entity[i].pose.Origin);
					const char* Side = "N/A";
					if (SideOfPlayer(i) == LeftSide)
						Side = "Left";
					else if (SideOfPlayer(i) == RightSide)
						Side = "Right";
					else if (SideOfPlayer(i) == CenterSide)
						Side = "Center";

					char DebugText[0x200];
					_snprintf(DebugText, sizeof(DebugText), "^1Yaw = %f^7\nPitch = %f\nDistance = %f\nSide = %s", (Yaw * 100), Entity[i].nextState.lerp.apos.trBase.x, Distance, Side);
					DrawBox("white", 8, ((uicontext.screenHeight / 2) - (TextHeight("normalFont", 0.5) + 3)), ((TextWidth(DebugText, "normalFont") * 0.5) + 4), 3, 3, 1, 0, 0, 0.6, 0, 0, 0, 0);
					DrawBox("white", 8, ((uicontext.screenHeight / 2)), ((TextWidth(DebugText, "normalFont") * 0.5) + 4), TextHeight("normalFont", 0.5) * 3, 3, 0, 0, 0, 0.6, 0, 0, 0, 0);
					SetTextWithBackGround(DebugText, "normalFont", 10, uicontext.screenHeight / 2, 0.5, 0.5, 1, 1, 1, 1, 0, 0, 0, 0.6);
				}
			}
		}
	}
	#endif

	if (CE.Controls)
	{
		if (!Menu.MenuStatus)
			user_strcpy(Buffer, "Press  +  to open the Menu || Press  +  to open the Players Menu");
		else {
			char NONE = 0xFF;
			if (*Menu.LastSub[Menu.CurrentSub] == NONE) {
				if ((Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] != SUB_STRINGARRAY) && (Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] != SUB_STRINGARRAYCALL))
				{
					if (Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_BOOL || Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_BOOLCALL || Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_BRANCH || Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_FUNCTIONVOID)
						user_strcpy(Buffer, "Press  to close the menu ||  /  to scroll ||  to select ||  /  to scroll tabs");
					else if (Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_TEXTBOX)
						user_strcpy(Buffer, "Press  to close the menu ||  to copy ||  to paste ||  to edit ||  to clear ||  /  to scroll tabs");
					else if (Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_TEXTBOXCALL)
						user_strcpy(Buffer, "Press  to close the menu ||  to copy ||  to paste ||  to edit ||  to clear ||  to set name");
					else
						user_strcpy(Buffer, "Press  to close the menu ||  /  to scroll ||  /  to select ||  /  to scroll tabs");
				}
				else
				{
					if ((Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_STRINGARRAYCALL) || (Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_INTCALL))
						user_strcpy(Buffer, "Press  to close the menu ||  /  to scroll ||  /  to select ||  to run option ||  /  to scroll tabs");
					else
					{
						if (CE.ArrayStyle == Single)
							user_strcpy(Buffer, "Press  to close the menu ||  /  to scroll ||  /  to select ||  /  to scroll tabs");
						else if (CE.ArrayStyle == DropDown) {
							if (!CE.IsDropDownOpen)
								user_strcpy(Buffer, "Press  to go to the previous menu ||  /  to scroll ||  to open drop down ||  /  to scroll tabs");
							else
								user_strcpy(Buffer, "   to confirm the drop down ||  /  to scroll");
						}
					}
				}
			}
			else
			{
				if ((Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_STRINGARRAYCALL) || (Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_INTCALL))
					user_strcpy(Buffer, "Press  to go to the previous menu ||  /  to scroll ||  /  to select ||  to run option");
				else
				{
					if (Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] != SUB_STRINGARRAY)
					{
						if (Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_BOOL || Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_BOOLCALL || Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_BRANCH || Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_FUNCTIONVOID)
							user_strcpy(Buffer, "Press  to go to the previous menu ||  /  to scroll ||  to select");
						else if (Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_TEXTBOX)
							user_strcpy(Buffer, "Press  to go to the previous menu ||  to copy ||  to paste ||  to edit ||  to clear");
						else if (Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_TEXTBOXCALL)
							user_strcpy(Buffer, "Press  to go to the previous menu ||  to copy ||  to paste ||  to edit ||  to clear ||  to set name");
						else
							user_strcpy(Buffer, "Press  to go to the previous menu ||  /  to scroll ||  /  to select");
					}
					else
					{
						if (CE.ArrayStyle == Single)
							user_strcpy(Buffer, "Press  to go to the previous menu ||  /  to scroll ||  /  to select");
						else if (CE.ArrayStyle == DropDown) {
							if (!CE.IsDropDownOpen)
								user_strcpy(Buffer, "Press  to go to the previous menu ||  /  to scroll ||  to open drop down");
							else
								user_strcpy(Buffer, "   to confirm the drop down ||  /  to scroll");
						}
					}
				}
			}
		}
		SetTextWithBackGround(Buffer, "normalFont", ((((uicontext.screenWidth + 15) - uicontext.screenWidth) + 60) - CE.MenuSafeArea[0]), (((uicontext.screenHeight - 10) - 60) + CE.MenuSafeArea[1]), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);
	}

	float RightX = (((uicontext.screenWidth - 15) - 60) + CE.MenuSafeArea[0]); float RightY = ((25 + 60) - CE.MenuSafeArea[1]);

	_snprintf(Buffer, sizeof(Buffer), "%s v%s", CheatName, CheatVersion);
	SetTextWithBackGround(Buffer, "normalFont", (RightX - (TextWidth(Buffer, "normalFont") * 0.5)), RightY + (25 * CurrentIndex), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);
	CurrentIndex++;

	if (!CE.IsMenuOpen)
	{
		if (CE.ScreenResolution) {
			_snprintf(Buffer, sizeof(Buffer), "Screen Resolution: %i x %i", uicontext.screenWidth, uicontext.screenHeight);
			SetTextWithBackGround(Buffer, "normalFont", (RightX - (TextWidth(Buffer, "normalFont") * 0.5)), RightY + (25 * CurrentIndex), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);
			CurrentIndex++;
		}

		if (CE.FPS) {
			_snprintf(Buffer, sizeof(Buffer), "FPS: %0.0f", ((uicontext.FPS > 60) ? (((uicontext.FPS / 2) > 60) ? 60.0f : (uicontext.FPS / 2)) : uicontext.FPS));
			SetTextWithBackGround(Buffer, "normalFont", (RightX - (TextWidth(Buffer, "normalFont") * 0.5)), RightY + (25 * CurrentIndex), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);
			CurrentIndex++;
		}

		if (CE.Host && ShouldHookRun()) {
			_snprintf(Buffer, sizeof(Buffer), "Host: %s", cgs->szHostName);
			SetTextWithBackGround(Buffer, "normalFont", (RightX - (TextWidth(Buffer, "normalFont") * 0.5)), RightY + (25 * CurrentIndex), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);
			CurrentIndex++;
		}

		if (CE.Map) {
			_snprintf(Buffer, sizeof(Buffer), "Map: %s", UI_GetMapName(Dvar_GetString("ui_mapname")));
			SetTextWithBackGround(Buffer, "normalFont", (RightX - (TextWidth(Buffer, "normalFont") * 0.5)), RightY + (25 * CurrentIndex), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);
			CurrentIndex++;
		}

		if (CE.GameMode) {
			_snprintf(Buffer, sizeof(Buffer), "Game Mode: %s", UI_GetGameTypeName(Dvar_GetString("ui_gametype")));
			SetTextWithBackGround(Buffer, "normalFont", (RightX - (TextWidth(Buffer, "normalFont") * 0.5)), RightY + (25 * CurrentIndex), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);
			CurrentIndex++;
		}

		if (ShouldHookRun() && cg->snap)
		{
			if (CE.Ping) {
				_snprintf(Buffer, sizeof(Buffer), "Ping: %i ms", cg->nextSnap->ping);
				SetTextWithBackGround(Buffer, "normalFont", (RightX - (TextWidth(Buffer, "normalFont") * 0.5)), RightY + (25 * CurrentIndex), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);
				CurrentIndex++;
			}

			if (CE.NumberOfEnt) {
				_snprintf(Buffer, sizeof(Buffer), "Number of Entities: %i", cg->snap->numEntities);
				SetTextWithBackGround(Buffer, "normalFont", (RightX - (TextWidth(Buffer, "normalFont") * 0.5)), RightY + (25 * CurrentIndex), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);
				CurrentIndex++;
			}

			if (CE.NumberOfClients) {
				_snprintf(Buffer, sizeof(Buffer), "Number of Clients: %i / %i", cg->snap->numClients, Dvar_GetInt("sv_maxclients"));
				SetTextWithBackGround(Buffer, "normalFont", (RightX - (TextWidth(Buffer, "normalFont") * 0.5)), RightY + (25 * CurrentIndex), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);
				CurrentIndex++;
			}


			if (CE.TargetDetails && CE.AimbotStatus)
			{
				if (CE.ScreenResolution || CE.FPS || CE.Ping || CE.Map || CE.GameMode || CE.Host)
					CurrentIndex++;

				SetTextWithBackGround("Target Details:", "normalFont", (RightX - (TextWidth("Target Details:", "normalFont") * 0.5)), RightY + (25 * CurrentIndex), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);
				CurrentIndex++;

				if (CE.AimbotTarget != -1)
				{
					char Buffer[0x200];
					_snprintf(Buffer, sizeof(Buffer), "Name: %s", cg->clientinfo[CE.AimbotTarget].Name);
					SetTextWithBackGround(Buffer, "normalFont", (RightX - (TextWidth(Buffer, "normalFont") * 0.5)), RightY + (25 * CurrentIndex), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);
					CurrentIndex++;

					int Distance = GetDistance(Entity[cg->ClientNumber].pose.Origin, Entity[CE.AimbotTarget].pose.Origin);
					_snprintf(Buffer, sizeof(Buffer), "Distance: %im", Distance);
					SetTextWithBackGround(Buffer, "normalFont", (RightX - (TextWidth(Buffer, "normalFont") * 0.5)), RightY + (25 * CurrentIndex), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);
					CurrentIndex++;

					_snprintf(Buffer, sizeof(Buffer), "Tag Targeting: %s", ClientOptions[CE.AimbotTarget].AimAtRoot ? "j_mainroot" : ((CE.AimbotTag[CE.AimbotTarget] > 0) ? CE.AimbotTag[CE.AimbotTarget] : TagList[CE.AimTag - 1]));
					SetTextWithBackGround(Buffer, "normalFont", (RightX - (TextWidth(Buffer, "normalFont") * 0.5)), RightY + (25 * CurrentIndex), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);
					CurrentIndex++;

					_snprintf(Buffer, sizeof(Buffer), "Visible/Killable: %s", (CE.CanSeePlayer[CE.AimbotTarget] || CE.IsKillable) ? "True" : "False");
					SetTextWithBackGround(Buffer, "normalFont", (RightX - (TextWidth(Buffer, "normalFont") * 0.5)), RightY + (25 * CurrentIndex), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);
					CurrentIndex++;

					_snprintf(Buffer, sizeof(Buffer), "Stance: %s", StanceToText(CE.AimbotTarget));
					SetTextWithBackGround(Buffer, "normalFont", (RightX - (TextWidth(Buffer, "normalFont") * 0.5)), RightY + (25 * CurrentIndex), 0.5, 0.5, CE.TextColour.R, CE.TextColour.G, CE.TextColour.B, CE.TextColour.A, CE.MenuSecondaryColour.R, CE.MenuSecondaryColour.G, CE.MenuSecondaryColour.B, CE.MenuSecondaryColour.A, CE.MenuPrimaryColour.R, CE.MenuPrimaryColour.G, CE.MenuPrimaryColour.B, CE.MenuPrimaryColour.A);
					CurrentIndex++;
				}
			}
		}
	}
}