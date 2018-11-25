#pragma once

void DoColors();

typedef void(*pR_AddCmdDrawStretchPicStub)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, float* color, Material* material);
extern pR_AddCmdDrawStretchPicStub R_AddCmdDrawStretchPicStub;
void R_AddCmdDrawStretchPicHook(float x, float y, float w, float h, float s0, float t0, float s1, float t1, float* color, Material* material);