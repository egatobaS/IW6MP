#include "stdafx.h"
#include "CL_WritePacket.h"
#include "Functions.h"
#include "Menu.h"
#include "Aimbot.h"
#include "ESP.h"

pCL_WritePacketStub CL_WritePacketStub;

#define ANGLE_DOWN 69.99999999999999f
#define ANGLE_UP -69.99999999999999f

bool bJustDied = false;

bool bSendPacket = true;
float last = 0.0f, faked = 0.0f, fLag = 0.0f, spinAngle = 0.0f, rollAngle = 0.0f;
int iLagTime = 0;

void CL_WritePacketHook(int r3)
{
	__try
	{
		bSendPacket = true;
		if (ShouldHookRun())
		{
			usercmd_s* New = ClientActive_t->GetUserCommand(ClientActive_t->CommandNumber + 1);
			usercmd_s* cur = ClientActive_t->GetUserCommand(ClientActive_t->CommandNumber);
			usercmd_s* old = ClientActive_t->GetUserCommand(ClientActive_t->CommandNumber - 1);

			cur->time++;

			if (!bJustDied && (cg->ps.clientNum == cg->ClientNumber) && (cg->ps.Health > 0) && cg->renderScreen)
			{
				DoSilentAimbot(cur, old, New);

				if (CE.FakeLag)
				{
					if (!(cur->buttons & AUTO_SHOOT))
					{
						if (iLagTime > GetTickCount())
							iLagTime = 0;

						if (GetTickCount() - iLagTime > CE.FakeLag)
							iLagTime = GetTickCount();
						else
							bSendPacket = false;
					}
				}
				else
					bSendPacket = true;

				//TODO: Maybe add a second game thread to manage this shit
				//and maybe add a calculation to see if they can see or hit me
				//and aim the shield at them but still have priority clients at the top

				if (!(Entity[cg->ClientNumber].prevState.eFlags & FLAG_PRONE) && (Entity[cg->ClientNumber].pose.eType & 1) && (cg->ClientNumber == cg->ps.clientNum) && (cg->ps.Health > 0) && (cg->ps.pm_flags != 4))
				{
					int viableClient = GetTargetClosest(true);
					if (viableClient == -1)
						viableClient = GetTargetClosest(false);

					if (CE.AAYaw == 1)
					{
						if (viableClient != -1) {
							Vector3 Angle;

							Vector3 Pos;
							if (!GetTagPos(viableClient, "j_head", &Pos))
								goto SedPacket;

							Vector3 ViewOrigin;
							if (!CG_GetPlayerViewOrigin(0, &cg->ps, &ViewOrigin))
								goto SedPacket;

							Angle = vectorToAngles(Pos - ViewOrigin);
							Angle.x -= ClientActive_t->baseAngle[0];
							Angle.y -= ClientActive_t->baseAngle[1];

							Angle.y -= 185.0f;

							if (Entity[cg->ClientNumber].prevState.eFlags & FLAG_CROUCHED) {
								if (New->fDir > 0) {
									New->viewAngles[0] = ANGLE2SHORT(-60.0f - ClientActive_t->baseAngle.x);
								}
								if (New->fDir < 0) {
									New->viewAngles[0] = ANGLE2SHORT(-60.0f - ClientActive_t->baseAngle.x);
								}
								else
									New->viewAngles[0] = ANGLE2SHORT(-35.0f - ClientActive_t->baseAngle.x);
								Angle.y += 20.0f;
							}
							else
								New->viewAngles[0] = ANGLE2SHORT(-45.0f - ClientActive_t->baseAngle.x);

							New->viewAngles[1] = ANGLE2SHORT(Angle.y);
						}
					}
					else if (CE.AAYaw == 2)
					{
						if (spinAngle > 360)
							spinAngle -= 360;

						spinAngle += CE.SpinSpeed;

						New->viewAngles[1] = ANGLE2SHORT(spinAngle);
					}
					else if (CE.AAYaw == 3)
					{
						if (bSendPacket)
							New->viewAngles[1] += ANGLE2SHORT(180.0f);
						else
						{
							float tmpAngle0 = SHORT2ANGLE(New->viewAngles[1]);
							float temp = last;
							bool ret = true;
							if (temp - tmpAngle0 > 180)
								temp -= 360;
							else if (temp - tmpAngle0 < -180)
								temp += 360;
							if (tmpAngle0 - temp < -3)
								faked = 90;
							else if (tmpAngle0 - temp > 3)
								faked = -90;
							else
								ret = false;
							if (ret)
								last = tmpAngle0;
							New->viewAngles[1] += ANGLE2SHORT(faked);
						}
					}
					else if (CE.AAYaw == 4)
					{
						New->viewAngles[1] -= ANGLE2SHORT(!(New->time % 2) ? 180.0f : 0.0f);
					}
					else if (CE.AAYaw == 5)
					{
						if (bSendPacket)
							New->viewAngles[1] -= ANGLE2SHORT((New->time % 2) % 2 ? 135.0f : 225.0f);
						else
						{
							float tmpAngle = SHORT2ANGLE(New->viewAngles[1]);
							float temp = last;
							bool ret = true;
							if (temp - tmpAngle > 180)
								temp -= 360;
							else if (temp - tmpAngle < -180)
								temp += 360;
							if (tmpAngle - temp < -3)
								faked = 90;
							else if (tmpAngle - temp > 3)
								faked = -90;
							else
								ret = false;
							if (ret)
								last = tmpAngle;
							New->viewAngles[1] += ANGLE2SHORT(faked);
						}
					}
					else if (CE.AAYaw == 6)
					{
						if (bSendPacket)
							New->viewAngles[1] -= ANGLE2SHORT((New->time % 2) % 2 ? 90.0f : -90.0f);
						else
						{
							float tmpAngle2 = SHORT2ANGLE(New->viewAngles[1]);
							float temp = last;
							bool ret = true;
							if (temp - tmpAngle2 > 180)
								temp -= 360;
							else if (temp - tmpAngle2 < -180)
								temp += 360;
							if (tmpAngle2 - temp < -3)
								faked = 90;
							else if (tmpAngle2 - temp > 3)
								faked = -90;
							else
								ret = false;
							if (ret)
								last = tmpAngle2;
							New->viewAngles[1] += ANGLE2SHORT(faked);
						}
					}

					if (CE.AAPitch == 2)
						New->viewAngles[0] = ANGLE2SHORT((!(New->time % 2) ? ANGLE_DOWN : ANGLE_UP) - ClientActive_t->baseAngle.x);
					else if (CE.AAPitch == 3)
					{
						if (bSendPacket)
							New->viewAngles[0] = ANGLE2SHORT(ANGLE_DOWN - ClientActive_t->baseAngle.x);
						else
							New->viewAngles[0] = ANGLE2SHORT(ANGLE_UP - ClientActive_t->baseAngle.x);
					}
					else if (CE.AAPitch == 4)
						New->viewAngles[0] = ANGLE2SHORT(ANGLE_DOWN - ClientActive_t->baseAngle.x);
					else if (CE.AAPitch == 5)
						New->viewAngles[0] = ANGLE2SHORT(ANGLE_UP - ClientActive_t->baseAngle.x);

					if (CE.AARoll == 1)
					{
						if (rollAngle > 360)
							rollAngle -= 360;

						rollAngle += CE.RollSpinSpeed;

						New->viewAngles[2] = ANGLE2SHORT(rollAngle);

						ClientActive_t->viewAngle.z = -ClientActive_t->baseAngle.z;

					}
				}
				//TODO: add the NewNew wall detection exile sent
			}
			else
				bJustDied = true;


			if (bJustDied)
			{
				ClientActive_t->viewAngle.y += 0.01;
				bJustDied = false;
			}

		SedPacket:
			CL_WritePacketStub(r3);

		}
		else
		{
			bJustDied = true;
			CL_WritePacketStub(r3);
		}
	}
	__except (filter("CL_WritePacket", GetExceptionCode(), GetExceptionInformation()))
	{
		CL_WritePacketStub(r3);
	}
}