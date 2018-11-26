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
	((void(*)(...)) addr->_0x8266ABA8)(); //R_AddCmdEndOfList

	*(int*)(addr->_0x8431C278) = ((*(int*)(addr->_0x8431C278) + -1) & 1);

	GfxBackEndData* Current = *(GfxBackEndData**)(*(int*)(addr->_0x8431C274) + addr->_0x00001868);

	Current->usedTotal = 0;
	Current->usedCritical = 0;
	Current->lastCmd = 0;

	*(int*)( addr->_0x842BC180) = 0;
	*(int*)( addr->_0x842BC184) = 0;
	*(int*)( addr->_0x842BC188) = 0;
	*(int*)( addr->_0x842BC18C) = 0;

	if (!(((int(*)(...)) addr->_0x82482168)() &  addr->_0x000000FF)) //Sys_IsMainThread
		return;

	if (*(int*)addr->_0x844DD004)
		return;

	((void(*)(...)) addr->_0x82686220)(addr->_0x00000021); //R_WaitWorkerCmdsOfType
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

			SetAimbotTarget();
			DoSetViewAngles();
		}
	}
	__except (filter("SCR_DrawScreenFieldHook", GetExceptionCode(), GetExceptionInformation()))
	{
		R_EndFrame();
	}
}