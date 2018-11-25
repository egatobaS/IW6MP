#include "stdafx.h"
#include "R_EndFrame.h"
#include "Functions.h"
#include "Aimbot.h"

struct GfxCmdHeader
{
	unsigned short id;
	unsigned short byteCount;
};

typedef struct
{
	char *cmds; //0x0
	int usedTotal; //0x4
	int usedCritical; //0x8
	GfxCmdHeader* lastCmd; //0xC
}GfxBackEndData;


void R_EndFrame()
{
	((void(*)(...))0x8266ABA8)(); //R_AddCmdEndOfList

	*(int*)(0x8431C278) = ((*(int*)(0x8431C278) + -1) & 1);

	GfxBackEndData* Current = *(GfxBackEndData**)(*(int*)(0x8431C274) + 0x1868);

	Current->usedTotal = 0;
	Current->usedCritical = 0;
	Current->lastCmd = 0;

	*(int*)(0x842BC180) = 0;
	*(int*)(0x842BC184) = 0;
	*(int*)(0x842BC188) = 0;
	*(int*)(0x842BC18C) = 0;

	if (!(((int(*)(...))0x82482168)() & 0xFF)) //Sys_IsMainThread
		return;

	if (*(int*)0x844DD004)
		return;

	((void(*)(...))0x82686220)(0x21); //R_WaitWorkerCmdsOfType
}

void R_EndFrameHook()
{
	__try
	{
		R_EndFrame();

		if (ShouldHookRun())
		{
			for (int i = 6; i < 12; i++)
			{
				if ((cg->ClientNumber == cg->ps.clientNum) && (cg->ps.Health > 0) && GoodPlayer(i))
				{
					RunRiotChecks(i);
					CacheTagOrigins(i);
				}
			}

			DoClientChecks();

			SetAimbotTarget();
			DoSetViewAngles();
		}
	}
	__except (filter("SCR_DrawScreenFieldHook", GetExceptionCode(), GetExceptionInformation()))
	{
		R_EndFrame();
	}
}