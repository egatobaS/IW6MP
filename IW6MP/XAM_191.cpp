#include "stdafx.h"
#include "XAM_191.h"
#include "Menu.h"
#include "Functions.h"

bool FirstShot = true;

void XAM_191Hook(DWORD r3, int r4, PXINPUT_STATE r5)
{
	__try
	{
		XINPUT_STATE bk = { 0 };

		XInputGetState(r3, &bk);

		if (Menu.MenuStatus && !CE.IsMenuOpen)
		{
			if (bk.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
				bk.Gamepad.wButtons -= XINPUT_GAMEPAD_RIGHT_SHOULDER;

			if (bk.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
				bk.Gamepad.wButtons -= XINPUT_GAMEPAD_LEFT_SHOULDER;

			if (bk.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
				bk.Gamepad.wButtons -= XINPUT_GAMEPAD_DPAD_UP;

			if (bk.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
				bk.Gamepad.wButtons -= XINPUT_GAMEPAD_DPAD_DOWN;

			if (bk.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
				bk.Gamepad.wButtons -= XINPUT_GAMEPAD_DPAD_LEFT;

			if (bk.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
				bk.Gamepad.wButtons -= XINPUT_GAMEPAD_DPAD_RIGHT;

			if (bk.Gamepad.wButtons & XINPUT_GAMEPAD_B)
				bk.Gamepad.wButtons -= XINPUT_GAMEPAD_B;

			if (bk.Gamepad.wButtons & XINPUT_GAMEPAD_A)
				bk.Gamepad.wButtons -= XINPUT_GAMEPAD_A;

			if (Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_TEXTBOX || Menu.SubType[Menu.CurrentSub][Menu.CurrentScroll[Menu.CurrentSub]] == SUB_TEXTBOXCALL)
			{
				if (bk.Gamepad.wButtons & XINPUT_GAMEPAD_X)
					bk.Gamepad.wButtons -= XINPUT_GAMEPAD_X;

				if (bk.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
					bk.Gamepad.wButtons -= XINPUT_GAMEPAD_Y;

				if (bk.Gamepad.wButtons & XINPUT_GAMEPAD_START)
					bk.Gamepad.wButtons -= XINPUT_GAMEPAD_START;
			}
		}

		if (CE.ShouldRunXAM_191 && CE.AimbotStatus && (CE.AimbotTarget != -1))
		{
			if ((CE.AimbotType != 1) && (CE.AimKey == NoAimKey))
			{
				bk.Gamepad.sThumbRX = 0;
				bk.Gamepad.sThumbRY = 0;
			}

			//if (CE.AutoShoot && (FirstShot || (cg->time < CE.StoredTimeForShotDelay) || ((cg->time - CE.StoredTimeForShotDelay) > CE.DelayTimeFix)))
			//{
			//	CE.StoredTimeForShotDelay = cg->time;
			//	if (!strstr(GetWeaponName(Entity[cg->ClientNumber].nextState.Weapon), "Riot Shield") || (BG_GetAmmoInClip(&cg->ps, cg->ps.Weapon) > 0)) {
			//		bk.Gamepad.bRightTrigger = 255;
			//		FirstShot = false;
			//	}
			//}
		}
		else FirstShot = true;

		memcpy(r5, &bk, sizeof(XINPUT_STATE));
	}
	__except (filter("XAM_191", GetExceptionCode(), GetExceptionInformation()))
	{

	}
}