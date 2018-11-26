#include "stdafx.h"
#include "CG_VisionSetStartLerp_To.h"
#include "Functions.h"

pCG_VisionSetStartLerp_ToStub CG_VisionSetStartLerp_ToStub;

void CG_VisionSetStartLerp_ToHook(int r3, int r4, char* vision, int duration, int r7)
{
	if (duration == 1337)
		CG_VisionSetStartLerp_ToStub(r3, r4, vision, 0, r7);
	else
	{
		if (cuser_strcmp(VisionSet, "ResetSetVision") || cuser_strcmp(VisionSet, ""))
			CG_VisionSetStartLerp_ToStub(r3, r4, vision, duration, r7);
		else
			CG_VisionSetStartLerp_ToStub(r3, r4, VisionSet, duration, r7);
	}
}