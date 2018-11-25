#pragma once

typedef int(*pCL_WritePacketStub)(int r3);
extern pCL_WritePacketStub CL_WritePacketStub;
void CL_WritePacketHook(int r3);