#pragma once

typedef int(*pCL_WritePacketStub)(int r3);
extern pCL_WritePacketStub CL_WritePacketStub;

typedef int(*pCL_CreateNewCommandStub)(int r3);
extern pCL_CreateNewCommandStub CL_CreateNewCommandStub;

void CL_CreateNewCommandHook(int r3);
void CL_WritePacketHook(int r3);