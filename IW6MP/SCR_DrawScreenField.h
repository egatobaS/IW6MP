#pragma once

typedef int(*pSCR_DrawScreenFieldStub)(int r3, int r4);
extern pSCR_DrawScreenFieldStub SCR_DrawScreenFieldStub;
void SCR_DrawScreenFieldHook(int r3, int r4);