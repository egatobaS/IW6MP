#pragma once

typedef int(*pR_AddCmdDrawQuadPicStub)(float const* verts[2], float const* color, Material* material);
extern pR_AddCmdDrawQuadPicStub R_AddCmdDrawQuadPicStub;
void R_AddCmdDrawQuadPicHook(float const* verts[2], float const* color, Material* material);