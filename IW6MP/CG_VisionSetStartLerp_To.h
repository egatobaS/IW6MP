#pragma once

typedef int(*pCG_VisionSetStartLerp_ToStub)(int r3, int r4, int r5, char* vision, int duration);
extern pCG_VisionSetStartLerp_ToStub CG_VisionSetStartLerp_ToStub;
void CG_VisionSetStartLerp_ToHook(int r3, int r4, int r5, char* vision, int duration);