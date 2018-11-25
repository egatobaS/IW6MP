#include "stdafx.h"
#include "CG_Obituary.h"
#include "Functions.h"
#include "Menu.h"

pCG_ObituaryStub CG_ObituaryStub;

bool validateKill(int attacker, int victim, int methodOfDeath) {
	if (attacker == cg->ClientNumber)
		return true;

	Vector3 vAimLocation, vAimOrigin, AimEnd;
	if (!GetTagPos(victim, "j_spine4", &vAimLocation) || !GetTagPos(attacker, "tag_eye", &vAimOrigin))
		return true;

	AimEnd = AnglesToForward(vAimOrigin, Entity[attacker].nextState.lerp.apos.trBase, WallDepth((float*)vAimOrigin, (float*)vAimLocation));

	return !(methodOfDeath == 9 &&
		WallDepth(vAimOrigin, vAimLocation) >= 300.0f &&
		WallDepth(vAimLocation, AimEnd) >= 100.0f);
}

int KillSpree = 0, MultiKills = 0, MultiKillswait, MultiKillsTick;
void DoMultiKillsWait(int time) {
	MultiKillsTick = GetTickCount();
	MultiKillswait = time;
}

int KillFeedWait, KillFeedTick;
void DoKillFeedWait(int time) {
	KillFeedTick = GetTickCount();
	KillFeedWait = time;
}

void CG_ObituaryHook(int localClientNum, entityState_s *ent, int eventparam)
{
	__try
	{
		if (ShouldHookRun())
		{
			short victimIndex = ent->otherEntityNum, attackerIndex = ent->attackerEntityNum;
			int meansofdeath = ent->eventParm & 0xFFFFFF7F, weaponId = ent->eventParm & 0xFF;

			if (CE.MDetection && IsEnemy(attackerIndex) && !validateKill(attackerIndex, victimIndex, meansofdeath) && !ClientOptions[attackerIndex].Ignored && !ClientOptions[attackerIndex].Prioritized) {
				CG_GameMessage(0, "[%s]^1Modder Detected: %s", CheatName, cg->clientinfo[attackerIndex].Name);
				CG_GameMessage(0, "[%s]^1Prioritizing: %s", CheatName, cg->clientinfo[attackerIndex].Name);
				ClientOptions[attackerIndex].Prioritized = true;
			}

			if (attackerIndex == cg->ClientNumber)
			{
				/*if (meansofdeath == 9)
					PlayCustomSound("GAME:\\xbOnline\\UT Sounds\\Headshot.xma");

				if (meansofdeath == 0 || meansofdeath == 13)
					PlayCustomSound("GAME:\\xbOnline\\UT Sounds\\Suicide.xma");*/

				MultiKills++;
				KillSpree++;

				if (victimIndex == cg->ClientNumber) {
					if (KillSpree > 3) {
						if (CE.quakeprints)
							CG_GameMessage(0, "%s just ended %s's %i kill killing spree!", cg->clientinfo[attackerIndex].Name, cg->clientinfo[cg->ClientNumber].Name, KillSpree);
					}

					KillSpree = 0;
					MultiKills = 0;
				}

				if ((GetTickCount() - MultiKillsTick) > MultiKillswait) {
					MultiKills = 0;
				}

				//printf("MultiKills = %i, KillSpree = %i\n", MultiKills, KillSpree);

				/*if (MultiKills == 2)
				{
					PlayCustomSound("GAME:\\xbOnline\\UT Sounds\\streak\\2 - Double_Kill.xma");
				}
				else if (MultiKills == 3)
				{
					PlayCustomSound("GAME:\\xbOnline\\UT Sounds\\streak\\3-triplekill.xma");
				}
				else if (MultiKills == 4)
				{
					PlayCustomSound("GAME:\\xbOnline\\UT Sounds\\streak\\4-MultiKill.xma");
				}
				else if (MultiKills == 5)
				{
					PlayCustomSound("GAME:\\xbOnline\\UT Sounds\\streak\\5-MegaKill.xma");
				}
				else if (MultiKills == 6)
				{
					PlayCustomSound("GAME:\\xbOnline\\UT Sounds\\streak\\6-UltraKill.xma");
				}
				else if (MultiKills == 7)
				{
					PlayCustomSound("GAME:\\xbOnline\\UT Sounds\\streak\\7-MonsterKill_F.xma");
				}
				else if (MultiKills == 8)
				{
					PlayCustomSound("GAME:\\xbOnline\\UT Sounds\\streak\\8-LudicrousKill.xma");
				}
				else if (MultiKills >= 9)
				{
					PlayCustomSound("GAME:\\xbOnline\\UT Sounds\\streak\\9-HolyShit.xma");
				}*/

				if (KillSpree == 5)
				{
					//PlayCustomSound("GAME:\\xbOnline\\UT Sounds\\spree\\5-Killing_Spree.xma");
					if (CE.quakeprints)
						CG_GameMessage(0, "%s is on a Killing Spree!", cg->clientinfo[cg->ClientNumber].Name);
				}
				else if (KillSpree == 10)
				{
					//PlayCustomSound("GAME:\\xbOnline\\UT Sounds\\spree\\10-Rampage.xma");
					if (CE.quakeprints)
						CG_GameMessage(0, "%s is on a Rampage!", cg->clientinfo[cg->ClientNumber].Name);
				}
				else if (KillSpree == 15)
				{
					//PlayCustomSound("GAME:\\xbOnline\\UT Sounds\\spree\\15-Dominating.xma");
					if (CE.quakeprints)
						CG_GameMessage(0, "%s is Dominating!", cg->clientinfo[cg->ClientNumber].Name);
				}
				else if (KillSpree == 20)
				{
					//PlayCustomSound("GAME:\\xbOnline\\UT Sounds\\spree\\20-Unstoppable.xma");
					if (CE.quakeprints)
						CG_GameMessage(0, "%s is Unstoppable!", cg->clientinfo[cg->ClientNumber].Name);
				}
				else if (KillSpree == 25)
				{
					//PlayCustomSound("GAME:\\xbOnline\\UT Sounds\\spree\\25-GodLike.xma");
					if (CE.quakeprints)
						CG_GameMessage(0, "%s is Godlike!", cg->clientinfo[cg->ClientNumber].Name);
				}
				else if (KillSpree >= 30)
				{
					//PlayCustomSound("GAME:\\xbOnline\\UT Sounds\\spree\\30-whickedsick.xma");
					if (CE.quakeprints)
						CG_GameMessage(0, "%s is Wicked Sick!", cg->clientinfo[cg->ClientNumber].Name);
				}
				DoMultiKillsWait(4000);
			}
		}
	}
	__except (filter("CG_Obituary", GetExceptionCode(), GetExceptionInformation()))
	{

	}

	CG_ObituaryStub(localClientNum, ent, eventparam);
}