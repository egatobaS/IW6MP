#include "stdafx.h"
#include "SCR_DrawScreenField.h"
#include "R_AddCmdDrawStretchPic.h"
#include "Menu.h"
#include "MenuFunc.h"
#include "Functions.h"
#include "Debug.h"
#include "ESP.h"
#include "Drawing.h"

pSCR_DrawScreenFieldStub SCR_DrawScreenFieldStub;

void SCR_DrawScreenFieldHook(int r3, int r4)
{
	__try
	{
		static bool HasReset = false;
		SCR_DrawScreenFieldStub(r3, r4);

		if (ShouldHookRun())
		{
			for (int i = 0; i < 6; i++)
			{
				if ((cg->ClientNumber == cg->ps.clientNum) && (cg->ps.Health > 0) && GoodPlayer(i))
				{
					RunRiotChecks(i);
					CacheTagOrigins(i);
				}
			}

			if (!CG_IsSelectingLocation(0) && (clientUIActive->OpenMenu != 0x40))
				CE.IsMenuOpen = false;
			else
				CE.IsMenuOpen = true;

			ModLooper();
			DrawESP();
			//OnPlayerSpawn();
			//DoCrouchBot();

			if (HasReset)
			{
				for (int i = 0; i < 12; i++)
					ClientOptions[i].reset();
				HasReset = false;
			}
		}

		if (!Dvar_GetBool("cl_ingame"))
		{
			if (!HasReset)
			{
				for (int i = 0; i < 12; i++)
					ClientOptions[i].reset();
				HasReset = true;
			}
			bDisConnected = false;
			CE.IsMenuOpen = false;
		}

		DrawDebug();
		DrawMenu();
		DoColors();
		MenuMonitor();
	}
	__except (filter("SCR_DrawScreenField", GetExceptionCode(), GetExceptionInformation()))
	{

	}
}