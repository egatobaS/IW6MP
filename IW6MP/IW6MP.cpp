#include "stdafx.h"
#include "LoadCFG.h"
#include "Menu.h"
#include "MenuFunc.h"
#include "Functions.h"

//Hooks
#include "SCR_DrawScreenField.h"
#include "R_AddCmdDrawStretchPic.h"
#include "XAM_191.h"
#include "R_EndFrame.h"
#include "CL_WritePacket.h"
#include "CG_Obituary.h"
#include "CG_VisionSetStartLerp_To.h"
#include "CL_Disconnect.h"


Detour* XAM_191Detour;
Detour* R_EndFrameDetour;
Detour* SCR_DrawScreenFieldDetour;
Detour* CL_WritePacketDetour;
Detour* CG_ObituaryDetour;
Detour* CG_VisionSetStartLerp_ToDetour;
Detour* CL_DisconnectDetour;

void RestoreHookGameHooks()
{
	XAM_191Detour->RestoreFunction();
	R_EndFrameDetour->RestoreFunction();
	SCR_DrawScreenFieldDetour->RestoreFunction();
	CL_WritePacketDetour->RestoreFunction();
	CG_ObituaryDetour->RestoreFunction();
	CG_VisionSetStartLerp_ToDetour->RestoreFunction();
	CL_DisconnectDetour->RestoreFunction();
}

DWORD WINAPI ManagerThread()
{
	while (!KillThread && (XamGetCurrentTitleID() == 0x415608FC) && cuser_strcmp((char*)0x820494C0, "mw3_content_required_error"))
	{
		__try
		{
			MonitorCFGThread();
			EnableSettingsFromCFG();
			UnInfector();
			Sleep(10);
		}
		__except (filter("ManagerThread", GetExceptionCode(), GetExceptionInformation()))
		{

		}
	}

	DWORD Thr_Exit = 0;
	ExitThread(Thr_Exit);
}

void StartManagerThread()
{
	HANDLE hThread; DWORD threadId;
	ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)ManagerThread, NULL, 2 | CREATE_SUSPENDED);
	XSetThreadProcessor(hThread, 3);
	ResumeThread(hThread);
}

Detour ExTerminateTitleProcessDetour;
typedef int(*pExTerminateTitleProcess)();
pExTerminateTitleProcess ExTerminateTitleProcessOriginal;

int ExTerminateTitleProcessHook()
{
	if ((XamGetCurrentTitleID() == 0x415608FC) && cuser_strcmp((char*)0x820494C0, "mw3_content_required_error"))
	{
		__try
		{
			KillThread = true;
			RestoreHookGameHooks();
			Sleep(500);
		}
		__except (filter("ExTerminateTitleProcessHook", GetExceptionCode(), GetExceptionInformation()))
		{

		}
	}

	return ExTerminateTitleProcessOriginal();
}


DWORD WINAPI InitThread()
{
	__try
	{
#ifdef DEVKIT
		CreateSymbolicLink("xbOnline:\\", "\\Device\\Harddisk0\\Partition1", true);
#endif

		MemoryBuffer Buf;
		if (CReadFile("xbOnline:\\xbOnline\\IW6MP.cfg", Buf))
		{
			std::string cfg = (const char*)Buf.GetData();

			if (cfg.find("Anti-Aim Menu") != -1)
				remove("xbOnline:\\xbOnline\\IW6MP.cfg");
		}

		if (!FileExists("xbOnline:\\xbOnline"))
			CreateDirectory("xbOnline:\\xbOnline", NULL);

		if (!FileExists("xbOnline:\\xbOnline\\UserGamertags.txt"))
			CreateNewFile("xbOnline:\\xbOnline\\UserGamertags.txt");

		if (!FileExists("xbOnline:\\xbOnline\\LooperGamerTags.txt"))
			CreateNewFile("xbOnline:\\xbOnline\\LooperGamerTags.txt");

		if (!FileExists("xbOnline:\\xbOnline\\SavedPlayers.cfg"))
			CreateNewFile("xbOnline:\\xbOnline\\SavedPlayers.cfg");

		SavedPlayers = (SavedPlayers_s*)malloc(sizeof(SavedPlayers_s));
		LoadAddresses();

		if ((XamGetCurrentTitleID() == 0x415608FC) && cuser_strcmp((char*)0x820494C0, "mw3_content_required_error"))
		{
			//version number 
			strcpy((char*)0x8204B874, va("^4IW6MP %s\0", CheatVersion));

			void(*SetUnhandledExceptionFilter_Game)(LPTOP_LEVEL_EXCEPTION_FILTER obj) = (void(*)(LPTOP_LEVEL_EXCEPTION_FILTER))0x8272CDB8;
			SetUnhandledExceptionFilter_Game(OurCrashHandler);
			SetUnhandledExceptionFilter(OurCrashHandler);
			ApplyPatches();

			user_strcpy((char*)CE.SetGamertag, (char*)addr->NameAddress);

			LoadUserGamertags(true);
			LoadLooperGamertags(true);
			CreateMenu();
			CE.Reset();

			XAM_191Detour = GetDetour();
			R_EndFrameDetour = GetDetour();
			SCR_DrawScreenFieldDetour = GetDetour();
			CL_WritePacketDetour = GetDetour();
			CG_ObituaryDetour = GetDetour();
			CG_VisionSetStartLerp_ToDetour = GetDetour();
			CL_DisconnectDetour = GetDetour();

			xbOHookFunction(XAM_191Detour, (void*)addr->XAM_191, (void*)XAM_191Hook);
			xbOHookFunction(R_EndFrameDetour, (void*)addr->R_EndFrame, (void*)R_EndFrameHook);
			SCR_DrawScreenFieldStub = (pSCR_DrawScreenFieldStub)xbOHookFunction(SCR_DrawScreenFieldDetour, (void*)addr->SCR_DrawScreenField, (void*)SCR_DrawScreenFieldHook);
			//CL_WritePacketStub = (pCL_WritePacketStub)xbOHookFunction(CL_WritePacketDetour, (void*)(void*)addr->CL_WritePacket, (void*)CL_WritePacketHook);
			//CG_VisionSetStartLerp_ToStub = (pCG_VisionSetStartLerp_ToStub)xbOHookFunction(CG_VisionSetStartLerp_ToDetour, (void*)addr->CG_VisionSetStartLerp_To, (void*)CG_VisionSetStartLerp_ToHook);
			//CL_DisconnectStub = (pCL_DisconnectStub)xbOHookFunction(CL_DisconnectDetour, (void*)addr->CL_Disconnect, (void*)CL_DisconnectHook);
			//CG_ObituaryStub = (pCG_ObituaryStub)xbOHookFunction(CG_ObituaryDetour, (void*)addr->CG_Obituary, (void*)CG_ObituaryHook);

			//InitializeAutoWallThreads();
			StartManagerThread();

			XNotify(toWCHAR("%s [Ghosts] Loaded!\nDeveloped by OSM & Sabotage <3", CheatName));

			LoadPlayersCFG(true);
			LoadMenuSettingsCFG(true);
			LoadCFG(true);
			FlashingGmaertag();
		}
	}
	__except (filter("InitThread", GetExceptionCode(), GetExceptionInformation()))
	{

	}

	ExTerminateTitleProcessOriginal = (pExTerminateTitleProcess)ExTerminateTitleProcessDetour.HookFunction(IsDevkit() ? 0x81D0FC8C : 0x81A71974, (unsigned int)ExTerminateTitleProcessHook);

	DWORD Thr_Exit = 0;
	ExitThread(Thr_Exit);
}

BOOL WINAPI DllMain(HANDLE hInstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		HANDLE hThread; DWORD threadId;
		ExCreateThread(&hThread, 0x10000, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)InitThread, NULL, 2 | CREATE_SUSPENDED);
		XSetThreadProcessor(hThread, 4);
		SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
		ResumeThread(hThread);
		break;
	case DLL_PROCESS_DETACH:

		ExTerminateTitleProcessDetour.RestoreFunction();

		#ifdef DEVKIT
		if ((XamGetCurrentTitleID() == 0x415608FC) && cuser_strcmp((char*)0x820494C0, "mw3_content_required_error"))
		{
			KillThread = true;
			RestoreHookGameHooks();
			Sleep(1000);
		}
		#endif

		#ifndef DEVKIT
		if (SavedPlayers)
			free(SavedPlayers);
		#endif

		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}