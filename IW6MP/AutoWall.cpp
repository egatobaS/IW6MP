#include "stdafx.h"
#include "AutoWall.h"
#include "Functions.h"
#include "Menu.h"

#include "Drawing.h"
#include "ESP.h"

#define vectorScale(v, s, o) (o.x = v.x * s, o.y = v.y * s, o.z = v.z * s)
#define VectorCopy(a,b)   ((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define VectorMA(v, s, b, o) ((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))

float GetWallDepth(Vector3 p1, Vector3 p2)
{
	Vector3 v = p2 - p1;
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float DidBulletDmg(int Client, Vector3 End) {
	BulletTraceResults br;
	BulletFireParams bp;

	BulletTraceResults br_2;
	BulletFireParams bp_2;

	Vector3 traceHitPos;

	bool hit = false, hit2 = false, hitall = false;

	int hitCount = 0;

	float wallDepth = 0.0f, penetrationPower = 0.0f, penetrationPower1 = 0.0f, penetrationPower2 = 0.0f;

	user_memset(&bp, 0x00, sizeof(BulletFireParams));

	unsigned int weapDef = (unsigned int)BG_GetWeaponDef(Entity[cg->ClientNumber].nextState.Weapon);

	if (!weapDef)
		return 0.0f;

	bp.weaponEntIndex = 2046;
	bp.ignoreEntIndex = cg->ClientNumber;
	bp.damageMultiplier = 1.0f;
	bp.methodOfDeath = MOD_PISTOL_BULLET;

	bp.origStart = cg->refdef.ViewOrigin;
	bp.start = cg->refdef.ViewOrigin;
	bp.end = End;

	bp.dir = VectorNormalize(bp.end - bp.start);

	//-----------------------------------
	hit = BulletTrace(0, &bp, &Entity[cg->ClientNumber], &br, 0);


	//if (br.trace.partGroup == 20)
	//	return 0.0f;

	if (!hit)
		return bp.damageMultiplier;

	if (Trace_GetEntityHitId(&br.trace) == Client)
		return bp.damageMultiplier;

	//if (br.trace.partGroup == 20)
	//	return 0.0f;

	while (bp.damageMultiplier > 0.0f)
	{
		penetrationPower1 = BG_GetSurfacePenetrationDepth(Entity[cg->ClientNumber].nextState.Weapon, false, br.depthSurfaceType);

		if (br.trace.surfaceFlags & 4)//
			penetrationPower1 = 100.0f;

		if (cg->characterinfo[cg->ClientNumber].perks[0] & 0x4000000)
			penetrationPower1 *= Dvar_GetFloat("perk_bulletPenetrationMultiplier");

		if (penetrationPower1 <= 0.0f)
			return 0.0f;

		traceHitPos = br.hitPos;

		if (!BG_AdvanceTrace(&bp, &br, 0.13500001))
			return 0.0f;

		hit = BulletTrace(0, &bp, &Entity[cg->ClientNumber], &br, br.depthSurfaceType);

		user_memcpy(&bp_2, &bp, sizeof(BulletFireParams));

		bp_2.dir = bp.dir * -1.0f;

		bp_2.start = bp.end;

		VectorMA(traceHitPos, 0.0099999998, bp_2.dir, bp_2.end);

		user_memcpy(&br_2, &br, sizeof(BulletTraceResults));

		vectorScale(br_2.trace.normal, -1.0f, br_2.trace.normal);

		if (hit)
			BG_AdvanceTrace(&bp_2, &br_2, 0.0099999998);

		hit2 = BulletTrace(0, &bp_2, &Entity[cg->ClientNumber], &br_2, br_2.depthSurfaceType);

		hitall = (hit2 && br_2.trace.allsolid) || (br.trace.startsolid && br_2.trace.startsolid);

		if (!hit2 && !hitall)
		{
			if (!hit)
				break;
			else
				goto LABEL_1;
		}

		if (hitall)
			wallDepth = GetWallDepth(bp_2.end, bp_2.start);
		else
			wallDepth = GetWallDepth(br_2.hitPos, traceHitPos);

		if (wallDepth < 1.0f)
			wallDepth = 1.0f;

		penetrationPower2 = BG_GetSurfacePenetrationDepth(Entity[cg->ClientNumber].nextState.Weapon, false, br_2.depthSurfaceType);

		if (cg->characterinfo[cg->ClientNumber].perks[0] & 0x4000000)
			penetrationPower2 *= Dvar_GetFloat("perk_bulletPenetrationMultiplier");

		if (hit2)
			penetrationPower = min(penetrationPower1, penetrationPower2);
		else
			penetrationPower = penetrationPower1;

		if (penetrationPower <= 0.0f)
			return 0.0f;

		bp.damageMultiplier -= wallDepth / penetrationPower;

	LABEL_1:
		if (!hit)
			break;
		else
		{
			//if (br.trace.partGroup == 20)
			//	return 0.0f;

			if (Trace_GetEntityHitId(&br.trace) == Client)
				return bp.damageMultiplier;

			if (++hitCount >= 5)
				break;
		}
	}

	if (!hit)
		return bp.damageMultiplier;

	return 0.0f;
}

bool BulletHitPlayer(Vector3 end, centity_s ent) {
	return (DidBulletDmg(ent.nextState.clientNum, end) > 0.0f);
}

bool BasicBulletHitPlayer(Vector3 end, centity_s ent)
{
	BulletFireParams bulletFireParams;
	bulletFireParams.weaponEntIndex = 2046;
	bulletFireParams.ignoreEntIndex = cg->ClientNumber;
	bulletFireParams.damageMultiplier = 1.0f;
	bulletFireParams.methodOfDeath = MOD_PISTOL_BULLET;
	bulletFireParams.origStart = cg->refdef.ViewOrigin;
	bulletFireParams.start = cg->refdef.ViewOrigin;
	bulletFireParams.end = end;
	AngleVectors(vectorToAngles(end - cg->refdef.ViewOrigin), &bulletFireParams.dir, NULL, NULL);

	*(int*)addr->_0x822C73C0 = addr->_0x4E800020;
	*(int*)addr->_0x82385B38 = addr->_0x4E800020;

	void(*FireBulletPenetrate) (int localClientNum, BulletFireParams* bulletFireParams, int weaponID, int unk2, centity_s* ent, int minus_1_param, Vector3 tracerStart, bool drawTracer) = (void(*)(int localClientNum, BulletFireParams* bulletFireParams, int weaponID, int unk2, centity_s* ent, int minus_1_param, Vector3 tracerStart, bool drawTracer))addr->FireBulletPenetrate;
	FireBulletPenetrate(0, &bulletFireParams, cg->ps.Weapon, 0, &Entity[cg->ClientNumber], -1, (float*)&cg->refdef.ViewOrigin, false);

	if (CE.Impacts)
	{
		*(int*)addr->_0x822C73C0 = addr->_0x7D8802A6;
		*(int*)addr->_0x82385B38 = addr->_0x7D8802A6;
	}

	return ((bulletFireParams.ignoreEntIndex == ent.nextState.clientNum) && (bulletFireParams.damageMultiplier > 0.0f));
}