#pragma once

typedef int(*pCG_VisionSetStartLerp_ToStub)(int r3, int r4, char* vision, int duration, int r7);
extern pCG_VisionSetStartLerp_ToStub CG_VisionSetStartLerp_ToStub;
void CG_VisionSetStartLerp_ToHook(int r3, int r4, char* vision, int duration, int r7);