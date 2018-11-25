#pragma once

extern bool bDisConnected;
typedef int(*pCL_DisconnectStub)(int localClientNum, bool deactivateClient);
extern pCL_DisconnectStub CL_DisconnectStub;
void CL_DisconnectHook(int localClientNbDisConnectedum, bool deactivateClient);