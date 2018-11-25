#include "stdafx.h"
#include "CG_VisionSetStartLerp_To.h"
#include "Functions.h"

pCG_VisionSetStartLerp_ToStub CG_VisionSetStartLerp_ToStub;

void CG_VisionSetStartLerp_ToHook(int r3, int r4, int r5, char* vision, int duration)
{
	if (duration == 1337)
		CG_VisionSetStartLerp_ToStub(r3, r4, r5, vision, 0);
	else
	{
		if (cuser_strcmp(VisionSet, "ResetSetVision") || cuser_strcmp(VisionSet, ""))
			CG_VisionSetStartLerp_ToStub(r3, r4, r5, vision, duration);
		else
			CG_VisionSetStartLerp_ToStub(r3, r4, r5, VisionSet, duration);
	}
}